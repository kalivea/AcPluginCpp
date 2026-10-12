param(
    [switch]$SdkBuild,
    [string]$SdkRoot = $env:ARX_SDK_ROOT,
    [string]$Toolset = 'v143',
    [ValidateSet('Debug', 'Release')][string]$Configuration = 'Release'
)
$ErrorActionPreference = 'Stop'
$repository = Split-Path $PSScriptRoot -Parent
& (Join-Path $repository 'tests/run_beam_tests.ps1')
& (Join-Path $repository 'tests/run_pillar_tests.ps1')
& (Join-Path $PSScriptRoot 'test-common-append.ps1')
& (Join-Path $repository 'tests/run_tools_tests.ps1')
& (Join-Path $PSScriptRoot 'test-menu-lifetime.ps1')
if ($SdkBuild) {
    if (!$SdkRoot -or !(Test-Path -LiteralPath (Join-Path $SdkRoot 'inc/dbmain.h'))) {
        throw 'Provide -SdkRoot pointing to an installed ObjectARX SDK.'
    }
    $locator = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
    $installation = & $locator -latest -products '*' -requires Microsoft.Component.MSBuild -property installationPath
    $msbuild = Join-Path $installation 'MSBuild/Current/Bin/MSBuild.exe'
    $targets = 'Tools;InteractiveTools;Common;Grid;CustomPillar;CustomBeam;CustomBeamUi;CustomPillarUi;EntityReactor;CustomWall;CustomEntityIntersection;DialogBarUi;DockControlBarUi;PaletteUi'
    $logDirectory = Join-Path $repository 'artifacts/stage1'
    New-Item -ItemType Directory -Path $logDirectory -Force | Out-Null
    $log = Join-Path $logDirectory ('sdk-' + $Configuration + '.log')
    & $msbuild (Join-Path $repository 'AcPluginCpp.sln') "/t:$targets" /m "/p:Configuration=$Configuration" /p:Platform=x64 "/p:PlatformToolset=$Toolset" "/p:ArxSdkDir=$SdkRoot" /v:minimal /nologo *> $log
    $result = $LASTEXITCODE
    Get-Content -LiteralPath $log -Tail 25
    if ($result -ne 0) { throw "Stage 1 SDK build failed ($result), see $log" }
    # Build the test command directly: the solution also declares unrelated IRM
    # dependencies whose Release properties are deferred to stage 3.
    $pluginLog = Join-Path $logDirectory 'sdk-test-plugin.log'
    & $msbuild (Join-Path $repository 'TestDbx/TestDbx.vcxproj') /t:Build "/p:SolutionDir=$repository\" "/p:Configuration=$Configuration" /p:Platform=x64 "/p:PlatformToolset=$Toolset" "/p:ArxSdkDir=$SdkRoot" /v:minimal /nologo *> $pluginLog
    $result = $LASTEXITCODE
    Get-Content -LiteralPath $pluginLog -Tail 15
    if ($result -ne 0) { throw "Stage 1 test plugin build failed ($result), see $pluginLog" }
}
Write-Output 'Stage 1 selected checks passed.'

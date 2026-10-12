param([string]$OutputDirectory = (Join-Path $env:TEMP 'AcPluginCpp-tools-tests'))
$ErrorActionPreference = 'Stop'
if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
    $locator = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    $installation = & $locator -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (-not $installation) { throw 'MSVC C++ tools not installed.' }
    $environmentScript = Join-Path $installation 'Common7\Tools\VsDevCmd.bat'
    $process = New-Object System.Diagnostics.Process
    $process.StartInfo.FileName = $env:ComSpec
    $process.StartInfo.Arguments = '/d /s /c ""' + $environmentScript + '" -arch=x64 -host_arch=x64 >nul && set"'
    $process.StartInfo.UseShellExecute = $false
    $process.StartInfo.CreateNoWindow = $true
    $process.StartInfo.RedirectStandardOutput = $true
    [void]$process.Start()
    $environmentValues = $process.StandardOutput.ReadToEnd()
    $process.WaitForExit()
    if ($process.ExitCode -ne 0) { throw 'MSVC environment initialization failed.' }
    $environmentValues -split "`r?`n" | ForEach-Object {
        if ($_ -match '^([^=]+)=(.*)$') { [Environment]::SetEnvironmentVariable($matches[1], $matches[2], 'Process') }
    }
}
New-Item -ItemType Directory -Path $OutputDirectory -Force | Out-Null
$repository = Split-Path $PSScriptRoot -Parent
$production = Get-Content (Join-Path $repository 'Tools\BasicTools.cpp') -Raw
foreach ($region in @(@('TOOLS_OFFSET_TEST','ToolsOffsetUnderTest.inc'), @('TOOLS_BORROWED_LINE_TEST','ToolsBorrowedLineUnderTest.inc'))) {
    $expression = '(?s)// ' + $region[0] + '_BEGIN\s*(.*?)// ' + $region[0] + '_END'
    $match = [regex]::Match($production, $expression)
    if (-not $match.Success) { throw "Production test region missing: $($region[0])" }
    Set-Content (Join-Path $OutputDirectory $region[1]) -Value $match.Groups[1].Value -Encoding utf8
}
$source = Join-Path $PSScriptRoot 'tools_geometry_tests.cpp'
$executable = Join-Path $OutputDirectory 'tools_geometry_tests.exe'
$object = Join-Path $OutputDirectory 'tools_geometry_tests.obj'
& cl.exe /nologo /std:c++17 /EHsc /W4 /WX "/I$OutputDirectory" $source "/Fe:$executable" "/Fo:$object"
if ($LASTEXITCODE -ne 0) { throw 'Tools host test compilation failed.' }
& $executable
if ($LASTEXITCODE -ne 0) { throw 'Tools host tests failed.' }

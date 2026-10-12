param([string]$OutputDirectory = (Join-Path $env:TEMP 'AcPluginCpp-beam-tests'))
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
$source = Join-Path $PSScriptRoot 'beam_dwg_tests.cpp'
$include = Join-Path $PSScriptRoot 'beam_stubs'
$executable = Join-Path $OutputDirectory 'beam_dwg_tests.exe'
$object = Join-Path $OutputDirectory 'beam_dwg_tests.obj'
& cl.exe /nologo /std:c++17 /EHsc /W4 /WX "/I$include" $source "/Fe:$executable" "/Fo:$object"
if ($LASTEXITCODE -ne 0) { throw 'Beam host test compilation failed.' }
& $executable
if ($LASTEXITCODE -ne 0) { throw 'Beam host tests failed.' }

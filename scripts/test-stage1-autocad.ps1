param(
    [string]$AutoCadDirectory = 'C:\Program Files\Autodesk\AutoCAD 2026',
    [ValidateSet('Debug', 'Release')][string]$Configuration = 'Release',
    [int]$TimeoutSeconds = 180,
    [switch]$FullAutoCad
)
$ErrorActionPreference = 'Stop'
$repository = Split-Path $PSScriptRoot -Parent
$console = Join-Path $AutoCadDirectory $(if ($FullAutoCad) { 'acad.exe' } else { 'accoreconsole.exe' })
if (!(Test-Path -LiteralPath $console)) { throw "AutoCAD console not found: $console" }
$output = Join-Path $repository ('artifacts/stage1/cad-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $output -Force | Out-Null
$commands = @('(setvar "FILEDIA" 0)', '(setvar "CMDDIA" 0)')
$modules = @('PolaTools.dbx', 'PolaCommon.dbx', 'PolaInteractiveTools.arx', 'PolaGrid.dbx', 'PolaCustomPillar.dbx', 'PolaCustomBeam.dbx', 'PolaEntityReactor.dbx', 'PolaCustomEntityIntersection.dbx', 'PolaTestDbx.arx')
foreach ($module in $modules) {
    $path = Join-Path $repository "x64/$Configuration/$module"
    if (!(Test-Path -LiteralPath $path)) { throw "Build stage 1 modules before running: $path" }
    $commands += '(arxload "' + $path.Replace('\', '/') + '")'
}
$commands += @('POLASTAGE1TEST', '_QUIT', '_N', '')
$script = Join-Path $output 'regression.scr'
Set-Content -LiteralPath $script -Value ($commands -join "`r`n") -Encoding ascii
$stdout = Join-Path $output 'stdout.log'
$stderr = Join-Path $output 'stderr.log'
$previousPath = $env:ACPLUGIN_STAGE1_DWG
$env:ACPLUGIN_STAGE1_DWG = Join-Path $output 'roundtrip.dwg'
$process = $null
try {
    $arguments = if ($FullAutoCad) { @('/nologo', '/t', 'acadiso.dwt', '/b', ('"' + $script + '"')) } else { @('/s', ('"' + $script + '"'), '/l', 'en-US') }
    $process = Start-Process -FilePath $console -ArgumentList $arguments -WorkingDirectory $output -WindowStyle Hidden -PassThru -RedirectStandardOutput $stdout -RedirectStandardError $stderr
    $deadline = [DateTime]::UtcNow.AddSeconds($TimeoutSeconds)
    $resultFile = $env:ACPLUGIN_STAGE1_DWG + '.result.txt'
    while (!$process.WaitForExit(5000)) {
        # The full application can keep a startup/exit dialog open after the
        # command finishes. Its independent result file is the completion gate.
        if ($FullAutoCad -and (Test-Path -LiteralPath $resultFile)) { break }
        if ([DateTime]::UtcNow -ge $deadline) { throw "AutoCAD regression timed out. Logs: $output" }
    }
    $text = if (Test-Path -LiteralPath $resultFile) { [string](Get-Content -LiteralPath $resultFile -Raw) } else { [string](Get-Content -LiteralPath $stdout -Raw) }
    Write-Output "AutoCAD logs: $output"
    $text = $text.Replace([string][char]0, '')
    $match = [regex]::Match($text, 'POLA_STAGE1_PASS checks=\d+')
    if (($process.HasExited -and $process.ExitCode -ne 0) -or !$match.Success -or $text -match 'POLA_STAGE1_FAIL') {
        Get-Content -LiteralPath $stderr -Tail 12
        Write-Output ($text.Substring([Math]::Max(0, $text.Length - 5000)))
        throw "AutoCAD regression did not pass (exit $($process.ExitCode))."
    }
    Write-Output $match.Value
    if (!$process.HasExited) { Write-Output 'Closing the disposable test process after the completed regression.' }
} finally {
    if ($process -and !$process.HasExited) { Stop-Process -Id $process.Id -Force }
    $env:ACPLUGIN_STAGE1_DWG = $previousPath
}

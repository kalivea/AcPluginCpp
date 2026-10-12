param([string]$Compiler = 'cl.exe')
$ErrorActionPreference = 'Stop'
$repository = Split-Path $PSScriptRoot -Parent
$temporaryRoot = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath())
$testDirectory = Join-Path $temporaryRoot ('AcPluginCpp-menu-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $testDirectory | Out-Null
try {
    $implementation = Get-Content -LiteralPath (Join-Path $repository 'DialogBarUi/CPolaMenu.cpp') -Raw
    $implementation = [regex]::Replace($implementation, '(?m)^#include[^\r\n]*(?:\r?\n|$)', '')
    Set-Content -LiteralPath (Join-Path $testDirectory 'MenuUnderTest.inc') -Value $implementation -Encoding utf8
    $executable = Join-Path $testDirectory 'menu_lifetime_tests.exe'
    $object = Join-Path $testDirectory 'menu_lifetime_tests.obj'
    & $Compiler /nologo /EHsc /std:c++17 /MD /D_AFXDLL /D_UNICODE /DUNICODE /W4 /WX (('/I' + $testDirectory)) (Join-Path $repository 'tests/menu_lifetime_tests.cpp') (('/Fe:' + $executable)) (('/Fo:' + $object))
    if ($LASTEXITCODE -ne 0) { throw "MFC menu regression compilation failed ($LASTEXITCODE)" }
    & $executable
    if ($LASTEXITCODE -ne 0) { throw "MFC menu regression failed ($LASTEXITCODE)" }
}
finally {
    $resolved = [System.IO.Path]::GetFullPath($testDirectory)
    if (!$resolved.StartsWith($temporaryRoot, [StringComparison]::OrdinalIgnoreCase)) { throw 'Test cleanup path escaped temp root' }
    Get-ChildItem -LiteralPath $resolved -File | Remove-Item -Force
    Remove-Item -LiteralPath $resolved
}

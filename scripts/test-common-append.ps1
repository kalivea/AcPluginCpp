param([string]$Compiler = 'cl.exe')
$ErrorActionPreference = 'Stop'
$repository = Split-Path $PSScriptRoot -Parent
$temporaryRoot = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath())
$testDirectory = Join-Path $temporaryRoot ('AcPluginCpp-common-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $testDirectory | Out-Null
try {
    $implementation = Get-Content -LiteralPath (Join-Path $repository 'Common/AddToModelSpace.cpp') -Raw
    # Exercise the actual production implementation with deterministic SDK mocks.
    $implementation = [regex]::Replace($implementation, '(?m)^#include[^\r\n]*(?:\r?\n|$)', '')
    Set-Content -LiteralPath (Join-Path $testDirectory 'AppendUnderTest.inc') -Value $implementation -Encoding utf8
    $executable = Join-Path $testDirectory 'common_append_tests.exe'
    $object = Join-Path $testDirectory 'common_append_tests.obj'
    & $Compiler /nologo /EHsc /std:c++17 /W4 /WX (('/I' + (Join-Path $repository 'Common'))) (('/I' + $testDirectory)) (Join-Path $repository 'tests/common_append_tests.cpp') (('/Fe:' + $executable)) (('/Fo:' + $object))
    if ($LASTEXITCODE -ne 0) { throw "Common append regression compilation failed ($LASTEXITCODE)" }
    & $executable
    if ($LASTEXITCODE -ne 0) { throw "Common append regression failed ($LASTEXITCODE)" }
}
finally {
    $resolved = [System.IO.Path]::GetFullPath($testDirectory)
    if (!$resolved.StartsWith($temporaryRoot, [StringComparison]::OrdinalIgnoreCase)) { throw 'Test cleanup path escaped temp root' }
    Get-ChildItem -LiteralPath $resolved -File | Remove-Item -Force
    Remove-Item -LiteralPath $resolved
}

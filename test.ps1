.\build.ps1

$exe = "build/Release/runTests.exe"
if (Test-Path $exe) {
    Write-Host "Running runTests..."
    & $exe
} else {
    Write-Error "$exe not found."
}

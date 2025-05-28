.\build.ps1

$exe = "build/Release/runUCI.exe"
if (Test-Path $exe) {
    Write-Host "Running runUCI..."
    & $exe
} else {
    Write-Error "$exe not found."
}

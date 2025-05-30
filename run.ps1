.\build.ps1

$exe = "build/Release/runChess.exe"
if (Test-Path $exe) {
    Write-Host "Running runChess..."
    & $exe
} else {
    Write-Error "$exe not found."
}

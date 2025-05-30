.\build.ps1

$exe = "build/Release/runBot.exe"
if (Test-Path $exe) {
    Write-Host "Running runBot..."
    & $exe
} else {
    Write-Error "$exe not found."
}
# Set up build directory
$buildDir = "build"
if (-Not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Navigate into build directory
Set-Location $buildDir

# Run CMake to configure the project
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build all targets
cmake --build . --config Release

# Return to project root directory
Set-Location ..

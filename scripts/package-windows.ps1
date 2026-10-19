param([string]$BuildDir = "build-release")
$ErrorActionPreference = "Stop"
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) { throw "CMake 3.24+ is required." }
cmake -S . -B $BuildDir -DCMAKE_BUILD_TYPE=Release -DCHESS_BUILD_GUI=OFF -DCHESS_BUILD_CLI=ON -DCHESS_BUILD_TESTS=ON
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
cmake --build $BuildDir --config Release
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
ctest --test-dir $BuildDir -C Release --output-on-failure
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$dist = Join-Path (Get-Location) "dist"
$portable = Join-Path $dist "RookForge"
$zip = Join-Path $dist "RookForge-1.0.0-windows-x64.zip"
if (Test-Path -LiteralPath $portable) { Remove-Item -LiteralPath $portable -Recurse -Force }
New-Item -ItemType Directory -Path $portable -Force | Out-Null
cmake --install $BuildDir --config Release --prefix $portable
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
if (Test-Path -LiteralPath $zip) { Remove-Item -LiteralPath $zip -Force }
Compress-Archive -Path (Join-Path $portable "*") -DestinationPath $zip -CompressionLevel Optimal
Write-Output "CLI portable archive: $zip"
Write-Output "This archive contains the command-line engine only; the desktop installer is not implemented."
exit 0

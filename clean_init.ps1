Write-Output "Cleaning all files   (git clean -f -d -X)"
git clean -f -d -X
Write-Output "Init git submodules"
git submodule init
git submodule update

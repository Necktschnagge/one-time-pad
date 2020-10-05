#!/bin/bash
echo "Cleaning all files   (git clean -f -d -X)"
git clean -f -d -X
echo "Init git submodules"
git submodule init
git submodule update

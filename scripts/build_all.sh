#!/bin/bash

set -e

# Release build
echo "Building Release..."
conan install .
cmake --preset conan-release
cmake --build --preset conan-release

# Debug builds
# sanitizers=("asan" "tsan" "msan" "none")
sanitizer=()

for sanitizer in "${sanitizers[@]}"; do
   echo "Building Debug with $sanitizer..."
   conan install . --settings=build_type=Debug --options=sanitizer="$sanitizer"
   cmake --preset conan-debug
   cmake --build --preset conan-debug
   
   # Rename executables with sanitizer suffix
    cd build/Debug
    if [[ -f "game" ]]; then
        sanitizer_clean="${sanitizer//+/_}"
        mv "game" "game_${sanitizer_clean}"
        echo "Renamed: game -> game_${sanitizer_clean}"
    fi
    cd ../..
done

echo ""
echo "All builds done!"
echo "Release executables: build/Release/"
# echo "Debug executables with sanitizer suffixes: build/Debug/"
# ls -l build/Debug/


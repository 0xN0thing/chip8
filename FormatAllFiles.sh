#!/bin/bash

search_dir="/your/starting/directory"

echo "Searching for .c, .cpp, .h, .hpp files in $search_dir ..."

# Find files and apply clang-format -i on each
find "$search_dir" -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -exec clang-format -i {} \;
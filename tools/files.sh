#!/bin/bash

# Check if correct number of arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <dir path> <number>"
    exit 1
fi

# Assign input arguments to variables
dir_path="$1"
number="$2"

# Check if directory exists
if [ ! -d "$dir_path" ]; then
    echo "Error: Directory '$dir_path' does not exist."
    exit 1
fi

# Loop to create files from 1 to the specified number
for ((i = 1; i <= number; i++)); do
    file_name="file-$i.txt"
    file_path="$dir_path/$file_name"
    echo "Hello from file $i" > "$file_path"
    echo "File '$file_name' created in directory '$dir_path'."
done

exit 0

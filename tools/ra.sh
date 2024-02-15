#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <total_byte_size> <output_file> <byte_size_per_line>"
    exit 1
fi

total_byte_size=$1
output_file=$2
byte_size_per_line=$3

newline_size=1

effective_byte_size_per_line=$((byte_size_per_line + newline_size))

total_lines=$((total_byte_size / effective_byte_size_per_line))

random_data=$(head -c "$((total_lines * byte_size_per_line))" /dev/urandom | base64)

formatted_data=$(echo "$random_data" | fold -b -w "$byte_size_per_line")

echo "$formatted_data" > "$output_file"

if [ -f "$output_file" ]; then
    file_size=$(wc -c < "$output_file")
    if [ -n "$file_size" ]; then
        if [ "$file_size" -gt "$total_byte_size" ]; then
            truncate -s "$total_byte_size" "$output_file" && echo "File size larger than specified total byte size. Truncated to $total_byte_size bytes."
        elif [ "$file_size" -lt "$total_byte_size" ]; then
            bytes_to_remove=$((file_size - total_byte_size))
            head -c "$((file_size - bytes_to_remove))" "$output_file" > "$output_file.tmp" && mv "$output_file.tmp" "$output_file"
            echo "File size smaller than specified total byte size. Removed $bytes_to_remove bytes."
        fi
    else
        echo "Error: Unable to determine file size."
    fi
else
    echo "Error: Output file not found."
fi

final_file_size=$(wc -c < "$output_file")
[ "$final_file_size" -eq "$total_byte_size" ] && echo "$final_file_size bytes: OK"
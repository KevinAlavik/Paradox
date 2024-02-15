#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <bytes> <out>"
    exit 1
fi

bytes="$1"
output_file="$2"

dd if=/dev/urandom bs=1 count="$bytes" > "$output_file"

final_file_size=$(wc -c < "$output_file")
[ "$final_file_size" -eq "$bytes" ] && echo "$final_file_size bytes: OK"
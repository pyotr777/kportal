#!/bin/bash

echo "Remove \\r in all files in src/ directory."

cd src
grep -I -lr $'\r' * 2>/dev/null >filelist.txt
echo "Replaced in:"
while read file; do
	echo -e "\033[38;5;214m$file\033[m"
	LC_ALL=C sed -i '' $'s/\r//' "$file"
done < filelist.txt

echo "Done."
echo "Check for files with \\r (with echo \"grep -I -lr '\r' *\"):"
grep -I -lr $'\r' *
rm filelist.txt

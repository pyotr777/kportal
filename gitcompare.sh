#!/bin/bash
#
# Compare one file between last N git commits.
# Require: diffuse
# Bryzgalov Peter (C) 2016 Stair.lab @ CHITECH

echo "Compare file between last N git commits."
echo ""
MAX=5
tmp_dir="tmp_gitcompare"

which diffuse &> /dev/null
if [ "$?" -ne 0 ]; then
	echo "Error: This script requires diffuse."
	exit 1
fi
git status &> /dev/null
if [ "$?" -ne 0 ]; then
	echo "Error: This script must be run in git repository working tree."
	exit 1
fi

if [ "$#" -lt 2 ]; then 
	printf "Usage:\n$0 <file name> <number of commits>\n"
	exit 1
fi
export FILE="$1"
export N="$2"

if [ ! -f "$FILE" ]; then
	echo "Error: File $FILE doesnt exist."
	exit 1
fi 

if [ "$N" -gt "$MAX" ]; then
	echo "Error: No more than $MAX commits."
	exit 1
fi
M=$((N-1))

mkdir -p $tmp_dir
git show HEAD:$FILE > $tmp_dir/$FILE
for i in $(seq 1 $M); do
	git show HEAD~$i:$FILE > $tmp_dir/file$i
done

command="diffuse"
for i in $(seq 1 $M); do
	j=$((N-i))
	command="$command $tmp_dir/file$j"
done
command="$command $tmp_dir/$FILE"
$command
rm -rf $tmp_dir
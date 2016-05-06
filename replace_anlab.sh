#!/bin/bash
set -e
if [ $# -lt 1 ]; then
    echo "Need name pattern as argument."
    exit 1
fi
echo "Replacing in $1 files"
find . -type f -name "$1" -exec sed -i.bak 's#/home/anlab/#/home/kportal/#g' {} \;

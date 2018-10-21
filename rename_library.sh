#!/bin/bash
#
# Rename all occurances of GCSG to a new value
#

if [ "$1" = "" ]; then
    echo "rename_library.sh NEWLIBRARYNAME"
    exit 1
fi

find . -type f -exec sed -i s/GCSG/$1/g {} +

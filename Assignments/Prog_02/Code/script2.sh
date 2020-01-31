#!/bin/bash
path="$1"
#putting each type of file into a list with conditions
source=`ls -p $path | egrep '\.c$|\.cpp$'`
header=`ls -p $path | egrep '\.h$|\.hpp$'`
script=`ls -p $path | egrep '\.sh$'`
text=`ls -p $path | egrep '\.txt$'`
directory=`ls -p $path  | egrep  '/$'`
other=`ls -p $path --ignore="*"{.c,.cpp,.h,.hpp,.sh,.txt} | egrep -v /$`
#A bunch of if/else statements that determine whether to print the categories if the list is valid or not
echo ""
if [ -z "$source" ]; then
    :
else
    echo "Source:"
    for entry in $source; do
        echo -n "$entry " 
    done
    echo ""
fi

if [ -z "$header" ]; then
    :
else

    echo "Header:"
    for entry in $header; do
        echo -n "$entry " 
    done
    echo ""
fi

if [ -z "$script" ]; then
    :
else

    echo "Script:"
    for entry in $script; do
        echo -n "$entry " 
    done
    echo ""
fi

if [ -z "$text" ]; then
    :
else

    echo "Text:"
    for entry in $text; do
        echo -n "$entry " 
    done
    echo ""
fi

if [ -z "$directory" ]; then
    :
else

    echo "Directory:"
    for entry in $directory; do
        echo -n "$entry " 
    done
    echo ""
fi

if [ -z "$other" ]; then
    :
else

    echo "Other:"
    for entry in $other; do
        echo -n "$entry " 
    done
    echo ""
fi
echo ""
#!/bin/bash
for i in "${@:2}"; do
    if ! [[ "$i" =~ ^[0-9]+$ ]]
    then
        echo "Invalid argument list."
        echo "Proper usage: myScript <path to executable> m1 [m2 [m3 [...]]]"
        exit 1
    fi
done

path="$1"

shift

#this figures out the highest element
for i in "$@"; do
    $path "$i"
    #numerically, top element
done | sort -n | head -1
# I could not figure out a good way to increment them so they didn't overlap
for i in "$@"; do
    for j in "${@:i}"; do
        $path $i $j
    done
done
#!/bin/bash
path="$1"
filenames=`ls -p $path | grep -v /`
for entry in $filenames
do
   echo $entry
done
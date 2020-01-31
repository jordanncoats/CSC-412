#!/bin/bash
#Takes the args and places them as variables
input=$1
output=$2
#Carriage return variable for comparison
cr=$'\r'
#opens the dir where the data files are
cd ./"$input"
#a counter to see how many proc_index there are
procNum=0

#This for loop looks through the dir for text and txt files then sees if they have a CR
#if they have a CR it will replace it with a NL instead, leaves NL alone.
for f in *.{text,txt}; do
test -f "$f" || continue
while read -n1 c; do
    if [[ $c = $cr ]]; then
       cat $f | tr -d '\n' | tr '\r' '\n'  > $f.new && mv $f.new $f
    fi
done < $f
done

#The next loop grabs just the first character and compares to to the value of procNum,
#e.g. if procNum is 2 and it reads a 4 it knows that 4 is now the highest value.
for f in *.{text,txt}; do
  test -f "$f" || continue
  count=$(head -c 1 $f)
  if (($procNum < $count));
  then
    procNum=$count
  fi
  
done
#Since we start at 0 and will always have at least 1 proc_index it adds to the value
#e.g. continued from above that 4 becomes a 5 since we know it was 0-4.
((procNum++))
#moves back to the original directory where script and prog05 are located.
cd "$OLDPWD"
#starts prog05 with the needed values
./prog05 $procNum "$input" "$output"


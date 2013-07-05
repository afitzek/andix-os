#!/bin/bash

echo "Searching dir: $1"
for f in $(find ./$1 -iname "*.[c|S|h]")
do
   if (! grep -q Copyright $f); then
   	echo "No Copyright in  $f"
   	cat copyright_notice.txt $f >$f.new && mv $f.new $f
   fi
done

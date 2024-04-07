#!/bin/bash

# execute script inside folder with STL files

mkdir img
for i in *.stl; do
  echo $i
  T=__tmp__$i
  B=`basename "$i"`
  echo import\(\"$i\"\)\; > "$T"
  /Applications/OpenSCAD.app/Contents/MacOS/OpenSCAD -o img/"$B".png --autocenter --viewall --imgsize=500,500 "$T"
  rm "$T"
done


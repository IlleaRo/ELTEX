#!/bin/bash
for ((i = 1; i < 6; i++)); do
  mkdir $i
  for ((j = 1; j < 11; j++)); do
    mkdir $i/$j
    for ((k = 1; k < 21; k++)); do
      touch $i/$j/$k.txt
    done
  done
done

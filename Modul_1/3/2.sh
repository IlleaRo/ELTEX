#!/bin/bash
read -rp "Input your path" path
cd $path
read -rp "Input quantity of first level path: " n_first
read -rp "Input quantity of second level path: " n_second
read -rp "Input quantity of files: " n_files
read -rp "Input pattern of folders: " folder_pattern
read -rp "Input pattern of files: " file_pattern
for ((i = 0; i < $n_first; i++)); do
  mkdir "${folder_pattern}_${i}"
  for ((j = 0; j < $n_second; j++)); do
    mkdir "${folder_pattern}_${i}/${folder_pattern}_${j}"
    for ((k = 0; k < $n_files; k++)); do
      touch "${folder_pattern}_${i}/${folder_pattern}_${j}/${file_pattern}_${k}.txt"
    done
  done
done

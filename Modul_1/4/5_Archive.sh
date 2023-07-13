#!/bin/bash

#Создадим файлы, которые предстоит архивировать
for ((k = 1; k < 4; k++)); do
      touch "file_${k}.txt"
    done

tar -cf archive.tar file_1.txt
printf "Архив с одним файлом:\n"
tar -tf archive.tar
tar --append --file=archive.tar file_2.txt
printf "Архив с двумя файлами:\n"
tar -tf archive.tar
tar --append --file=archive.tar file_3.txt
printf "Архив с тремя файлами:\n"
tar -tf archive.tar

tar --delete -f archive.tar file_2.txt
printf "Теперь архив без второго файла:\n"
tar -tf archive.tar
mkdir files_from_archive

tar -xf archive.tar -C files_from_archive

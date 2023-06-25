#!/bin/bash
read -rp "English phrase: " phrase
res=$(echo "$phrase" | sed 's/zh/ж/g'| sed 's/kh/x/g' | sed 's/ch/ч/g' |
sed 's/sh/ш/g' | sed 's/sch/щ/g' | sed 's/yu/ю/g'| sed 's/ya/я/g' |
sed 's/tc/ц/g' | sed 's/yo/йо/g' | sed 's/x/икс/g' |
sed 'y/abvgdeziilmnopruftkshw/абвгдезийлмнопруфтксxв/'|
sed 'y/ABVGDEZIILMNOPRUFTKSHW/АБВГДЕЗИЙЛМНОПРУФTКSХВ/')
echo "Transliterationed phrase: ${res}"

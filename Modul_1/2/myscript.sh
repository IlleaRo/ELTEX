#!/bin/bash
find /etc -group root | grep -Po 'etc.+?\K[^/]+$' | sed "y/abvgdezijklmnoprstufxcyw/абвгдезийклмнопрстуфхцыв/" | sed 's/q/кью/g' #Рекурсивный поиск файлов, принадлежащих группе "root" в директории "/etc"

find / -size 10M #Поиск файлов, которые больше 10 МБ

journalctl -u systemd-logind --since today --until tomorrow | grep -e New -e Removed | awk '{s=gsub(/ \S* systemd-logind\[[0-9]*\]:/, ".");}1'

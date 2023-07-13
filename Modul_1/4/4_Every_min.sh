#!/bin/bash
cd /home/illearo/Documents/ELTEX/Modul_1/4/ || return 1
time="$(date)"
key=$(grep -Po "\d+" < history.txt | tail -1)
key=$((++key))
echo "${time} I run ${key} time" >> history.txt

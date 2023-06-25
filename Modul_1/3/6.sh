#!/bin/bash

read -rp "Reservation purpose: " source
read -rp "Directory which will keep a copy: " distance
read -rp "Periodicity in seconds: " period

while true
do
    if [ -f "${distance}/backup.tar.bz2" ]
    then
        echo "[INFO] Deleting existing backup..."
        rm "${distance}/backup.tar.bz2"
        echo "Done!"
    fi
    echo "[INFO] Creating new backup..."
    tar -cjf "${distance}/backup.tar.bz2" "$source"
    echo "Done!"
    sleep "$period"
done

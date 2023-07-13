#!/bin/bash

read -rp "Input alaram time (HH:MM): " alarm_time
read -rp "Choose music file: " mFile
echo "konsole -e mplayer $mFile" | at "$alarm_time"

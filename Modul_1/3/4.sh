#!/bin/bash

read -rp "Input name of process: " pName
echo "Please use YYYY-MM-DD format of date!"
read -rp "Since: " since
read -rp "Until: " until

if [ ! -z "$pName" ]
then
    if [ ! -z "$since" ]
    then
        if [ ! -z "$until" ]
        then
            journalctl -r --since="$since" --until="$until" | grep "${pName}\["
        else
            journalctl -r --since="$since" | grep "${pName}\["
        fi
    elif [ ! -z "$until" ]
    then
        journalctl -r --until="$until" | grep "${pName}\["
    else
        journalctl -r | grep "${pName}\["
    fi
fi

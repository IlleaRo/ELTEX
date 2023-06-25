#!/bin/bash

pName=
since=
until=


while IFS= read -r line
do
    if [[ ! $line =~ "#" && $line =~ "Process_Name" ]]
    then
        pName=$(echo $line | grep -Po '=\K.+')
    elif [[ ! $line =~ "#" && $line =~ "since" ]]
    then
        since=$(echo $line | grep -Po '=\K.+')
    elif [[ ! $line =~ "#" && $line =~ "until" ]]
    then
        until=$(echo $line | grep -Po '=\K.+')
    fi
done < settings.conf

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

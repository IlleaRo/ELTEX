#!/bin/bash

printf 'Кол-во букв: %d\n' "$(sed 's/[^[:alpha:]]//g' alice.txt | tr -d '\n' | wc -c)"
printf 'Кол-во слов: %d\n' "$(wc -w < alice.txt)"
printf 'Первая строка с упоминанием королевы: %s' "$(grep -i 'королев[aыуо]' alice.txt | head -1)"
printf '\n'
printf 'Участники чаяпития:\n %s\n' "$(echo "$(grep -osz ' Шляпа и Заяц.*белочки' alice.txt)" |  grep -osP '[А-Я][а-я]+[^[:punct:],[:space:]]' | sort -u)"
printf '\n'


#[A-Z][a-z]+[^[:punct:],[:space:]]
#echo 'Here is Bob Bob' | grep -oP '[A-Z][a-z]+[^[:punct:],[:space:]]' | sort -u

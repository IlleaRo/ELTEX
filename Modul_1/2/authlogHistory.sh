#!/bin/bash
read -rp "Requested user: " ruser # Считываем переменную

# Смотрим в журнале события systemd-logind, которые произошли со
# вчерашнего дня по сегодняшний
journalctl -u systemd-logind --since "yesterday" --until "tomorrow" |
# Отсеиваем неинтерисующие нас события
  grep -Pe "New session.+${ruser}|Removed.+\ \d" |
# Выводим только дату и событие в файл
  awk '{$4=$5="";print $0}' > "${ruser}s_history".txt

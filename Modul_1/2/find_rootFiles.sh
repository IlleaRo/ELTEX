#!/bin/bash
# Рекурсивный поиск всех файлов и подкатологов в "/etc", которые
# принадлежат группе суперпользователя
# /etc - место поиска; group - название группы, которой принадлежит
# grep - выводить строки, согласно заданному выражению
# P - использование регулярных выражений, совместимоых с perl
# o - печать, только соответсвующих строк

find /etc -group root | grep -Po '[^/]+$'

# [^/] - начинаются с символа "/"; + - любое количество символов
# $ - якорь - Совпадение возможно, если оно имеется в конце

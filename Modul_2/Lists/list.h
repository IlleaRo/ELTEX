//
// Created by illearo on 7/4/23.
//

#ifndef LISTS_LIST_H
#define LISTS_LIST_H

#define TYPE int

typedef struct node node;
typedef struct list list;

// Функции вставки нового узла
void pushHead(list** head, TYPE* data); //Добавить элемент в начало
void pushOnPlace(list** head, int place, TYPE* data); //Вставка на заданную позицию
void pushBack(list** head, TYPE* data); //Добавить элемент в конец (Если позиция больше размера, то вставляет в конец!!!!)

// Функции удаления узла
void delHead(list*); //Удаление начала списка
void delEnd(list*); //Удаление конца списка
void delWithIndex(list*, int); //Удаление по индексу

// Функции редактирования существующего узла
void editHead(list*, TYPE*);
void editEnd(list*, TYPE*);
void editWithId(list*,int,TYPE*);

// Вывод списка в консоль
void printFromHead(list*); // Вывод с головы
void printFromEnd(list*); // Вывод с хвоста

// Генерация списка со случайными значениями
void fillWithRNum(list**, int); //Создать новый лист указанного размера

// Очистка листа
void cleanList(list* head);


// Сортировка шейкером
void shakeSort(list*);

// Обратная сортировка расческой
void combSort(list*);

#endif //LISTS_LIST_H
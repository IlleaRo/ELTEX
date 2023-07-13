//
// Created by illearo on 7/5/23.
//

#ifndef RED_BLACK_TREE_TREE_H
#define RED_BLACK_TREE_TREE_H

#define TYPE int

typedef struct TreeNode TreeNode; // Структура вершины дерева

// Вывод дерева на экран
void showLine(char* c, int p, int s);
void showTree(TreeNode* wood, int p, int s);


TreeNode* find(TreeNode*, TYPE); // Поиск узла в дереве, содержащего искомое значение
void fixInsertion(TreeNode**, TreeNode*); // Проверка выполнения условий после вставки
void leftRotate(TreeNode**, TreeNode*); // Левый поворот
void rightRotate(TreeNode**, TreeNode*); // Правый поворот
void insertRed(TreeNode**, TYPE); // Обычная вставка в бинарное дерево
void clearTree(TreeNode**); // Очистка дерева

#endif //RED_BLACK_TREE_TREE_H

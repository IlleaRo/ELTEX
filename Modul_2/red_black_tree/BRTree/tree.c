//
// Created by illearo on 7/5/23.
//

// Прекрасная статья про алгоритмы: http://mech.math.msu.su/~vvb/2course/Borisenko/lecTree.html

//    1) Узел может быть либо красным, либо чёрным и имеет двух потомков;
//    2) Корень — как правило чёрный.
//    3) Все листья, не содержащие данных — чёрные.
//    4) Оба потомка каждого красного узла — чёрные.
//    5) Любой простой путь от узла-предка до листового узла-потомка содержит одинаковое число чёрных узлов.

#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

typedef struct TreeNode{
  struct TreeNode* left;
  struct TreeNode* right;
  struct TreeNode* parent;
  TYPE* value;
  bool isRed;
} TreeNode;

TreeNode* find(TreeNode* root, TYPE x){
    if (!root) return NULL;
    TreeNode* v = root, *vp = root;
    TYPE val;
    while (v){
        val = *v->value;
        if (val==x) return v;
        else if (x < val){
            vp = v;
            v = v->left;
        } else {
            vp = v;
            v = v->right;
        }
    }
    return vp;
}


void showLine(char* c, int p, int s) {
    int t = s;
    for (int i = 0; i < p; i++) {
        printf(t & 1 ? "|  " : "   ");
        t /= 2;
    }
    printf("%s", c);
}
void showTree(TreeNode* wood, int p, int s) { // string s = string("")) {
    if (!wood) return;
    printf("%d (%c)\n", *wood->value, wood->isRed? 'R':'B');
    if (wood->left) {
        showLine("|\n", p, s); showLine("L: ", p, s);
        showTree(wood->left, p + 1, s + ((!wood->right ? 0 : 1) << p));
    }
    if (wood->right) {
        showLine("|\n", p, s); showLine("R: ", p, s);
        showTree(wood->right, p + 1, s);
    }
}

void insertRed(TreeNode** root, TYPE value){
    // Создание нового узла и заполнение его данными
    TreeNode* tNode = (TreeNode*) malloc(sizeof(TreeNode));
    tNode->right = NULL; tNode->left = NULL;
    tNode->value = (TYPE*) malloc(sizeof (TYPE));
    *tNode->value = value;
    tNode->isRed=true; // Новый узел красим в красный
    if (!*root){
        *root = tNode;
        tNode->parent = NULL;
        return fixInsertion(root,tNode);
    } // Устанавливаем новый корень, если нам передано пустое дерево
    TreeNode* p = *root;
    TreeNode* q = NULL;
    while (p){
        q = p;
        if (*p->value==value)
            return;
        else if (*p->value<value)
            p = p->right;
        else p = p->left;
    }
    tNode->parent = q;
    if (*q->value<value)
        q->right=tNode;
    else
        q->left=tNode;
    fixInsertion(root,tNode); // Проверка выполнения условий вставки
}

void fixInsertion(TreeNode** root, TreeNode* node){
    if (*root==node){
        node->isRed=false;
        return;
    } // Если нам передан только корень, то окрашиваем его в черный цвет и завершаем работу

    TreeNode* dad = node->parent;
    TreeNode* grandFather = dad->parent;
    TreeNode* uncle = NULL;
    if (grandFather){
        if (grandFather->left==dad)
            uncle = grandFather->right;
        else
            uncle = grandFather->left;
    }

    while (dad->isRed){
        if (grandFather->left==dad){  // Отец левый сын деда
            if (uncle && uncle->isRed){ // Случай 1 (красный дядя)
                dad->isRed=false; // Перекрашиваем отца в черный
                uncle->isRed=false; // Перекрашиваем дядю в черный
                grandFather->isRed=true; // Перекрашиваем деда в красный
                node = grandFather; // Переходим к рассмотрению деда

                // Ниже происходит "переопределение" родни
                dad = node->parent;
                if (!dad) break;
                grandFather = dad->parent;
                if (grandFather){
                    if (grandFather->left==dad)
                        uncle = grandFather->right;
                    else
                        uncle = grandFather->left;

                }
            } else { // Дядя черный или его нет
                if (dad->right==node){ // узел является правым сыном отца
                    node=dad; // Переходим к рассмотрению отца
                    leftRotate(root, node); // Левый поворот относительно отца
                }
                dad->isRed=false; // Перекрашиваем отца в черный
                grandFather->isRed=true; // Перекрашиваем деда в красный
                rightRotate(root,grandFather); // Правое вращение деда
            }
        } else { // Отец - правый сын деда - зеркальная ситуация
            if (uncle && uncle->isRed){
                dad->isRed=false;
                uncle->isRed=false;
                grandFather->isRed=true;
                node = grandFather;
                dad = node->parent;
                if (!dad) break;
                grandFather = dad->parent;
                if (grandFather){
                    if (grandFather->left==dad)
                        uncle = grandFather->right;
                    else
                        uncle = grandFather->left;
                }
            } else {
                if (dad->left==node){
                    node = dad;
                    rightRotate(root, node);
                }
                dad->isRed=false;
                grandFather->isRed=true;
                leftRotate(root,grandFather);
            }
        }
    }
    (*root)->isRed = false;
}

void leftRotate(TreeNode** root, TreeNode* node){
    TreeNode* r = node->right;
    TreeNode* parent = node->parent;
    r->parent = parent;
    if (parent){
        if (node == parent->left)
            parent->left = r;
        else
            parent->right = r;
    }
    node->right = r->left;
    if (r->left)
        r->left->parent = node;
    r->left = node;
    node->parent = r;
    if (*root==node){
        *root = r;
    }
}
void rightRotate(TreeNode** root ,TreeNode* node){
    TreeNode* l = node->left;
    TreeNode* parent = node->parent;
    l->parent = parent;
    if (parent){
        if (node == parent->left)
            parent->left = l;
        else
            parent->right = l;
    }

    node->left = l->right;
    if (l->right)
        l->right->parent = node;
    l->right = node;
    node->parent = l;
    if (*root==node){
        *root = l;
    }
}

void clearTree(TreeNode** root){
    if (!*root) return;
    clearTree(&(*root)->left);
    clearTree(&(*root)->right);
    free((*root)->value);
    free(*root);
    *root = NULL;
}
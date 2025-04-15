#include <iostream>
using namespace std;

struct node
{
    int data;
    node *left;
    node *right;
    int height;
};

class AVL
{
private:
    node *root;

    node *insert(node *root, int data);
    node *get(node *root, int data);
    node *del(node *root, int data);
    int getHeight(node *root);

public:
    void insert(int data);
    node *get(int data);
    void del(int data);
};
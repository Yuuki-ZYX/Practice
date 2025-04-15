#include "avl_myself.h"

void AVL::insert(int data)
{
    insert(root, data);
}
node *AVL::get(int data)
{
    return get(root, data);
}
void AVL::del(int data)
{
    del(root, data);
}
node *AVL::insert(node *root, int data)
{
    
}
node *AVL::get(node *root, int data)
{

}
node *AVL::del(node *root, int data)
{

}
int AVL::getHeight(node *root)
{
    return root->height;
}
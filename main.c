#include "avltree.h"

void foreach_tree_inode(struct avl_inode *tree)
{
	if(tree) {
		foreach_tree_inode(tree->l_tree);
		printf("%d  ", tree->key);
		foreach_tree_inode(tree->r_tree);
	}
}

int main(int argc, char *argv[])
{
	int i;
	int a[10]={3,2,1,4,5,6,7,10,9,8};
	struct avl_inode *root = NULL;
	struct avl_inode *inode = NULL;

	for(i = 0;i < 10; i++){
		inode = avl_alloc(a[i]);
		root = avl_insert(root, inode);
	}
	printf("中序遍历二叉平衡树:\n");
	foreach_tree_inode(root);
	printf("\n");
#if 0
	printf("删除结点元素5后中序遍历:\n");
	inode = avl_get_inode(5);
	root = avl_delete(root, inode);
	foreach_tree_inode(T);
	printf("\n");
	avl_free(inode);
#endif
	return 0;
}

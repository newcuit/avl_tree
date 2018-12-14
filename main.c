#include "avltree.h"

static int do_tree(struct avl_inode *tree)
{
	printf("%d  ", tree->key);

	return 0;
}

int main(int argc, char *argv[])
{
	int i = 0;
	int status = 0;
	struct avl_inode *root = NULL;
	struct avl_inode *inode = NULL;
	int key[] = {3,2,1,4,5,6,7,10,9,8,14,12,13,11,15,15,16,15,20,21,22,19,18,17};

	for (i = 0; i < sizeof(key) / sizeof(key[0]); i++)
		root = avl_insert(root, avl_alloc(key[i]), &status);
	
	printf("中序遍历二叉平衡树:\n");
	avl_doit(root, do_tree);
	printf("\n");

	inode = avl_search(root, 5);
	root = avl_delete(root, inode);
	avl_free(inode);

	printf("删除结点元素5后中序遍历:\n");
	avl_doit(root, do_tree);
	printf("\n");

	return 0;
}

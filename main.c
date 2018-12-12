#include "avltree.h"

static int do_tree(struct avl_inode *tree)
{
	printf("%d  ", tree->key);
	return 0;
}

int main(int argc, char *argv[])
{
	int i;
	int status;
	struct avl_inode *root = NULL;
	struct avl_inode *inode = NULL;
	int a[]={3,2,1,4,5,6,7,10,9,8,14,12,13,11,15,15,16,15,20,21,22,19,18,17};

	for(i = 0;i < sizeof(a)/sizeof(int); i++) {
		status = 0;
		inode = avl_alloc(a[i]);

		//printf("root inode:%p key:%d insert %d->%p", 
		//		root, root?root->key:-1, a[i], inode);
		root = avl_insert(root, inode, &status);
		//printf(" status:%08X\n", status);
	}
	printf("中序遍历二叉平衡树:\n");
	avl_doit(root, do_tree);
	printf("\n");

	printf("删除结点元素5后中序遍历:\n");
	inode = avl_search(root, 5);
	root = avl_delete(root, inode);
	avl_free(inode);

	avl_doit(root, do_tree);
	printf("\n");

	return 0;
}

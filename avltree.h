#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <stdlib.h>

#define AVL_L_HIGH            1
#define AVL_BALANCE           0
#define AVL_R_HIGH           -1

#ifndef likely
#define likely(exp)         __builtin_expect(!!(exp), 1)
#endif

#ifndef unlikely
#define unlikely(Exp)       __builtin_expect(!!(Exp), 0)
#endif

struct avl_inode {
	int key;
	int8_t bf;
	struct avl_inode *l_tree, *r_tree;
};

int avl_free(struct avl_inode *tree);
struct avl_inode *avl_alloc(int key);
struct avl_inode *avl_insert(struct avl_inode *root, struct avl_inode *inode);
struct avl_inode *avl_delete(struct avl_inode *root, struct avl_inode *inode);

#endif

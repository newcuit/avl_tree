#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <stdlib.h>

#define AVL_EMPTY 			 0x01
#define AVL_BALANCE			 0x02
#define AVL_CHANGE           0x03
#define AVL_R_HIGH           0x04
#define AVL_L_HIGH           0x05
#define AVL_EXISTS           0x80

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

int avl_free(struct avl_inode *);
struct avl_inode *avl_alloc(int);
typedef int (*avl_func_t)(struct avl_inode *);
int avl_doit(struct avl_inode *root, avl_func_t cb);
struct avl_inode *avl_search(struct avl_inode *root, int key);
struct avl_inode *avl_insert(struct avl_inode *, struct avl_inode *, int *);
struct avl_inode *avl_delete(struct avl_inode *, struct avl_inode *);

#endif

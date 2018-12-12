#include "avltree.h"

static struct avl_inode *r_rotate(struct avl_inode *root)
{
	struct avl_inode *tree = root->l_tree;

	root->l_tree = tree->r_tree;
	tree->r_tree = root;

	return tree;
}

static struct avl_inode *l_rotate(struct avl_inode *root)
{
	struct avl_inode *tree = root->r_tree;

	root->r_tree = tree->l_tree;
	tree->l_tree = root;

	return tree;
}

static struct avl_inode *right_balance(struct avl_inode *root)
{
	struct avl_inode *tree = root->r_tree;

	if(tree->bf == AVL_R_HIGH) {
		root->bf = AVL_BALANCE;
		tree->bf = AVL_BALANCE;
		root = l_rotate(root);
	} else if(tree->bf == AVL_L_HIGH) {
		switch(tree->l_tree->bf) {
		case AVL_L_HIGH:
			root->bf = AVL_BALANCE;
			tree->bf = AVL_R_HIGH;
			break;
		case AVL_BALANCE:
			root->bf = AVL_BALANCE;
			tree->bf = AVL_BALANCE;
			break;
		case AVL_R_HIGH:
			root->bf = AVL_L_HIGH;
			tree->bf = AVL_BALANCE;
			break;
		}
		tree->l_tree->bf = AVL_BALANCE;
		root->r_tree = r_rotate(root->r_tree);
		root = l_rotate(root);
	}

	return root;
}

static struct avl_inode *left_balance(struct avl_inode *root)
{
	struct avl_inode *tree = root->l_tree;

	if(tree->bf == AVL_L_HIGH) {
		root->bf = AVL_BALANCE;
		tree->bf = AVL_BALANCE;
		root = r_rotate(root);
	} else if(tree->bf == AVL_R_HIGH) {
		switch(tree->r_tree->bf) {
		case AVL_L_HIGH:
			root->bf = AVL_R_HIGH;
			tree->bf = AVL_BALANCE;
			break;
		case AVL_BALANCE:
			root->bf = AVL_BALANCE;
			tree->bf = AVL_BALANCE;
			break;
		case AVL_R_HIGH:
			root->bf = AVL_BALANCE;
			tree->bf = AVL_L_HIGH;
			break;
		}
		tree->r_tree->bf = AVL_BALANCE;
		root->l_tree = l_rotate(root->l_tree);
		root = r_rotate(root);
	}

	return root;
}

int avl_doit(struct avl_inode *root, avl_func_t cb)
{
	if(likely(root != NULL)) {
		avl_doit(root->l_tree, cb);
		if(likely(cb)) cb(root);
		avl_doit(root->r_tree, cb);
	}

	return 0;
}

struct avl_inode *avl_insert(struct avl_inode *root, struct avl_inode *inode, int *s)
{
	struct avl_inode *new_root = root;

	if(unlikely(root == NULL)) {
		*s |= AVL_CHANGE;
		inode->bf = AVL_BALANCE;
		return inode;
	}

	if(inode->key > root->key) {
		root->r_tree = avl_insert(root->r_tree, inode, s);
		if(unlikely(*s & AVL_CHANGE)) {
			switch(root->bf) {
			case AVL_L_HIGH:
				*s &= (~AVL_CHANGE);
				root->bf = AVL_BALANCE;
				break;
			case AVL_BALANCE:
				*s |= AVL_CHANGE;
				root->bf = AVL_R_HIGH;
				break;
			case AVL_R_HIGH:
				*s &= (~AVL_CHANGE);
				new_root = right_balance(root);
				break;
			}
		}
	} else if(inode->key < root->key) {
		root->l_tree = avl_insert(root->l_tree, inode, s);
		if(unlikely(*s & AVL_CHANGE)) {
			switch(root->bf) {
			case AVL_R_HIGH:
				*s &= (~AVL_CHANGE);
				root->bf = AVL_BALANCE;
				break;
			case AVL_BALANCE:
				*s |= AVL_CHANGE;
				root->bf = AVL_L_HIGH;
				break;
			case AVL_L_HIGH:
				*s &= (~AVL_CHANGE);
				new_root = left_balance(root);
				break;
			}
		}
	} else {
		*s |= AVL_EXISTS;
	}

	return new_root;
}

static int __avl_delete(struct avl_inode **tree, int key)
{
	int avl= AVL_EMPTY;
	struct avl_inode *inode = (*tree);

	if(unlikely((inode) == NULL)) return avl;

	if(key > inode->key) {
		avl = __avl_delete(&inode->r_tree, key);
		if(avl & AVL_CHANGE) {
			switch(inode->bf) {
			case AVL_L_HIGH:
				*tree = left_balance(inode);
				avl = inode->l_tree->bf == AVL_BALANCE?0:1;
				break;
			case AVL_BALANCE:
				avl = AVL_BALANCE;
				inode->bf = AVL_L_HIGH;
				break;
			case AVL_R_HIGH:
				inode->bf = AVL_BALANCE;
				break;
			}
		}
	}
	if(key < inode->key) {
		avl = __avl_delete(&inode->l_tree, key);
		if(avl & AVL_CHANGE) {
			switch(inode->bf) {
			case AVL_R_HIGH:
				*tree = right_balance(inode);
				avl = inode->r_tree->bf == AVL_BALANCE?0:1;
				break;
			case AVL_BALANCE:
				avl = AVL_BALANCE;
				inode->bf = AVL_R_HIGH;
				break;
			case AVL_L_HIGH:
				inode->bf = AVL_BALANCE;
				break;
			}
		}
	}

	if(unlikely(inode->key == key)) {
		if(inode->l_tree == NULL) {
			avl = AVL_CHANGE;
			*tree = inode->r_tree;
		} else if (inode->r_tree == NULL) {
			avl = AVL_CHANGE;
			inode = inode->l_tree;
		} else {
			inode = inode->l_tree;
			while(inode->r_tree) inode = inode->r_tree;
			avl = __avl_delete(&(*tree)->l_tree, inode->key);
			inode->l_tree = (*tree)->l_tree;
			inode->r_tree = (*tree)->r_tree;
			*tree = inode;
		}
	}

	return avl;
}

struct avl_inode *avl_delete(struct avl_inode *root, struct avl_inode *inode)
{
	__avl_delete(&root, inode->key);
	return root;
}

struct avl_inode *avl_search(struct avl_inode *root, int key)
{
	struct avl_inode *inode = root;

	if(unlikely(root == NULL)) return NULL;

	if(key > root->key) {
		inode = avl_search(root->r_tree, key);
	}
	if(key < root->key){
		inode = avl_search(root->l_tree, key);
	}
	return inode;
}

struct avl_inode *avl_alloc(int key)
{
	struct avl_inode *tree = NULL;

	tree = malloc(sizeof(struct avl_inode));
	if(unlikely(tree == NULL)) return NULL;

	tree->key = key;
	tree->bf = AVL_BALANCE;
	tree->l_tree = NULL;
	tree->r_tree = NULL;
	return  tree;
}

int avl_free(struct avl_inode *tree)
{
	if(likely(tree != NULL)) free(tree);

	return 0;
}

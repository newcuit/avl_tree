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
		root = r_rotate(root);
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

struct avl_inode *avl_insert(struct avl_inode *root, struct avl_inode *inode)
{
	struct avl_inode *tree = NULL;
	struct avl_inode *new_root = NULL;

	if(unlikely(root == NULL)) {
		inode->bf = AVL_BALANCE;
		return inode;
	}

	if(inode->key > root->key) {
		tree = avl_insert(root->r_tree, inode);
		if(unlikely(tree != NULL)) {
			root->r_tree = tree;
			switch(root->bf) {
			case AVL_L_HIGH:
				root->bf = AVL_BALANCE;
				break;
			case AVL_BALANCE:
				new_root = root;
				root->bf = AVL_R_HIGH;
				break;
			case AVL_R_HIGH:
				new_root = right_balance(root);
				break;
			}
		}
	} else if(inode->key < root->key) {
		tree = avl_insert(root->l_tree, inode);
		if(unlikely(tree != NULL)) {
			root->l_tree = tree;
			switch(root->bf) {
			case AVL_R_HIGH:
				root->bf = AVL_BALANCE;
				break;
			case AVL_BALANCE:
				new_root = root;
				root->bf = AVL_L_HIGH;
				break;
			case AVL_L_HIGH:
				new_root = left_balance(root);
				break;
			}
		}
	}

	return new_root;
}

struct avl_inode *avl_delete(struct avl_inode *root, struct avl_inode *inode)
{
	struct avl_inode *new_root = NULL;

	if(unlikely(root == NULL)) {
		return NULL;
	}

	if(inode->key > root->key) {
		new_root = avl_delete(root->r_tree, inode);
		if(new_root == NULL) {
			
		}
	}
	if(inode->key < root->key) {
		new_root = avl_delete(root->l_tree, inode);
	}

	if(unlikely(inode->key == root->key)) {
		return NULL;
	}

	return new_root;
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

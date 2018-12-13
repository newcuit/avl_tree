#include "avltree.h"

/*****************************************************************************
 * * FunctionName   : r_rotate()
 * * Description    : avl右旋转
 * * EntryParameter : root,指向需要旋转的root结点
 * * ReturnValue    : 返回旋转后的root结点
 * ***************************************************************************/
static struct avl_inode *r_rotate(struct avl_inode *root)
{
	struct avl_inode *tree = root->l_tree;

	root->l_tree = tree->r_tree;
	tree->r_tree = root;

	return tree;
}

/*****************************************************************************
 * * FunctionName   : l_rotate()
 * * Description    : avl左旋转
 * * EntryParameter : root,指向需要旋转的root结点
 * * ReturnValue    : 返回旋转后的root结点
 * ***************************************************************************/
static struct avl_inode *l_rotate(struct avl_inode *root)
{
	struct avl_inode *tree = root->r_tree;

	root->r_tree = tree->l_tree;
	tree->l_tree = root;

	return tree;
}

/*****************************************************************************
 * * FunctionName   : right_balance()
 * * Description    : avl平衡右子树
 * * EntryParameter : root,指向需要平衡的右子树结点
 * * ReturnValue    : 返回平衡后的新结点
 * ***************************************************************************/
static struct avl_inode *right_balance(struct avl_inode *root)
{
	struct avl_inode *tree = root->r_tree;

	// 1. 需要右平衡的结点的右子树高（即符号相同,同为右高）， 直接左旋转
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
		// 2. 需要右平衡的结点的左子树高（即符号相反）,需要先反向旋转，再左旋转
		tree->l_tree->bf = AVL_BALANCE;
		root->r_tree = r_rotate(root->r_tree);
		root = l_rotate(root);
	}

	return root;
}

/*****************************************************************************
 * * FunctionName   : left_balance()
 * * Description    : avl平衡左子树
 * * EntryParameter : root,指向需要平衡的左子树结点
 * * ReturnValue    : 返回平衡后的新结点
 * ***************************************************************************/
static struct avl_inode *left_balance(struct avl_inode *root)
{
	struct avl_inode *tree = root->l_tree;

	// 1. 需要左平衡的结点的左子树高（即符号相同,同为左高）， 直接右旋转
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
		// 2. 需要左平衡的结点的右子树高（即符号相反）,需要先反向旋转，再右旋转
		tree->r_tree->bf = AVL_BALANCE;
		root->l_tree = l_rotate(root->l_tree);
		root = r_rotate(root);
	}

	return root;
}

/*****************************************************************************
 * * FunctionName   : avl_doit()
 * * Description    : avl从小到大依次遍历，执行cb函数
 * * EntryParameter : root,指向树根结点, cb周期处理函数
 * * ReturnValue    : 返回0
 * ***************************************************************************/
int avl_doit(struct avl_inode *root, avl_func_t cb)
{
	if(unlikely(!root)) return 0;

	avl_doit(root->l_tree, cb);
	if(likely(cb)) cb(root);
	avl_doit(root->r_tree, cb);
	return 0;
}

/*****************************************************************************
 * * FunctionName   : avl_insert()
 * * Description    : avl插入一个结点inode
 * * EntryParameter : root,指向树根结点, inode,要插入的结点，s状态码
 * * ReturnValue    : 返回root根结点（由于底层需要平衡树，root结点可能变动）
 * ***************************************************************************/
struct avl_inode *avl_insert(struct avl_inode *root, struct avl_inode *inode, int *s)
{
	struct avl_inode *new_root = root;

	// 1.当前结点为空，表示遍历到了尽头，将插入结点当成尽头结点返回
	if(unlikely(root == NULL)) {
		*s |= AVL_CHANGE;
		inode->bf = AVL_BALANCE;
		return inode;
	}

	// 2.key值大于当前结点，遍历右子树
	if(inode->key > root->key) {
		root->r_tree = avl_insert(root->r_tree, inode, s);
		// 2.1 AVL在插入新节点后，树高发生变动
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
				// 2.2 AVL右树比左树高2，需要右平衡
				new_root = right_balance(root);
				break;
			}
		}
	// 3.key值小于当前结点，遍历左子树
	} else if(inode->key < root->key) {
		root->l_tree = avl_insert(root->l_tree, inode, s);
		// 3.1 AVL在插入新节点后，树高发生变动
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
				// 3.2 AVL左树比右树高2，需要左平衡
				new_root = left_balance(root);
				break;
			}
		}
	} else {
		*s |= AVL_EXISTS;
	}

	return new_root;
}

/*****************************************************************************
 * * FunctionName   : __avl_delete()
 * * Description    : avl删除一个键值为key的结点
 * * EntryParameter : tree,指向树根结点, key,键值
 * * ReturnValue    : 返回tree结点，删除key后的状态
 * ***************************************************************************/
static int __avl_delete(struct avl_inode **tree, int key)
{
	int avl= AVL_EMPTY;
	struct avl_inode *inode = (*tree);

	// 1. 当前结点为NULL，表明未找到key,返回EMPTY
	if(unlikely((inode) == NULL)) return avl;

	// 2.key值大于当前结点，遍历右子树
	if(key > inode->key) {
		avl = __avl_delete(&inode->r_tree, key);
		// 2.1 AVL在删除节点后，树高发生变动
		if(avl & AVL_CHANGE) {
			switch(inode->bf) {
			case AVL_L_HIGH:
				// 2.3 AVL左树比右树高2，需要左平衡
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
	// 3.key值小于当前结点，遍历左子树
	if(key < inode->key) {
		avl = __avl_delete(&inode->l_tree, key);
		// 3.1 AVL在删除节点后，树高发生变动
		if(avl & AVL_CHANGE) {
			switch(inode->bf) {
			case AVL_R_HIGH:
				// 3.3 AVL右树比左树高2，需要右平衡
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

	// 4. 找到key,执行删除动作
	if(unlikely(inode->key == key)) {
		// 4.1 root左子树不存在 直接用root的右子树作为新的root
		if(inode->l_tree == NULL) {
			avl = AVL_CHANGE;
			*tree = inode->r_tree;
		// 4.2 root右子树不存在 直接用root的左子树作为新的root
		} else if (inode->r_tree == NULL) {
			avl = AVL_CHANGE;
			inode = inode->l_tree;
		// 4.3 root左右子树都存在，则用AVL左子树的最右边结点作为新的root结点
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

/*****************************************************************************
 * * FunctionName   : avl_delete()
 * * Description    : avl删除一个结点inode
 * * EntryParameter : root,指向树根结点, inode,待删除的结点
 * * ReturnValue    : 返回root结点
 * ***************************************************************************/
struct avl_inode *avl_delete(struct avl_inode *root, struct avl_inode *inode)
{
	__avl_delete(&root, inode->key);
	return root;
}

/*****************************************************************************
 * * FunctionName   : avl_search()
 * * Description    : avl查询key键值的结点地址
 * * EntryParameter : root,指向树根结点, key,键值
 * * ReturnValue    : 返回查找到的inode结点
 * ***************************************************************************/
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

/*****************************************************************************
 * * FunctionName   : avl_init()
 * * Description    : 初始化一段avl结点地
 * * EntryParameter : key,键值
 * * ReturnValue    : 返回inode结点
 * ***************************************************************************/
struct avl_inode *avl_init(struct avl_inode *tree, int key)
{
	if(unlikely(!tree)) return tree;

	tree->key = key;
	tree->l_tree = NULL;
	tree->r_tree = NULL;
	tree->bf = AVL_BALANCE;

	return  tree;
}

/*****************************************************************************
 * * FunctionName   : avl_alloc()
 * * Description    : 申请一段avl结点地址，并初始化
 * * EntryParameter : key,键值
 * * ReturnValue    : 返回inode结点
 * ***************************************************************************/
struct avl_inode *avl_alloc(int key)
{
	struct avl_inode *tree = NULL;

	tree = malloc(sizeof(struct avl_inode));
	if(unlikely(tree == NULL)) return NULL;

	return avl_init(tree, key);
}

/*****************************************************************************
 * * FunctionName   : avl_free()
 * * Description    : 释放一段avl结点地址
 * * EntryParameter : tree, 待释放结点
 * * ReturnValue    : 返回0
 * ***************************************************************************/
int avl_free(struct avl_inode *tree)
{
	if(unlikely(!tree)) return -1;

	free(tree);
	return 0;
}

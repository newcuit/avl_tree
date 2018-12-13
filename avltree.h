#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <stdlib.h>

#define AVL_EMPTY 			 0x01 // AVL未找到对应结点
#define AVL_BALANCE			 0x02 // AVL结点是平衡结点
#define AVL_CHANGE           0x03 // AVL结点高度发生改变
#define AVL_R_HIGH           0x04 // AVL结点右子树高
#define AVL_L_HIGH           0x05 // AVL结点左子树高
#define AVL_EXISTS           0x80 // AVL结点已经存在

#ifndef likely
#define likely(exp)         __builtin_expect(!!(exp), 1)
#endif

#ifndef unlikely
#define unlikely(Exp)       __builtin_expect(!!(Exp), 0)
#endif

/*****************************************************************************
 * * Description    : 定义avl树结点信息
 * ***************************************************************************/
struct avl_inode {
	int key;                          // 键值
	int8_t bf;                        // 平衡因子（L, B，R）
	struct avl_inode *l_tree;         // 左子树
	struct avl_inode *r_tree;         // 右子树
};

/*****************************************************************************
 * * FunctionName   : avl_free()
 * * Description    : 释放一段avl结点地址
 * * EntryParameter : tree, 待释放结点
 * * ReturnValue    : 返回0
 * ***************************************************************************/
int avl_free(struct avl_inode *);

/*****************************************************************************
 * * FunctionName   : avl_alloc()
 * * Description    : 申请一段avl结点地址，并初始化
 * * EntryParameter : key,键值
 * * ReturnValue    : 返回inode结点
 * ***************************************************************************/
struct avl_inode *avl_alloc(int);

/*****************************************************************************
 * * FunctionName   : avl_init()
 * * Description    : 初始化一段avl结点地
 * * EntryParameter : key,键值
 * * ReturnValue    : 返回inode结点
 * ***************************************************************************/
struct avl_inode *avl_init(struct avl_inode *tree, int key);

typedef int (*avl_func_t)(struct avl_inode *);

/*****************************************************************************
 * * FunctionName   : avl_doit()
 * * Description    : avl从小到大依次遍历，执行cb函数
 * * EntryParameter : root,指向树根结点, cb周期处理函数
 * * ReturnValue    : 返回0
 * ***************************************************************************/
int avl_doit(struct avl_inode *root, avl_func_t cb);

/*****************************************************************************
 * * FunctionName   : avl_search()
 * * Description    : avl查询key键值的结点地址
 * * EntryParameter : root,指向树根结点, key,键值
 * * ReturnValue    : 返回查找到的inode结点
 * ***************************************************************************/
struct avl_inode *avl_search(struct avl_inode *root, int key);

/*****************************************************************************
 * * FunctionName   : avl_depth()
 * * Description    : 获取avl树深度
 * * EntryParameter : root,执行avl树根
 * * ReturnValue    : 返回树深度
 * ***************************************************************************/
int avl_depth(struct avl_inode *root);

/*****************************************************************************
 * * FunctionName   : avl_insert()
 * * Description    : avl插入一个结点inode
 * * EntryParameter : root,指向树根结点, inode,要插入的结点，s状态码
 * * ReturnValue    : 返回root根结点（由于底层需要平衡树，root结点可能变动）
 * ***************************************************************************/
struct avl_inode *avl_insert(struct avl_inode *, struct avl_inode *, int *);

/*****************************************************************************
 * * FunctionName   : avl_delete()
 * * Description    : avl删除一个结点inode
 * * EntryParameter : root,指向树根结点, inode,待删除的结点
 * * ReturnValue    : 返回root结点
 * ***************************************************************************/
struct avl_inode *avl_delete(struct avl_inode *, struct avl_inode *);

#endif

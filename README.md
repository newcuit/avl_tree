# AVL树   API使用说明， main.c为测试测序

1、释放一个avl结点， 该结点必须是不在avl树中的结点
int avl_free(struct avl_inode *inode)

2、以key作为关键键 申请一个avl结点
struct avl_inode *avl_alloc(int key);

3、定义avl_doit的回调函数
typedef int (*avl_func_t)(struct avl_inode *);

4、在root指向的avl树中，从小到大遍历key，并执行cb函数回调
int avl_doit(struct avl_inode *root, avl_func_t cb);

5、从avl树种 查找key所在的inode结点， 返回查找到的结点，没找到为NULL
struct avl_inode *avl_search(struct avl_inode *root, int key);

6、将一个inode结点插入到root指向的avl树中，并排序， status返回状态值(AVL_EXISTS表示结点已经存在)， 函数返回新的root结点（排序后，avl树的根结点可能发生改变）
struct avl_inode *avl_insert(struct avl_inode *root, struct avl_inode *inode, int *status);

7、将一个inode结点从root指向的avl树中删除，删除后，才可以用avl_free释放，函数返回新的root结点（avl树中结点被删除，可能导致avl根结点发生变化）
struct avl_inode *avl_delete(struct avl_inode *root, struct avl_inode *inode);

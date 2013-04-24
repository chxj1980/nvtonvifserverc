#ifndef LIST_H_
#define LIST_H_

#if defined(__cplusplus)
extern "C" {
#endif

typedef void* Element;
  
typedef struct node{
    Element element;
    struct node * nextNode;
}*Node;

typedef struct list *List;
//无序列表
struct list{
    Node dataNodes;
    void (*addFirst)(List this,Element);      //添加元素到列表头.
    void (*addLast)(List this,Element);  //添加元素到列表尾部.
    void (*removeFirst)(List this);           //删除列表头部元素.
    void (*removeLast)(List this);       //删除链表尾部元素.
    Node (*first)(List this);            //获取列表第一个节点.
    Node (*last)(List this);             //获取列表最后一节点.
    int (*size)(List this);
    void (*removeNode)(Node);            //节点资源释放函数
    void (*forEach)(List this,void(*)(Node));          //遍历节点
    Node (*get)(List this, int index);  // 得到指定位置的节点
    void (*deleteIndex)(List this, int index);  // 得到指定位置的节点
};

List newList(void (*remove)(Node));      //new一个列表对象
void delList(List this);                 //删除一个列表对象

#if defined(__cplusplus)
}
#endif

#endif /* LIST_H_ */

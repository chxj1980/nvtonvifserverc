#ifndef LIST_H_
#define LIST_H_

#if defined(__cplusplus)
extern "C" {
#endif

typedef void* ListElement;
  
typedef struct listnode{
    ListElement element;
    struct listnode * nextNode;
}*ListNode;

typedef struct list *List;
//无序列表
struct list{
    ListNode dataNodes;
    void (*addFirst)(List this,ListElement);      //添加元素到列表头.
    void (*addLast)(List this,ListElement);  //添加元素到列表尾部.
    void (*removeFirst)(List this);           //删除列表头部元素.
    void (*removeLast)(List this);       //删除链表尾部元素.
    ListNode (*first)(List this);            //获取列表第一个节点.
    ListNode (*last)(List this);             //获取列表最后一节点.
    int (*size)(List this);
    void (*clearNode)(List this, ListNode);            //节点释放函数
    void (*clearElement)(ListElement);            //节点资源释放函数
    void (*forEach)(List this,void(*)(ListNode));          //遍历节点
    ListNode (*get)(List this, int index);  // 得到指定位置的节点
    void (*deleteIndex)(List this, int index);  // 得到指定位置的节点
};

List newList(void (*clearElement)(ListElement));      //new一个列表对象
void delList(List this); 	//删除一个列表对象

#if defined(__cplusplus)
}
#endif

#endif /* LIST_H_ */

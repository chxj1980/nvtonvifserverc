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
//�����б�
struct list{
    ListNode dataNodes;
    void (*addFirst)(List this,ListElement);      //���Ԫ�ص��б�ͷ.
    void (*addLast)(List this,ListElement);  //���Ԫ�ص��б�β��.
    void (*removeFirst)(List this);           //ɾ���б�ͷ��Ԫ��.
    void (*removeLast)(List this);       //ɾ������β��Ԫ��.
    ListNode (*first)(List this);            //��ȡ�б��һ���ڵ�.
    ListNode (*last)(List this);             //��ȡ�б����һ�ڵ�.
    int (*size)(List this);
    void (*clearNode)(List this, ListNode);            //�ڵ��ͷź���
    void (*clearElement)(ListElement);            //�ڵ���Դ�ͷź���
    void (*forEach)(List this,void(*)(ListNode));          //�����ڵ�
    ListNode (*get)(List this, int index);  // �õ�ָ��λ�õĽڵ�
    void (*deleteIndex)(List this, int index);  // �õ�ָ��λ�õĽڵ�
};

List newList(void (*clearElement)(ListElement));      //newһ���б����
void delList(List this); 	//ɾ��һ���б����

#if defined(__cplusplus)
}
#endif

#endif /* LIST_H_ */

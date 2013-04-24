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
//�����б�
struct list{
    Node dataNodes;
    void (*addFirst)(List this,Element);      //���Ԫ�ص��б�ͷ.
    void (*addLast)(List this,Element);  //���Ԫ�ص��б�β��.
    void (*removeFirst)(List this);           //ɾ���б�ͷ��Ԫ��.
    void (*removeLast)(List this);       //ɾ������β��Ԫ��.
    Node (*first)(List this);            //��ȡ�б��һ���ڵ�.
    Node (*last)(List this);             //��ȡ�б����һ�ڵ�.
    int (*size)(List this);
    void (*removeNode)(Node);            //�ڵ���Դ�ͷź���
    void (*forEach)(List this,void(*)(Node));          //�����ڵ�
    Node (*get)(List this, int index);  // �õ�ָ��λ�õĽڵ�
    void (*deleteIndex)(List this, int index);  // �õ�ָ��λ�õĽڵ�
};

List newList(void (*remove)(Node));      //newһ���б����
void delList(List this);                 //ɾ��һ���б����

#if defined(__cplusplus)
}
#endif

#endif /* LIST_H_ */

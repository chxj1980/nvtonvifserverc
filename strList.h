/*
 * strList.h
 *
 *  Created on: 2013-9-24
 *      Author: PC01
 */

#ifndef STRLIST_H_
#define STRLIST_H_

#include "list.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct strList* PStrList;
//�����б�
struct strList{
    List list;
    void (*addFirst)(PStrList this,const char*);      //���Ԫ�ص��б�ͷ.
    void (*addLast)(PStrList this,const char*);  //���Ԫ�ص��б�β��.
    void (*removeFirst)(PStrList this);           //ɾ���б�ͷ��Ԫ��.
    void (*removeLast)(PStrList this);       //ɾ������β��Ԫ��.
    char* (*first)(PStrList this);            //��ȡ�б��һ���ڵ�.
    char* (*last)(PStrList this);             //��ȡ�б����һ�ڵ�.
    int (*size)(PStrList this);
    void (*forEach)(PStrList this, void(*)(char*, void*), void*);          //�����ڵ�
    char* (*get)(PStrList this, int index);  // �õ�ָ��λ�õĽڵ�
    void (*deleteIndex)(PStrList this, int index);  // �õ�ָ��λ�õĽڵ�
    void (*clearAll) (List this);   // �������
};

PStrList newStrList();      //newһ���б����
void delStrList(PStrList this); 	//ɾ��һ���б����

#if defined(__cplusplus)
}
#endif

#endif /* STRLIST_H_ */

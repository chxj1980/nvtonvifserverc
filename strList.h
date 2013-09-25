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
//无序列表
struct strList{
    List list;
    void (*addFirst)(PStrList this,const char*);      //添加元素到列表头.
    void (*addLast)(PStrList this,const char*);  //添加元素到列表尾部.
    void (*removeFirst)(PStrList this);           //删除列表头部元素.
    void (*removeLast)(PStrList this);       //删除链表尾部元素.
    char* (*first)(PStrList this);            //获取列表第一个节点.
    char* (*last)(PStrList this);             //获取列表最后一节点.
    int (*size)(PStrList this);
    void (*forEach)(PStrList this, void(*)(char*, void*), void*);          //遍历节点
    char* (*get)(PStrList this, int index);  // 得到指定位置的节点
    void (*deleteIndex)(PStrList this, int index);  // 得到指定位置的节点
    void (*clearAll) (List this);   // 清除所有
};

PStrList newStrList();      //new一个列表对象
void delStrList(PStrList this); 	//删除一个列表对象

#if defined(__cplusplus)
}
#endif

#endif /* STRLIST_H_ */

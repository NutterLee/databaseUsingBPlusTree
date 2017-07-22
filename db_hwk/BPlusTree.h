#pragma once
#include"node.h"

class BPlusTree 
{
public:
	BPlusTree();
	pair<int,int>* search(int key);
	//insert插入键值对，如果是key已经存在则不操作
	bool insert(int key, int value);
	//删除key对应的元素和key 不存在的话就返回false
	bool remove(int key);
	//modify也是修改键值对，如果key不存在则不操作，否则重新修改其值
	bool modify(int key, int value);
	bool isEmpty() { return size; }
	interiorNode* getRoot() { return root; }
	//对内部节点操作以后要调用这个函数设置head（TODO 改进地方？？？）
//	void setHead();
	interiorNode* getHead() { return head; }
	//根据传入的key 返回可能存有它的叶子节点的指针
	interiorNode* moveToLeaf(int key);
	//遍历打印出所存的key value键值对,num是要打印的数据数目
	//超过了内在的数目则打印出全部的内容
	void traverse(int num);

private:
	//解决一切overflow问题，处理完叶子节点的overflow以后要递归调用非叶子结点的overflow
	//注意： 两个overflow的标准似乎不同？一个是key的数目，一个是item的数目（虽然把他们都设置成一样了）
	void solveOverflow(interiorNode*);
	//leaf的overflow和interiorNode的overflow不一样，下面这个用于解决非叶子节点的overflow问题
	void solveInnerOverflow(interiorNode*);
	void solveUnderflow(interiorNode* node, int key);
	void solveInnerUnderflow(interiorNode* node,int key);
	//函数说明：设置tree中的head值，在删除，插入操作以后要调用
	void setHead();
	//函数说明：更新父节点，再删除后使用
	//功能 删除父辈中可能存在的key值，不引发underflow，潜在的underflow由其函数单独检查
	void updateParent(interiorNode* node, int key);
	int size;
	//M是阶数
	int M;
	interiorNode* root;
	//叶子节点链表头
	interiorNode* head;
};
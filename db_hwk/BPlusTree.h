#pragma once
#include"node.h"

class BPlusTree 
{
public:
	BPlusTree();
	pair<int,int>* search(int key);
	//insert�����ֵ�ԣ������key�Ѿ������򲻲���
	bool insert(int key, int value);
	//ɾ��key��Ӧ��Ԫ�غ�key �����ڵĻ��ͷ���false
	bool remove(int key);
	//modifyҲ���޸ļ�ֵ�ԣ����key�������򲻲��������������޸���ֵ
	bool modify(int key, int value);
	bool isEmpty() { return size; }
	interiorNode* getRoot() { return root; }
	//���ڲ��ڵ�����Ժ�Ҫ���������������head��TODO �Ľ��ط���������
//	void setHead();
	interiorNode* getHead() { return head; }
	//���ݴ����key ���ؿ��ܴ�������Ҷ�ӽڵ��ָ��
	interiorNode* moveToLeaf(int key);
	//������ӡ�������key value��ֵ��,num��Ҫ��ӡ��������Ŀ
	//���������ڵ���Ŀ���ӡ��ȫ��������
	void traverse(int num);

private:
	//���һ��overflow���⣬������Ҷ�ӽڵ��overflow�Ժ�Ҫ�ݹ���÷�Ҷ�ӽ���overflow
	//ע�⣺ ����overflow�ı�׼�ƺ���ͬ��һ����key����Ŀ��һ����item����Ŀ����Ȼ�����Ƕ����ó�һ���ˣ�
	void solveOverflow(interiorNode*);
	//leaf��overflow��interiorNode��overflow��һ��������������ڽ����Ҷ�ӽڵ��overflow����
	void solveInnerOverflow(interiorNode*);
	void solveUnderflow(interiorNode* node, int key);
	void solveInnerUnderflow(interiorNode* node,int key);
	//����˵��������tree�е�headֵ����ɾ������������Ժ�Ҫ����
	void setHead();
	//����˵�������¸��ڵ㣬��ɾ����ʹ��
	//���� ɾ�������п��ܴ��ڵ�keyֵ��������underflow��Ǳ�ڵ�underflow���亯���������
	void updateParent(interiorNode* node, int key);
	int size;
	//M�ǽ���
	int M;
	interiorNode* root;
	//Ҷ�ӽڵ�����ͷ
	interiorNode* head;
};
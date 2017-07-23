#pragma once
#include"file.h"
#include "cache.h"
//˵����
//���������û������ݿ�ӿ�ͷ�ļ�
//testʱҲ���ô˴��Ľӿ�

class database
{
public:	
	~database();
	void db_open();
	void db_close();
	//��key�м����ֵ�ԣ���Ӧ�����Ѵ��ڵ�
	bool db_store(int key, int vvalue);
	//��ѯ��������db���ҳ�key����Ӧ��ֵ
	int db_fetch(int key);
	//���db
	void db_clear();
	//�޸�db��һ����ֵ��
	bool db_modify(int key, int value);

private:
	readBuffer* inner_read_buffer;
	BPlusTree* inner_tree;
};
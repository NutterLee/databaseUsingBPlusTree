#include "interface.h"

database::~database()
{
	if(inner_read_buffer!=nullptr)
	delete inner_read_buffer;
	if(inner_tree!=nullptr)
	delete inner_tree;
}

void database::db_open()
{
	inner_read_buffer = new readBuffer();
	fstream file("index.dat");
	inner_tree = new BPlusTree();
	if (file.is_open())
	{
		file.close();
		readFromFile((*inner_tree), "index.dat");
	}
}

void database::db_close()
{
	//TODO:
	//step 1 ��д�뻺�������д������
	//step 2 ɾ��������ڴ�ռ�
}

bool database::db_store(int key, int value)
{
	//step 1 �ȿ�д����������û��Ҫstore�� �еĻ����滻��
	//step2 û������������������Ӧ��ֵ
	return false;
}

int database::db_fetch(int key)
{
	//step1 ���ڻ�����Ѱ�� ��������������в��Ҳ�����д�뻺����
	int result = inner_read_buffer->find(key);
	if (result == -1)
	{
		auto tmp_ptr=inner_tree->search(key);
		if (tmp_ptr == nullptr) return -1;
		inner_read_buffer->add(tmp_ptr->first, tmp_ptr->second);
		return tmp_ptr->second;
	}
	else return result;
}

void database::db_clear()
{
	delete inner_read_buffer;
	delete inner_tree;
	inner_read_buffer = nullptr;
	inner_tree = nullptr;
}

bool database::db_modify(int key, int value)
{
	return (*inner_tree).modify(key, value);
}

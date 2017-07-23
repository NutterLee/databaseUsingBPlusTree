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
	//step 1 将写入缓存的数据写入数据
	//step 2 删除申请的内存空间
}

bool database::db_store(int key, int value)
{
	//step 1 先看写缓存里面有没有要store的 有的话就替换他
	//step2 没有则往缓存里面加入对应的值
	return false;
}

int database::db_fetch(int key)
{
	//step1 先在缓存中寻找 如果不存在则到树中查找并将其写入缓存中
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

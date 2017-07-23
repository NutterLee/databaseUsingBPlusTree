#pragma once
#include"file.h"
#include "cache.h"
//说明：
//这是面向用户的数据库接口头文件
//test时也调用此处的接口

class database
{
public:	
	~database();
	void db_open();
	void db_close();
	//向key中加入键值对，不应该是已存在的
	bool db_store(int key, int vvalue);
	//查询函数，从db中找出key所对应的值
	int db_fetch(int key);
	//清空db
	void db_clear();
	//修改db的一个键值对
	bool db_modify(int key, int value);

private:
	readBuffer* inner_read_buffer;
	BPlusTree* inner_tree;
};
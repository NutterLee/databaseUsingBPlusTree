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
	//删除db中的键值对
	bool db_delete(int key);

private:
	//将write_cache的东西写入到data.dat中，并设定其在B+树中的偏移量/删除此节点
	//指令分为 “remove”“add”“modify”三种
	//对于remove的 value和offPos的值设定为-1
	//对于add的 offPos应等于-1
	void writeToFile(int key,int value, int offPos,string operation,fstream& file);
	//根据偏移量从data.dat中取得数据，一次取一个叶子节点的大小的数据，并存到读缓存中
	//第二个参数是一次取出的键值对数目
	void readFromData(int offPos, int nums);
	//会判断是否有必要进行refresh
	void refreshWriteCache();
	//强制刷新写缓存
	void refreshWriteCache_force();
	readBuffer* inner_read_buffer;
	writeBuffer* inner_write_buffer;
	BPlusTree* inner_tree;
};
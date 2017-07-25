#pragma once
//缓存设计类
//缓存设计总思路：
//固定大小，优先读取
//除了特定操作外，要强制刷新，其余都会自己维护缓存容量
//每次达到容量上限时就强制刷新，对读缓存是删除访问次数低的一半元素
//对写缓存则是将变化的元素写到B+树中去
#include"BPlusTree.h"
#include<map>
#include<algorithm>
using namespace std;
//readBuffer本质上是一个unordered_map
//其中的node除了记录key和value之外 还记录了被访问的次数
//未达到容量上限时 与正常map一样使用
//达到容量上限时对map按被访问次数进行排序
//删除一半低访问次数的元素

//readBufferNode存着value和访问次数
//声明一个、访问一个就会增加访问次数
class readBufferNode
{
public:
	friend class readBuffer;
	readBufferNode() { value = 0; visited_times == 0; }
	readBufferNode(int _value, int _offPos);	
	readBufferNode(int _value);
	void setValue(int _value,int _offPos);
	void setValue(int _value);
	int value;
	int visited_times = 0;
	int offPos;
};

bool cmp_nodes(const pair<int, readBufferNode> node1, const pair<int, readBufferNode>node2);

class readBuffer
{
public:
	//返回对应的value值，不存在则返回 -1
	int find(int key);
	//刷新缓存区 只删除一半的缓存
	void refresh();
	//设置缓存大小
	void setBufferSize(int _size) { size = _size; }
	readBuffer() { nodes = new map<int, readBufferNode>; size = 5000; }
	~readBuffer() { delete nodes; }
	//如果之前已经有此key存在了 不作任何事情 返回false
	bool add(int key, int value, int offPos);
	bool add(int key, int value);
	//不管key是否存在都会写入
	void modify(int key, int value, int offPos);
	void modify(int key, int value);
	//调用remove前要保证nodes里面有key
	bool remove(int key);
	//找不到返回nullptr 否则返回对应的
	int* search(int key);
	friend class database;
	void clear();
	
private:
	map<int, readBufferNode>* nodes;
	int size;
};

class writeBufferNode
{
public:
	//operation分为"add" "modify" "remove"
	string operataion;
	int offPos;
	int size;
	string new_info;
	int key;
	int value;
};
class writeBuffer
{
public:
	writeBuffer();
	int* search(int key);
	writeBufferNode* search_pro(int key);
	bool modify(int key, int value);
	bool add(int key, int value, string oper, int offpos);
	//此函数无论存不存在都会添加或者修改
	bool add_pro(int key, int value, string oper, int offpos);
	void setBufferSize(int _size){size = _size;}
	int getSize() { return size; }
	void refresh();
	void remove(int key);
	int getTrueSize();
	map<int, writeBufferNode>& getData();
	void clear();


private:
	//TODO待补完
	map<int, writeBufferNode> inner_nodes;
	int size;
};
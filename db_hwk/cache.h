#pragma once
//缓存设计类
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
	readBufferNode() { value = 0; visited_times == 0; }
	readBufferNode(int _value)
	{
		value = _value;
		visited_times++;
	}
	void setValue(int _value)
	{
		value = _value;
		visited_times++;
	}
	int value;
	int visited_times = 0;
};

bool cmp_nodes(const pair<int, readBufferNode> node1, const pair<int, readBufferNode>node2)
{
	return (node1.second.value > node2.second.value);
}

class readBuffer
{
	//返回对应的value值，不存在则返回 -1
	int find(int key);
	//刷新缓存区 只删除一半的缓存
	void refresh();
	//设置缓存大小
	void setBufferSize(int _size) { size = _size; }
	readBuffer()
	{
		size = 5000;
	}
	//调用add前要先保证nodes里面没有要找到key
	void add(int key, int value)
	{
		readBufferNode tmp_node(value);
		nodes[key]=tmp_node;
	}
	//调用remove前要保证nodes里面有key
	void remove(int key)
	{
		nodes.erase(key);
	}
private:
	map<int, readBufferNode> nodes;
	int size;
};

#include "cache.h"

int readBuffer::find(int key)
{
	auto result = (*nodes).find(key);
	if (result == (*nodes).end()) return-1;
	else
	{
		result->second.visited_times++;
		return result->second.value;
	}
}

void readBuffer::refresh()
{
	if ((*nodes).size() < size) return;
	else
	{
		vector<pair<int, readBufferNode>> cmp_container((*nodes).begin(), (*nodes).end());
		sort(cmp_container.begin(), cmp_container.end(), cmp_nodes);
		(*nodes).clear();
		for (int i = 0; i < cmp_container.size() / 2; i++)
		{
			(*nodes).insert(cmp_container[i]);
		}
	}
}

bool readBuffer::add(int key, int value, int offPos)
{
		refresh();
		if (find(key) != -1) return false;
		modify(key,value, offPos);
		return true;	
}

bool readBuffer::add(int key, int value)
{
	return add(key, value, -1);
}

void readBuffer::modify(int key, int value, int offPos)
{
	//refresh();
	readBufferNode tmp_node(value,offPos);	
	tmp_node.visited_times++;
	(*nodes)[key] = tmp_node;
}

void readBuffer::modify(int key, int value)
{
	modify(key, value, -1);
}

bool readBuffer::remove(int key)
{
		if (find(key) == -1) return false;
		(*nodes).erase(key);
		return true;
}

int * readBuffer::search(int key)
{
	auto iter = (*nodes).find(key);
	if (iter == (*nodes).end())
		return nullptr;
	else
		return &((*iter).second.value);
}

void readBuffer::clear()
{
	nodes->clear();
}

bool cmp_nodes(const pair<int, readBufferNode> node1, const pair<int, readBufferNode> node2)
{
		return (node1.second.value > node2.second.value);
}

writeBuffer::writeBuffer()
{
	size = 5000;
}

int* writeBuffer::search(int key)
{
	auto iter = inner_nodes.find(key);
	if (iter == inner_nodes.end())
	{
		return nullptr;
	}
	else
	{
		return &((*iter).second.value);
	}
}

writeBufferNode* writeBuffer::search_pro(int key)
{
	auto iter = inner_nodes.find(key);
	if (iter == inner_nodes.end())
	{
		return nullptr;
	}
	else
	{
		return &((*iter).second);
	}
}

bool writeBuffer::modify(int key, int value)
{
	auto ptr = search(key);
	if(ptr==nullptr) return false;
	(*ptr) = value;
}

bool writeBuffer::add(int key, int value, string oper, int offpos)
{
	if (search(key) != nullptr) return false;
	writeBufferNode tmp_node;
	tmp_node.key = key;
	tmp_node.value = value;
	tmp_node.operataion = oper;
	tmp_node.offPos = offpos;
	string tmp_key = to_string(key);
	string tmp_value = to_string(value);
	if (tmp_key.length() > 9 || tmp_value.length() > 9)
	{
		throw outOfRange();
	}
	while (tmp_key.length() != 9)tmp_key = "0" + tmp_key;
	while (tmp_value.length() != 9) tmp_value = "0" + tmp_value;	
	tmp_node.new_info = tmp_key+ " " + tmp_value + " ";
	tmp_node.size = (tmp_node.new_info).size();
	if (tmp_node.size != 20) throw cacheLengthError();
	inner_nodes[key] = tmp_node;
}

bool writeBuffer::add_pro(int key, int value, string oper, int offpos)
{
	writeBufferNode tmp_node;
	tmp_node.key = key;
	tmp_node.value = value;
	tmp_node.operataion = oper;
	tmp_node.offPos = offpos;
	string tmp_key = to_string(key);
	string tmp_value = to_string(value);
	if (tmp_key.length() > 9 || tmp_value.length() > 9)
	{
		throw outOfRange();
	}
	while (tmp_key.length() != 9)tmp_key = "0" + tmp_key;
	while (tmp_value.length() != 9) tmp_value = "0" + tmp_value;
	tmp_node.new_info = tmp_key + " " + tmp_value + " ";
	tmp_node.size = (tmp_node.new_info).size();
	if (tmp_node.size != 20) throw cacheLengthError();
	inner_nodes[key] = tmp_node;
}


void writeBuffer::refresh()
{
	if (inner_nodes.size()<5000) return;
	else
	{
	}
	//TODO
	//否则刷新一半内容进去
}

void writeBuffer::remove(int key)
{
	inner_nodes.erase(key);
}

int writeBuffer::getTrueSize()
{
	return inner_nodes.size();
}

map<int, writeBufferNode>& writeBuffer::getData()
{
	// TODO: 在此处插入 return 语句
	return inner_nodes;
}

void writeBuffer::clear()
{
	inner_nodes.clear();
}

readBufferNode::readBufferNode(int _value, int _offPos)
{
		value = _value;
		offPos = _offPos;
		visited_times++;	
}

readBufferNode::readBufferNode(int _value)
{
	value = _value;	
	visited_times++;
}

void readBufferNode::setValue(int _value, int _offPos)
{
	offPos = _offPos;
	value = _value;
	visited_times++;
}

void readBufferNode::setValue(int _value)
{
	value = _value;
	visited_times++;
}

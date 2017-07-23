#include "cache.h"

int readBuffer::find(int key)
{
	auto result = (*nodes).find(key);
	if (result == (*nodes).end()) return-1;
	else return result->second.value;
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

bool readBuffer::add(int key, int value)
{
		if (find(key) != -1) return false;
		modify(key,value);
		return true;	
}

void readBuffer::modify(int key, int value)
{
	refresh();
	readBufferNode tmp_node(value);
	(*nodes)[key] = tmp_node;
}

bool readBuffer::remove(int key)
{
		if (find(key) == -1) return false;
		(*nodes).erase(key);
		size--;
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

bool cmp_nodes(const pair<int, readBufferNode> node1, const pair<int, readBufferNode> node2)
{
		return (node1.second.value > node2.second.value);
}

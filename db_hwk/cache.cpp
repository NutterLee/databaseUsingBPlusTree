#include "cache.h"

int readBuffer::find(int key)
{
	auto result = nodes.find(key);
	if (result == nodes.end()) return-1;
	else return result->second.value;
}

void readBuffer::refresh()
{
	if (nodes.size() < size) return;
	else
	{
		vector<pair<int, readBufferNode>> cmp_container(nodes.begin(),nodes.end());
		sort(cmp_container.begin(), cmp_container.end(), cmp_nodes);
		nodes.clear();
		for (int i = 0; i < cmp_container.size() / 2; i++)
		{
			nodes.insert(cmp_container[i]);
		}
	}
}

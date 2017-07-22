#pragma once
//���������
#include"BPlusTree.h"
#include<map>
#include<algorithm>
using namespace std;

//readBuffer��������һ��unordered_map
//���е�node���˼�¼key��value֮�� ����¼�˱����ʵĴ���
//δ�ﵽ��������ʱ ������mapһ��ʹ��
//�ﵽ��������ʱ��map�������ʴ�����������
//ɾ��һ��ͷ��ʴ�����Ԫ��

//readBufferNode����value�ͷ��ʴ���
//����һ��������һ���ͻ����ӷ��ʴ���
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
	//���ض�Ӧ��valueֵ���������򷵻� -1
	int find(int key);
	//ˢ�»����� ֻɾ��һ��Ļ���
	void refresh();
	//���û����С
	void setBufferSize(int _size) { size = _size; }
	readBuffer()
	{
		size = 5000;
	}
	//����addǰҪ�ȱ�֤nodes����û��Ҫ�ҵ�key
	void add(int key, int value)
	{
		readBufferNode tmp_node(value);
		nodes[key]=tmp_node;
	}
	//����removeǰҪ��֤nodes������key
	void remove(int key)
	{
		nodes.erase(key);
	}
private:
	map<int, readBufferNode> nodes;
	int size;
};

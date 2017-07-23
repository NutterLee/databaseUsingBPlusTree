#pragma once
//���������
//���������˼·��
//�̶���С�����ȶ�ȡ
//�����ض������⣬Ҫǿ��ˢ�£����඼���Լ�ά����������
//ÿ�δﵽ��������ʱ��ǿ��ˢ�£��Զ�������ɾ�����ʴ����͵�һ��Ԫ��
//��д�������ǽ��仯��Ԫ��д��B+����ȥ
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
	friend class readBuffer;
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

bool cmp_nodes(const pair<int, readBufferNode> node1, const pair<int, readBufferNode>node2);


class readBuffer
{
public:
	//���ض�Ӧ��valueֵ���������򷵻� -1
	int find(int key);
	//ˢ�»����� ֻɾ��һ��Ļ���
	void refresh();
	//���û����С
	void setBufferSize(int _size) { size = _size; }
	readBuffer() { nodes = new map<int, readBufferNode>; size = 5000; }
	~readBuffer() { delete nodes; }
	//���֮ǰ�Ѿ��д�key������ �����κ����� ����false
	bool add(int key, int value);	
	void modify(int key, int value);
	//����removeǰҪ��֤nodes������key
	bool remove(int key);
	//�Ҳ�������nullptr ���򷵻ض�Ӧ��
	int* search(int key);

private:
	map<int, readBufferNode>* nodes;
	int size;
};

class writeBuffer
{
public:

private:
	//operation��Ϊ"add"
	string operataion;
	//TODO������

};
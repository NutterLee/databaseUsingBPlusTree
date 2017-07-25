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
	//���ض�Ӧ��valueֵ���������򷵻� -1
	int find(int key);
	//ˢ�»����� ֻɾ��һ��Ļ���
	void refresh();
	//���û����С
	void setBufferSize(int _size) { size = _size; }
	readBuffer() { nodes = new map<int, readBufferNode>; size = 5000; }
	~readBuffer() { delete nodes; }
	//���֮ǰ�Ѿ��д�key������ �����κ����� ����false
	bool add(int key, int value, int offPos);
	bool add(int key, int value);
	//����key�Ƿ���ڶ���д��
	void modify(int key, int value, int offPos);
	void modify(int key, int value);
	//����removeǰҪ��֤nodes������key
	bool remove(int key);
	//�Ҳ�������nullptr ���򷵻ض�Ӧ��
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
	//operation��Ϊ"add" "modify" "remove"
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
	//�˺������۴治���ڶ�����ӻ����޸�
	bool add_pro(int key, int value, string oper, int offpos);
	void setBufferSize(int _size){size = _size;}
	int getSize() { return size; }
	void refresh();
	void remove(int key);
	int getTrueSize();
	map<int, writeBufferNode>& getData();
	void clear();


private:
	//TODO������
	map<int, writeBufferNode> inner_nodes;
	int size;
};
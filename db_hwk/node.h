#pragma once
#include<vector>
#include"error.h"
#include<string>
#include<utility>
#include<memory>
#include<iostream>
#include<cassert>
//node.h定义了B+树的底层的node
//B+树的node分为二种，leafNode，interiorNode
//分两个文件，一个index一个data
//leafnode不要存数据，存数据长度+偏移量
//三种node都由纯虚基类node继承\
//key 和value 都支持int int和string string
using namespace std;

class interiorNode
{
	friend class BPlusTree;
	//void saveToFile(BPlusTree & tree);
public:
	interiorNode();
	virtual string getType() { return "interiorNode"; }
	//要求 返回不大于key的下标 找不到会返回最后一个可用下标,注意边界情况！！！
	virtual int search(int key);
	//找到则返回下标 找不到就返回-1
	virtual int find(int key);
	//返回可能存在目标节点的下个interiorNode，如果本身已经是leaf了那就返回nullptr
	virtual interiorNode* getNextNode(int key);
	//返回对leaf节点来说返回下一个leaf节点，对于非leaf节点则报错
	virtual interiorNode* getNextLeaf() { throw nodeTypeError(); }
	//设定leaf节点的nextLeaf的值 如果是interiorNode调用这个报错
	virtual void setNextLeaf(interiorNode* node) { throw nodeTypeError(); }
	//合并this_node和sibling，效果是在前面的一个会写入后一个node的所有信息，并获得其所有children的parent地位
	//要合并左节点 应如此调用 mergeWithSibling( leftSibling, this_node); 返回leftSibling
	//要合并右节点 mergeWitgSibling(this_node,rightSibling);返回this_node
	//注意 只写入信息+调整true_num+调整children的parent信息，其余要额外弄
	virtual interiorNode* mergeWithLeftSibling(interiorNode* leftSibling, interiorNode* this_node);
	virtual interiorNode* mergeWithRightSibling(interiorNode* this_node, interiorNode* rightSibling);

	virtual ~interiorNode();
	//为了能让interiorNode的指针访问leafNode内部的数据而设计的函数
	virtual vector<pair<int, int>>& getData() 
	{
		throw dataError();
	}
	//为了能让leafNode的interiorNode指针访问到interiorNode内部的children而设定的函数
	virtual vector<interiorNode*>& getChildren()
	{
		return children;
	}
	//在非叶节点返回阶数限制，在叶节点返回items数目限制
	virtual int getLimitNum() { return num_of_keys; }
	virtual int getTrueNum() { return true_num_of_keys; }
	virtual void setTrueNum(int num) { true_num_of_keys = num; }
	//在interiorNode里的行为是在key的vector中找到位置插入key以保证顺序
	//在leafNode中的行为应该是在存了pair的vector中找到位置插入pair对以保持顺序
	//返回的值是insert以后新的key所在的下标
	virtual int insert(int key, int value);
	//删除非叶节点中的key，应该在underflow中调用？（or删除以后逐层删除存的相应key？注意某一层找不到就还是要找到最上面为止）
	//返回的是key在原来的node中存在的下标
	//如果不存在应该返回-1
	//对非叶子节点remove只删除了key！！！ 还要再调用其之后调整children
	virtual int remove(int key);
	virtual interiorNode* getParent() { return parent; }
	virtual void setParent(interiorNode* node) { parent = node; };
	//得到当前node的左兄弟，左兄弟不存在返回nullptr
	virtual interiorNode* getLeftSibling(interiorNode* this_node);
	virtual interiorNode* getRightSibling(interiorNode* this_node);
	static int num_of_keys ;
	static void setM(int num) ; 
	vector<int>& getKeys() { return keys; }
	string id;
	virtual string getID() { return id; }
	virtual void setID(string num) { id = num; }
	virtual interiorNode* getNextParent(interiorNode* this_node);
	friend void saveToFile(BPlusTree & tree);
	friend void readFromFile(BPlusTree& tree, string filename);


private:
	int true_num_of_keys;
	vector<int>keys;
	vector<interiorNode*> children;
	interiorNode* parent;
};

class leafNode : public interiorNode
{
public:
	leafNode();	
	//要求 返回不大于key的下标 找不到会返回最后一个可用下标
	int search(int key)override;
	//找到就返回下标，否则返回-1
	int find(int key) override;
	string getType() override { return "leafNode"; }
	vector<pair<int, int>>& getData() { return items; }
	static int num_of_items;
	static void setL(int num);
	virtual int getLimitNum() { return num_of_items; }
	virtual int getTrueNum() { return true_num_of_items; }
	virtual void setTrueNum(int num) { true_num_of_items = num; }
	virtual int insert(int key, int value) override;
	//对叶子节点的remove是删除了键值对的
	virtual int remove(int key) override;
	//返回对leaf节点来说返回下一个leaf节点，对于非leaf节点则返回nullptr
	virtual interiorNode* getNextLeaf()override { return next_leaf; }
	//设定leaf节点的nextLeaf的值 如果是interiorNode调用这个报错
	virtual void setNextLeaf(interiorNode* node) { next_leaf=node; }
	virtual void setParent(interiorNode* node) { parent = node; };
	virtual interiorNode* getParent() { return parent; }
	virtual interiorNode* getLeftSibling(interiorNode* this_node) override;
	virtual interiorNode* getRightSibling(interiorNode* this_node) override;
	//为了能让leafNode的interiorNode指针访问到interiorNode内部的children而设定的函数
	//对leafNode来说返回的是父节点的children
	virtual vector<interiorNode*>& getChildren()
	{
		return (this->getParent())->getChildren();
	}
	virtual interiorNode* mergeWithLeftSibling(interiorNode* leftSibling, interiorNode* this_node);
	virtual interiorNode* mergeWithRightSibling(interiorNode* this_node, interiorNode* rightSibling);
	string id;
	virtual string getID() { return id; }
	virtual void setID(string num) { id = num; }
	friend void saveToFile(BPlusTree & tree);
	friend void readFromFile(BPlusTree& tree, string filename);

private:
	int true_num_of_items;
	vector<pair<int, int>>items;
	interiorNode* parent;
	interiorNode* next_leaf;
};
#include "file.h"


//序列化树
//广度优先
//约定 1. 以换行表示一个node的结束
//2. 每一项之间以空格隔开
//3. 存储顺序
//1）type
//2) 当前层数
//2) 父母ID
//3）自己ID
//4) keys的数目
//5）keys的值+ children的ID(暂时去掉children的id) （对leafNode来说应该是键值对）

void saveToFile(BPlusTree & tree)
{
	if (tree.size== 0) return;
	unordered_set<string> first_nodes_id;
	//先将tree的每一层第一个node的地址转换成的id保存起来
	interiorNode* record_node = tree.getRoot();
	while (record_node != nullptr)
	{
		string tmp_id = ptrToString(record_node);
		first_nodes_id.insert(tmp_id);
		if (record_node->children.size() != 0)
		{
			record_node = record_node->getChildren()[0];
		}
		else record_node = nullptr;
	}
	fstream output("index.dat", ios_base::out);
	interiorNode* this_node = tree.getRoot();

	output << tree.getM() << " " << tree.getSize() << " " 
		<<(tree.getRoot()) <<endl;
	queue<interiorNode*>flows;
	flows.push(this_node);
	int depth = -1;
	while (!flows.empty())
	{
		this_node = flows.front();
		flows.pop();
		if (first_nodes_id.find(ptrToString(this_node)) != first_nodes_id.end())
		{
			depth++;
		}
		if (this_node->getType() != "leafNode")
		{
			//interiorNode写入文件
			output << this_node->getType() << " ";
			output << depth << " ";
			output << this_node->getParent() << " ";
			output << this_node << " ";
			output << this_node->getTrueNum() << " ";
			for (auto iter = this_node->getKeys().begin(); iter != this_node->getKeys().end(); iter++)
			{
				output << *iter << " ";
			}
		/*	for (auto iter = this_node->getChildren().begin(); iter != this_node->getChildren().end(); iter++)
			{
				output << *iter << " ";
			}*/
			output << endl;
			for (auto iter = this_node->getChildren().begin(); iter != this_node->getChildren().end(); iter++)
			{
				flows.push(*iter);
			}
		}
		else
		{
			//leafNode 写入文件
			output << this_node->getType() << " ";
			output << depth << " ";
			output << this_node->getParent() << " ";
			output << this_node << " ";
			output << this_node->getTrueNum() << " ";
			for (auto iter = this_node->getData().begin(); iter != this_node->getData().end(); iter++)
			{
				output << (*iter).first << " " << (*iter).second << " ";
			}
			output << endl;
		}
	}
	output.close();
}

void readFromFile(BPlusTree& tree,string filename)
{
	//从index文档中读取
	fstream input(filename, ios_base::in);
	if(!input.is_open()) 
	{
		cout << "unable to read data file" << endl;
		return;
	}
	int tree_size;
	int tree_m;
	string tree_rootID;
	input >> tree_m >> tree_size>>tree_rootID;
	tree.setM(tree_m);
	tree.setSize(tree_size);
	tree.setRootID(tree_rootID);
	string line;
	interiorNode* last_node = nullptr;
	int depth_record = 0;
	int test_i = 0;
	while (getline(input,line))
	{
		test_i++;
	//	cout << test_i << endl;
		if (line == "")continue;
		stringstream tmp_flow;
		tmp_flow << line;
		string node_type;
		string parent_id;
		string id;
		int keys_size;
		int depth;
		string children_id;
		interiorNode* this_node;
		tmp_flow >> node_type >>depth>> parent_id >> id >> keys_size;		
		if (node_type != "leafNode")
		{
			this_node = new interiorNode();
			//last_node记录的是上一个节点的信息 
			//如果与上一节点父母ID相同，可以判断是同一个parent的孩子
			//否则调用getNextParent 寻找到最右边的上一层的父节点
			//先写入这个node的基本信息
			this_node->setID(id);
			for (int i = 0; i < keys_size; i++)
			{
				int key;
				tmp_flow >> key;
				this_node->getKeys().push_back(key);				
			}
			this_node->setTrueNum(this_node->getKeys().size());
			//case1 树完全是空的
			if (last_node == nullptr)
			{
				tree.root = this_node;
				last_node = this_node;
			}
			else
			{
				//除了根节点以外的第一层的链接
				if(last_node->getID()==parent_id)
				{ 
					last_node->getChildren().push_back(this_node);
					this_node->setParent(last_node);
					this_node->parent = last_node;
					last_node = this_node;
				}
				else
				{	//一般情况 先看和last_node的parent的id是否相同
					//相同则插入到last_node的parent的children中，并修改自身的parent记录
					//不同则试着取nextParent为parent
					//nextParent为0就取tree的getFirst为parent
					if (last_node->getParent()->getID() == parent_id)
					{
						last_node->getParent()->getChildren().push_back(this_node);
						this_node->setParent(last_node->getParent());
						this_node->parent = last_node->getParent();
						last_node = this_node;
					}
					else
					{
						interiorNode* new_parent = last_node->getNextParent(last_node);
						if (new_parent == nullptr) new_parent = tree.getFloorFirstNode();
						new_parent->getChildren().push_back(this_node);
						this_node->setParent(new_parent);
						this_node->parent = new_parent;
						last_node = this_node;
					}
				}
			}
		}
		else
		{
			this_node = new leafNode();
			this_node->setID(id);
			for (int i = 0; i < keys_size; i++)
			{
				int key;
				int value;
				tmp_flow >> key >> value;
				this_node->getData().push_back({ key,value });				
			}
			this_node->setTrueNum(this_node->getData().size());
			if (last_node == nullptr)
			{
				tree.root = this_node;
				last_node = this_node;
			}
			else
			{
				if (last_node->getID() == parent_id)
				{
					last_node->children.push_back(this_node);
					this_node->setParent(last_node);
					this_node->parent = last_node;
					last_node = this_node;
				}
				else if (last_node->getParent()->getID() == parent_id)
				{
					(last_node->getParent())->getChildren().push_back(this_node);
					this_node->setParent(last_node->getParent());
					this_node->parent = last_node->getParent();
					if (last_node->getType() == "leafNode") last_node->setNextLeaf(this_node);
					last_node = this_node;
				}
				else
					//对于叶子节点来说尝试拿nextParent就好了 不会用到tree的getfirst
					//整个树写完之后可以重新设定head
				{
					interiorNode* next_parent = (last_node->getNextParent(last_node));
					if (next_parent == nullptr) next_parent = tree.getFloorFirstNode();
					if (next_parent->getType() == "leafNode") return;
					else
					{
						//test only code
						if (next_parent->getID() != parent_id)
						{
							cout << "something wrong!!!" << test_i << endl;
							system("pause");
						}
						next_parent->getChildren().push_back(this_node);
						this_node->setParent(next_parent);
						this_node->parent = next_parent;
						last_node = this_node;
					}
				}
			}
		}	
	//	input.ignore();
	}
	tree.setHead();
}

string ptrToString(interiorNode * node)
{
	if (node == nullptr) return "00000000";
	stringstream tmp_flow;
	tmp_flow << node;
	string tmp_string;
	tmp_flow >> tmp_string;
	return tmp_string;
}

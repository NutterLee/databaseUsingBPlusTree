#include"BPlusTree.h"

BPlusTree::BPlusTree()
{
	M = 128;
	size = 0;
	root =new leafNode();
	head = nullptr;
}

pair<int,int>* BPlusTree::search(int key)
{
	interiorNode* this_node =moveToLeaf(key);	
	//此时this_node是可能存在目标key的大节点
//	assert(this_node->getType() != "leafNode");
	int pos = this_node->search(key);
	if ((this_node->getData()).size() <=pos) return nullptr;
	if ((this_node->getData())[pos].first == key) return &(this_node->getData())[pos];
	else	 return nullptr	;
}

bool BPlusTree::insert(int key, int value)
{
	if (root == nullptr) root = new leafNode();
	auto pos = search(key);
	if(pos!=nullptr)	return false;
	interiorNode* this_node = moveToLeaf(key);
	//TODO 如何插入/?????
	//TODO 连接各个节点呢？？ test done
	this_node->insert(key, value);
	solveOverflow(this_node);
	size++;
	setHead();
	return true;
}

bool BPlusTree::remove(int key)
{
	interiorNode* this_node = moveToLeaf(key);
	if (this_node == nullptr) return false;
	int remove_pos=this_node->remove(key);
	size--;
	if (this_node == root) return true;
	//case 1 : 删除节点后节点的true_num大于要求的一半
	if (this_node->getTrueNum() >= (this_node->getLimitNum() / 2))
	{
		//updateParent(this_node->parent, key);
		//调整父索引
		//1. 只有在原来children中第一位的 才有可能出现在之前的索引中
		if(remove_pos!=0)return true;
		else
		{
			//如果key在parent的children中不是最小的且又找不到，可以返回了
			int parent_remove_pos = (this_node->getParent())->find(key);
			if (parent_remove_pos == -1 && (key > (this_node)->getParent()->keys[0])) return true;
			//这是在父节点中命中的
			else if (parent_remove_pos != -1)
			{
				//将原node中被删除元素的下一个key复制上来
				//this_node 肯定是leafNode所以才这么做
				//if(remove_pos<this_node->getTrueNum()) 原node如果只有一个的话 应该已经被underflow捕获了
				(this_node)->getParent()->keys[parent_remove_pos] = this_node->getData()[remove_pos].first;
				return true;
			}
			//这是在父节点以上的位置命中的
			else
			{
				interiorNode* next_node = this_node->getParent();
				while (next_node != nullptr)
				{
					int next_remove_pos = next_node->find(key);
					//仍然没找到，向上一层
					if (next_remove_pos == -1)
					{
						next_node = next_node->getParent();
						continue;
					}
					else
					{
						//{//将叶子节点中删除的key的下一个顶替上去
							(next_node)->keys[next_remove_pos] = this_node->getData()[remove_pos].first;
							return true;
					}
				}
				return true;
			}
		}
	}
	else
	{
		if (remove_pos == 0)
		{
			//如果key在parent的children中不是最小的且又找不到，可以返回了
			int parent_remove_pos = (this_node->getParent())->find(key);
			if (parent_remove_pos == -1 && (key > (this_node)->getParent()->keys[0]))
			{
				solveUnderflow(this_node, key);
				return true;
			}
			//这是在父节点中命中的
			else if (parent_remove_pos != -1)
			{
				//将原node中被删除元素的下一个key复制上来
				//this_node 肯定是leafNode所以才这么做
				//if(remove_pos<this_node->getTrueNum()) 原node如果只有一个的话 应该已经被underflow捕获了
				(this_node)->getParent()->keys[parent_remove_pos] = this_node->getData()[remove_pos].first;
				solveUnderflow(this_node, key);
				return true;
			}
			//这是在父节点以上的位置命中的
			else
			{
				interiorNode* next_node = this_node->getParent();
				while (next_node != nullptr)
				{
					int next_remove_pos = next_node->find(key);
					//仍然没找到，向上一层
					if (next_remove_pos == -1)
					{
						next_node = next_node->getParent();
						continue;
					}
					else
					{
						//{//将叶子节点中删除的key的下一个顶替上去
						(next_node)->keys[next_remove_pos] = this_node->getData()[remove_pos].first;
						solveUnderflow(this_node, key);
						return true;
					}
				}
				solveUnderflow(this_node, key);
				return true;
			}
		}
		else
		{
			solveUnderflow(this_node, key);
			return true;
		}
	}
}

void BPlusTree::setHead()
{
	interiorNode* this_node = root;
	interiorNode* record = root;
	while ((this_node->children).size() != 0) 
	{
		//test only code
		record = this_node;
		this_node = *(this_node->children.begin());
	}
	head = this_node;;
}

//暂时废除 以后再考虑封装
void BPlusTree::updateParent(interiorNode * this_node, int key)
{
	//说明： 递归调用 初始的this_node应该是删除了key的leafNode
	if (this_node == nullptr) return;
	
	int remove_pos = this_node->find(key);
	//z在此node中找到了key
	if (remove_pos != -1)
	{

	}
	auto iter = (this_node->keys).begin()+remove_pos;
	(this_node->keys).erase(iter);
	this_node->setTrueNum(this_node->keys.size());
	//TODO 待完成

}

interiorNode * BPlusTree::moveToLeaf(int key)
{
	interiorNode* this_node = root;
	if (this_node == nullptr)return nullptr;
	//先移动到叶子节点
	interiorNode* next_node = this_node->getNextNode(key);
	while (next_node != nullptr)
	{
		this_node = next_node;
		next_node = this_node->getNextNode(key);
	}
	return this_node;
}

void BPlusTree::traverse(int num)
{
	interiorNode* this_node = getHead();
	if (this_node == nullptr) return;
	for (int i = 0; i < num; i++)
	{
		for (auto iter =( this_node->getData()).begin(); iter != (this_node->getData()).end(); iter++)
		{
			cout << "( " << (*iter).first << " , " << (*iter).second << " ) " << endl;
		}
		this_node = this_node->getNextLeaf();
		if (this_node == nullptr)break;
	}
}

void BPlusTree::solveOverflow(interiorNode * this_node)
{
	if (this_node == nullptr || this_node->getLimitNum() >= this_node->getTrueNum())return;
	else
	{
		int split_pos =( this_node->getLimitNum() +1)/ 2;
		//分root节点和不是root节点
		if (this_node->parent != nullptr)
		{
			//不是root节点
			int insert_pos = (this_node->parent)->insert((this_node->getData())[split_pos].first, 0);
			interiorNode* new_node = new leafNode();
			//assert(new_node->getType() != "leafNode" || this_node->getType() != "leafNode");
			for (int i = split_pos; i < this_node->getData().size(); i++)
			{
				new_node->getData().push_back(this_node->getData()[i]);
			}
			//在parent的children里面插入new_node
			auto iter = (this_node->parent->children).begin()+insert_pos+1;
			//for (int i = 0; i <=insert_pos; i++) iter++;
			if (iter == (this_node->parent->children).end())
				(this_node->parent->children).push_back(new_node);
			else (this_node->parent->children).insert(iter,new_node);
			this_node->parent->setTrueNum(this_node->parent->keys.size());
			this_node->getData().resize(split_pos);
			//new_node的parent的设置
			new_node->setParent(this_node->parent);
			new_node->parent = this_node->parent;
			//重新设置true_num的值
			this_node->setTrueNum(this_node->getData().size());
			new_node->setTrueNum(new_node->getData().size());
			
			//设置好叶子之间的连接
			new_node->setNextLeaf(this_node->getNextLeaf());
			this_node->setNextLeaf(new_node);
		}
		else
		{
			//是root节点时
			auto split_node = new interiorNode();
			interiorNode* new_node = new leafNode();
			split_node->keys.push_back( ((this_node->getData())[split_pos]).first);
			split_node->children.push_back(this_node);
			split_node->children.push_back(new_node);
			root = split_node;
			//assert(this_node->getType() != "leafNode" || new_node->getType() != "leafNode");
			for (int i = split_pos; i < this_node->getData().size(); i++)
			{
				(new_node->getData()).push_back((this_node->getData())[i]);
			}
			this_node->setParent(split_node);
			new_node->setParent(split_node);
			this_node->parent = split_node;
			new_node->parent = split_node;
			this_node->getData().resize(split_pos);
			this_node->setTrueNum( this_node->getData().size());
			new_node->setTrueNum(new_node->getData().size());
			this_node->parent->setTrueNum(this_node->parent->keys.size());
			//设置好叶子之间的连接
			new_node->setNextLeaf(this_node->getNextLeaf());
			this_node->setNextLeaf(new_node);
		}
		solveInnerOverflow(this_node->parent);
	}
}

void BPlusTree::solveInnerOverflow(interiorNode * this_node)
{
	if (this_node==nullptr||this_node->getLimitNum() >= this_node->getTrueNum()) return;
	else
	{
		int split_pos =(this_node->getLimitNum()+1)/2;
		//不是root节点时
		if (this_node->parent != nullptr)
		{
			//不是root而是一般的interiorNode时，往parent的keys里面加入分割点的key
			//step 1 将split_node这个key写到parent的key里面
			int insert_pos=(this_node->parent)->insert((this_node->keys)[split_pos], 0);
			//step2 新建一个node，写入需要的keys和children
			auto new_node = new interiorNode();
			auto key_iter = (this_node->keys).begin()+split_pos+1;
			auto children_iter = (this_node->children).begin()+split_pos+1;
			auto parent_iter = ((this_node->parent)->children).begin()+insert_pos+1;
			//TODO 改一下这个方法 还有越界和迭代器失效 问题要注意		 test done
			//将原node中split点以后的全都写入新的node中
			for (auto iter = key_iter; iter != (this_node->keys).end(); iter++)
			{
				new_node->keys.push_back(*iter);
			}
			for (auto iter = children_iter; iter != (this_node->children).end(); iter++)
			{
				(*iter)->setParent (new_node);
				(*iter)->parent = new_node;
				new_node->children.push_back(*iter);
			}
			//step3 将new_node写入到children里面
			//指向待插入的children位置			
			((this_node->parent)->children).insert(parent_iter, new_node);
			//step4 砍掉原来node的多余部分
			//TODO 看看这个resize的大小有没有问题 test done
				(this_node->keys).resize(this_node->keys.size()/2);
				(this_node->children).resize((this_node->children.size() +1)/ 2);
		
			//step5： 修正一下parent节点
			//改一下父母节点
			new_node->setParent(this_node->parent);
			new_node->parent = this_node->parent;
			//this_node->parent = this_node->parent->parent;
			//step6： 修正true_num_of_keys
			this_node->setTrueNum(this_node->keys.size());
			new_node->setTrueNum( new_node->keys.size());
			(this_node->parent)->setTrueNum( this_node->parent->keys.size());	
			//step7：递归看看父节点是不是也有问题
			solveInnerOverflow(this_node->parent);
		}
		//root也要overflow的情况
		else
		{
			//step1 新建未来的root
			auto split_node = new interiorNode();
			split_node->keys.push_back((this_node->keys)[split_pos]);
			//step2 新建分离后的node
			auto new_node = new interiorNode();
			auto key_iter = (this_node->keys).begin()+split_pos+1;
			auto children_iter = (this_node->children).begin()+split_pos+1;
			//将原node中split点以后的全都写入新的node中
			for (auto iter = key_iter; iter != (this_node->keys).end(); iter++)
			{
				new_node->keys.push_back(*iter);
			}
			for (auto iter = children_iter; iter != (this_node->children).end(); iter++)
			{
				(*iter)->setParent(new_node);
				(*iter)->parent = new_node;
				new_node->children.push_back(*iter);
			}
			//Step3 删除原node多余的内容
			if (split_pos * 2 == this_node->getLimitNum())
			{
				(this_node->keys).resize(this_node->keys.size() / 2);
				(this_node->children).resize(this_node->children.size() / 2);
			}
			else
			{
				(this_node->keys).resize(this_node->keys.size() - split_pos);
				(this_node->children).resize(this_node->children.size() / 2 + 1);
			}
			//step 4 修改children和parent各种关系
			split_node->children.push_back(this_node);
			split_node->children.push_back(new_node);
			this_node->setParent(split_node);
			new_node->setParent(split_node);
			this_node->parent = split_node;
			new_node->parent = split_node;
			root=split_node;
			//step5： 修正true_num_of_keys
			this_node->setTrueNum(this_node->keys.size());
			new_node->setTrueNum( new_node->keys.size());
			//root没有parent(this_node->parent)->setTrueNum(this_node->parent->keys.size());
			//step6： 已经修改到了最末端了 可以结束递归了
			return;
		}		
	}
}

void BPlusTree::solveUnderflow(interiorNode * this_node, int key)
{
	//以下解决的是叶子节点的underflow
	if (this_node->getTrueNum() >= this_node->getLimitNum() / 2) return;
	//case 1 p是根节点
	if (this_node == root)
	{
		if (this_node->getTrueNum() == 0) root = nullptr;
		return;
	}
	//不是根节点
	//step1 看看左邻居的情况
	auto leftSibling = this_node->getLeftSibling(this_node);
	auto rightSibling = this_node->getRightSibling(this_node);
	int min_limit = this_node->getLimitNum() / 2;
	int max_limit = this_node->getLimitNum();
	if (leftSibling != nullptr&&leftSibling->getTrueNum() > min_limit)
	{
		//向左借孩子
		//step1: 将左兄弟的最后一位拷贝过来，并放到this_node的最前面，删除左兄弟最后一个
		(this_node->getData()).insert(this_node->getData().begin(), (leftSibling->getData())[leftSibling->getData().size() - 1]);
		leftSibling->getData().pop_back();
		leftSibling->setTrueNum(leftSibling->getData().size());
		this_node->setTrueNum(this_node->getData().size());
		int adapt_pos = (this_node->getParent())->search(this_node->getData().back().first);
		this_node->getParent()->keys[adapt_pos] = this_node->getData()[0].first;
		//更新true_num的值
		//更新父节点索引
	}
	else if (rightSibling != nullptr&&rightSibling->getTrueNum() > rightSibling->getLimitNum() / 2)
	{
		//向右借孩子
		//step 拷贝右兄弟的第一位 加到自己后面 并从右兄弟里面删除
		//将parent对应的索引改为右兄弟删了以后的第一位
		(this_node->getData()).insert(this_node->getData().end(), rightSibling->getData()[0]);
		rightSibling->getData().erase(rightSibling->getData().begin());
		rightSibling->setTrueNum(rightSibling->getData().size());
		this_node->setTrueNum(this_node->getData().size());
		int adapt_pos = (this_node->getParent())->search(rightSibling->getData().back().first)-1;
		this_node->getParent()->keys[adapt_pos] = rightSibling->getData()[0].first;
		//更新true_num的值
		//更新父节点索引
	}
	else
	{
		if (leftSibling != nullptr && (this_node->getTrueNum() + leftSibling->getTrueNum()) <= this_node->getLimitNum())
		{
			//step1 拷贝信息
			this_node->mergeWithLeftSibling(leftSibling, this_node);
			//step2 更新leaf之间连接的信息
			leftSibling->setNextLeaf( this_node->getNextLeaf());
			//从parent的children中删去this_node
			int adapt_pos = (this_node->getParent())->search(this_node->getData().back().first);
			if ((this_node->getParent())->find(this_node->getData().back().first) != -1) adapt_pos++;
			auto adapt_iter =( (this_node->getParent())->children).begin()+adapt_pos;
			((this_node->getParent())->children).erase(adapt_iter);
			//更新索引和parent的true_num TODO
			//只有this_node的第一个可能出现在父辈的keys之中
			int remove_pos = this_node->getParent()->remove(this_node->getData()[0].first);
			//如果返回的不是-1  说明parent中有这个key且已经删掉了 那就不可能再在父辈之上存在了，调整一下parent的true_num即可
			//由于是左孩子合并那说明父辈之上必然不会有this_node中的key了 《=TODO check！
			if (remove_pos != -1)
			{
				this_node->getParent()->setTrueNum(this_node->getParent()->keys.size());
			}
			//合并this_node和左孩子
		}
		else if(rightSibling!=nullptr&&(this_node->getTrueNum()+rightSibling->getTrueNum())<= this_node->getLimitNum())
		{
			//合并this_node和右孩子
			this_node->mergeWithRightSibling(this_node, rightSibling);
			this_node->setNextLeaf(rightSibling->getNextLeaf());
			//从parent的children中删去this_node
			int adapt_pos = (this_node->getParent())->search(rightSibling->getData().back().first);
			auto adapt_iter = (this_node->getParent())->children.begin() + adapt_pos ;
			(this_node->getParent())->children.erase(adapt_iter);
			//更新索引和parent的true_num TODO
			//只有this_node的第一个可能出现在父辈的keys之中			
			int remove_pos = this_node->getParent()->remove(rightSibling->getData()[0].first);
			//如果返回的不是-1  说明parent中有这个key且已经删掉了 那就不可能再在父辈之上存在了，调整一下parent的true_num即可
			//由于是左孩子合并那说明父辈之上必然不会有this_node中的key了 《=TODO check！
			if (remove_pos != -1)
			{
				this_node->getParent()->setTrueNum(this_node->getParent()->keys.size());
			}
		}
		//更新父节点的children
		//更新父节点的索引
		solveInnerUnderflow(this_node->getParent(),key);
	}
}

void BPlusTree::solveInnerUnderflow(interiorNode * this_node,int key)
{
	int min_limit = this_node->getLimitNum() / 2;
	int max_limit = this_node->getLimitNum() / 2;
	while (this_node != nullptr)
	{
		if (this_node->getParent() == nullptr)
		{
			if (this_node->getTrueNum() == 0)
			{
				//更新root为t的第一个孩子？？？？？？TODO TODOTODOTODO!!!
				root = this_node->children[0];
				root->parent = nullptr;
			}
			break;
		}
		else if (this_node->getTrueNum() >= min_limit)
		{
			//自接更新当前节点的孩子
			break;
		}
		else if (this_node->getTrueNum() < min_limit)
		{
			auto leftSibling = this_node->getLeftSibling(this_node);
			auto rightSibling = this_node->getRightSibling(this_node);
			if (leftSibling != nullptr && (leftSibling->getTrueNum() >min_limit))
			{
				int adapt_pos = (this_node->getParent())->search(this_node->keys[0]) - 1;
				this_node->keys.insert(this_node->keys.begin(), (this_node->getParent()->keys[adapt_pos]));
				this_node->getParent()->keys[adapt_pos] = leftSibling->keys[leftSibling->keys.size() - 1];
				leftSibling->keys.pop_back();
				this_node->children.insert(this_node->children.begin(), leftSibling->children.back());
				leftSibling->children.pop_back();
				leftSibling->setTrueNum(leftSibling->keys.size());
				this_node->setTrueNum(this_node->keys.size());
				//向左借元素
				//更新父节点索引
				//更新父节点孩子
				break;
			}
			else if (rightSibling != nullptr && (rightSibling->getTrueNum() > this_node->getLimitNum() / 2))
			{
				//向右借
				//更新父索引
				//更新父孩子
				int adapt_pos = (this_node->getParent())->search(rightSibling->keys[0]) - 1;
				this_node->keys.push_back((this_node->getParent())->keys[adapt_pos]);
				(this_node->getParent())->keys[adapt_pos] = rightSibling->keys[0];
				rightSibling->keys.erase(rightSibling->keys.begin());
				(this_node->children).push_back(rightSibling->children[0]);
				rightSibling->children.erase(rightSibling->children.begin());
				rightSibling->setTrueNum(rightSibling->keys.size());
				this_node->setTrueNum(this_node->keys.size());
				break;
			}
			else if (leftSibling != nullptr && (this_node->getTrueNum() + leftSibling->getTrueNum()) <= this_node->getLimitNum())
			{
				this_node->mergeWithLeftSibling(leftSibling, this_node);
				int adapt_pos = (this_node->getParent())->search(this_node->keys[0]) - 1;
				auto parent_iter = (this_node->getParent())->keys.begin() + adapt_pos;
				auto parent_children_iter = (this_node->getParent())->children.begin() + adapt_pos + 1;
				(this_node->getParent())->keys.erase(parent_iter);
				(this_node->getParent())->children.erase(parent_children_iter);
				(this_node->getParent())->setTrueNum((this_node->getParent())->keys.size());
				leftSibling->setTrueNum(leftSibling->keys.size());
				//合并this_node和左孩子
				this_node = this_node->getParent();
			}
			else if (rightSibling != nullptr && (this_node->getTrueNum() + rightSibling->getTrueNum()) <= this_node->getLimitNum())
			{
				//合并this_node和右孩子
				this_node->mergeWithRightSibling(this_node, rightSibling);
				int adapt_pos = (this_node->getParent())->search(rightSibling->keys[0]) - 1;
				auto parent_iter = (this_node->getParent())->keys.begin() + adapt_pos;
				auto parent_children_iter = (this_node->getParent())->children.begin() + adapt_pos + 1;
				(this_node->getParent())->keys.erase(parent_iter);
				(this_node->getParent())->children.erase(parent_children_iter);
				(this_node->getParent())->setTrueNum((this_node->getParent())->keys.size());
				this_node->setTrueNum(this_node->keys.size());
				this_node = this_node->getParent();
			}
		}
	}
}

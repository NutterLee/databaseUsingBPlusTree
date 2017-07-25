#include"node.h"
int interiorNode::num_of_keys = 128;
int leafNode::num_of_items =128;
interiorNode::interiorNode()
{
	//setM(128);
	true_num_of_keys = 0;	
	//keys.reserve(num_of_keys);
	//children.reserve(num_of_keys + 1);
	parent = nullptr;
}

//TODO test 看是否满足要求 test done
int interiorNode::search(int key)
{
	int low = 0;
	int high = true_num_of_keys ;
	int mid = (low + high) / 2;
	while (low<=high)
	{
		mid = low + (high - low) / 2;
		if (mid >= keys.size()) return mid;
		if (keys[mid] >= key)
		{
			high = mid-1;		
		}
		else
		{
			low = mid+1;
		}
	}
	return low;
}

int interiorNode::find(int key)
{
	int pos = search(key);
	if (pos >= keys.size()) return -1;
	if (keys[pos] == key) return pos;
	else return -1;	
}
//TODO test 看看返回的对不对的 test done
interiorNode * interiorNode::getNextNode(int key)
{
	if (getType() == "leafNode")return nullptr;
	else
	{
		int pos = search(key);
		if (pos >= keys.size())return children[pos];
		if (keys[pos] == key) return children[pos + 1];
		else return children[pos];
	}
}

interiorNode * interiorNode::mergeWithLeftSibling(interiorNode * leftSibling, interiorNode * this_node)
{
	//内部节点的与左兄弟结合是一个类似逆时针旋转的过程，还要写入父节点的一个key
	if (leftSibling == nullptr || this_node == nullptr) return nullptr;
	//auto key_iter = this_node->keys.begin();
	auto child_iter = this_node->children.begin();
	int adapt_pos = (this_node->getParent())->search(this_node->keys[0])-1;
	leftSibling->keys.push_back((this_node->getParent())->keys[adapt_pos]);
	//将信息写入 child的parent重新绑定
	for(auto key_iter= (this_node->keys).begin();key_iter!= (this_node->keys).end();++key_iter)
	{
		(leftSibling->keys).push_back(*key_iter);
	}
	while (child_iter != this_node->children.end())
	{
		leftSibling->children.push_back(*child_iter);
		(*child_iter)->parent = leftSibling;
		(*child_iter)->setParent(leftSibling);
		child_iter++;
	}
	leftSibling->setTrueNum(leftSibling->keys.size());
	return leftSibling;
}

interiorNode * interiorNode::mergeWithRightSibling(interiorNode * this_node, interiorNode * rightSibling)
{
	if(this_node==nullptr||rightSibling==nullptr) 	return nullptr;
	int adapt_pos = (this_node->getParent())->search(rightSibling->keys[0]) - 1;
	this_node->keys.push_back((this_node->getParent())->keys[adapt_pos]);
	auto key_iter = rightSibling->keys.begin();
	auto child_iter = rightSibling->children.begin();
	while (key_iter != rightSibling->keys.end())
	{
		this_node->keys.push_back(*key_iter);
		key_iter++;
	}
	while (child_iter != rightSibling->children.end())
	{
		this_node->children.push_back(*child_iter);
		(*child_iter)->parent = this_node;
		(*child_iter)->setParent(this_node);
		child_iter++;
	}
	this_node->setTrueNum(this_node->keys.size());
	return this_node;
}

interiorNode::~interiorNode()
{
}

//TODO test 函数功能 test done
int interiorNode::insert(int key, int value)
{
	int pos = search(key);
	auto iter = keys.begin();
	if (pos == 0) keys.insert(iter, key);
	else
	{
		iter = iter + pos;
		if (iter == keys.end())
			{
				keys.push_back(key);
				true_num_of_keys++;
				return pos;
			}
		
		keys.insert(iter, key);
	}
	true_num_of_keys++;
	return pos;
}

int interiorNode::remove(int key)
{
	int remove_pos = find(key);
	if (remove_pos == -1) return -1;
	auto iter = keys.begin()+remove_pos;
	keys.erase(iter);
	setTrueNum(keys.size());
	return remove_pos;
}

interiorNode * interiorNode::getLeftSibling(interiorNode * this_node)
{
	if(this_node==nullptr||this_node->parent==nullptr)	return nullptr;
	if (this_node->keys.size() == 0)
	{
	//	cout << "error occurs in getLeftSibling func" << endl;
		return nullptr;
	}
	//search_pos是为了找兄弟，当前指针在其parent的children里面的下标
	int search_pos = (this_node->parent)->search((this_node->keys)[0]);
	if (this_node->getParent()->find((this_node->keys)[0]) == -1)
	{
		if (search_pos == 0) return nullptr;
		else return (this_node->getParent())->getChildren()[search_pos - 1];
	}
	else return  (this_node->getParent())->getChildren()[search_pos];
}

interiorNode * interiorNode::getRightSibling(interiorNode * this_node)
{
	if (this_node == nullptr || this_node->parent == nullptr)	return nullptr;
	if (this_node->keys.size() == 0)
	{
		//cout << "error occurs in getLeftSibling func" << endl;
		return nullptr;
	}
	//search_pos是为了找兄弟，当前指针在其parent的children里面的下标
	int search_pos = (this_node->parent)->search((this_node->keys)[0]);
	if ((this_node->parent)->keys.size() == 1)
	{
		if (this_node->getParent()->find((this_node->keys)[0]) == -1 && search_pos == 0) return (this_node->getParent())->children[1];
		else return nullptr;
	}
	if (this_node->getParent()->find((this_node->keys)[0]) == -1)
	{
		if (search_pos >= this_node->getParent()->keys.size()) return nullptr;
		else return (this_node->getParent())->children[search_pos + 1];
	}
	else
	{
		if (search_pos >= this_node->getParent()->keys.size()-1) return nullptr;
		else return (this_node->getParent())->getChildren()[search_pos + 2];
	}
}

void interiorNode::setM(int num)
{
	if (num <= 0) throw keyNumError();
	num_of_keys = num;
}

interiorNode * interiorNode::getNextParent(interiorNode * this_node)
{
	if (this_node->getRightSibling(this_node) != nullptr)
	{
		return this_node->getParent();
	}
	else
	{
		interiorNode* up_node = this_node->getParent();
		if (up_node->getParent() == nullptr) return nullptr;
		while( up_node!=nullptr&&(up_node->getRightSibling(up_node) == nullptr))
		{
			up_node = up_node->getParent();
		}
		if (up_node == nullptr) return nullptr;
		else 
		{
			interiorNode* next_parent=up_node->getRightSibling(up_node);
			while (next_parent->children.size() != 0)
			{
				next_parent = next_parent->children[0];
			}
			return next_parent;
		}
	}
	return nullptr;
}

leafNode::leafNode()
{
	true_num_of_items = 0;
	parent = nullptr;
	next_leaf = nullptr;
}

int leafNode::search(int key)
{
	int low = 0;
	int high = true_num_of_items;
	int mid = (low + high) / 2;
	while (low <= high)
	{
		mid = low + ( high-low) / 2;
		if (mid >= items.size()) {
			return mid;
		}
		if (items[mid].first>= key)
		{
			high = mid - 1;
		}
		else 
		{
			low = mid + 1;
		}
	}
	return low;	
}

int leafNode::find(int key)
{
	int pos = search(key);
	if (pos >= items.size()) return -1;
	if (items[pos].first == key)return pos;
	else return -1;
}

void leafNode::setL(int num)
{
	if (num <= 0) throw itemNumError();
	num_of_items = num;
}

int leafNode::insert(int key, int value)
{
	int pos = search(key);
	auto iter = items.begin();
	pair<int, int> to_do = { key,value };
	if (pos == 0) items.insert(iter, to_do);
	else
	{//TODO 检查一下这个要不要= test done
		iter = iter + pos;
		if (iter == items.end())
		{
			items.push_back(to_do);
			true_num_of_items++;
			return pos;
		}
		else	 items.insert(iter, to_do);
	}
	true_num_of_items++;
	return pos;
}

//TODO 待测试功能
int leafNode::remove(int key)
{
	//找到则erase pair
	int remove_pos = find(key);
	auto iter = items.begin();
	if (remove_pos == -1)return -1;
	else  iter = items.begin() + remove_pos;
	items.erase(iter);
	//重新设置true_num的值
	setTrueNum(items.size());
	return remove_pos;
}

interiorNode * leafNode::getLeftSibling(interiorNode * this_node)
{
	if (this_node == nullptr || this_node->getParent()== nullptr)	return nullptr;
	if (this_node->getData().size() == 0)
	{
		//cout << "error occurs in getLeftSibling func" << endl;
		return nullptr;
	}
	//search_pos是为了找兄弟，当前指针在其parent的children里面的下标
	int search_pos = (this_node->getParent())->search((this_node->getData())[0].first);
	if (this_node->getParent()->find((this_node->getData())[0].first) == -1)
	{
		if (search_pos == 0) return nullptr;
		else return (this_node->getParent())->getChildren()[search_pos - 1];
	}
	else return  (this_node->getParent())->getChildren()[search_pos ];
}

interiorNode * leafNode::getRightSibling(interiorNode * this_node)
{
	if (this_node == nullptr || this_node->getParent() == nullptr)	return nullptr;
	if (this_node->getData().size() == 0)
	{
		//cout << "error occurs in getLeftSibling func" << endl;
		return nullptr;
	}
	//search_pos是为了找兄弟，当前指针在其parent的children里面的下标
	int search_pos = (this_node->getParent())->search((this_node->getData())[0].first);
	if (search_pos >=(this_node->getParent()->getChildren().size() - 2)) return nullptr;
	if (this_node->getParent()->find((this_node->getData())[0].first) == -1)
	{
		return (this_node->getParent())->getChildren()[search_pos + 1];
	}
	else
	{
		return (this_node->getParent())->getChildren()[search_pos + 2];
	}
}

interiorNode * leafNode::mergeWithLeftSibling(interiorNode * leftSibling, interiorNode * this_node)
{
	if(leftSibling==nullptr||this_node==nullptr)	return nullptr;
	auto item_iter = this_node->getData().begin();
	while (item_iter != this_node->getData().end())
	{
		leftSibling->getData().push_back(*item_iter);
		item_iter++;
	}
	leftSibling->setTrueNum(leftSibling->getData().size());
	return leftSibling;
}

interiorNode * leafNode::mergeWithRightSibling(interiorNode * this_node, interiorNode * rightSibling)
{
	if(rightSibling==nullptr||this_node==nullptr) 	return nullptr;
	auto item_iter = rightSibling->getData().begin();
	while (item_iter != rightSibling->getData().end())
	{
		this_node->getData().push_back(*item_iter);
		item_iter++;
	}
	this_node->setTrueNum(this_node->getData().size());
	return this_node;
}

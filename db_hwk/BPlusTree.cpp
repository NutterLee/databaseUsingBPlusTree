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
	//��ʱthis_node�ǿ��ܴ���Ŀ��key�Ĵ�ڵ�
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
	//TODO ��β���/?????
	//TODO ���Ӹ����ڵ��أ��� test done
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
	//case 1 : ɾ���ڵ��ڵ��true_num����Ҫ���һ��
	if (this_node->getTrueNum() >= (this_node->getLimitNum() / 2))
	{
		//updateParent(this_node->parent, key);
		//����������
		//1. ֻ����ԭ��children�е�һλ�� ���п��ܳ�����֮ǰ��������
		if(remove_pos!=0)return true;
		else
		{
			//���key��parent��children�в�����С�������Ҳ��������Է�����
			int parent_remove_pos = (this_node->getParent())->find(key);
			if (parent_remove_pos == -1 && (key > (this_node)->getParent()->keys[0])) return true;
			//�����ڸ��ڵ������е�
			else if (parent_remove_pos != -1)
			{
				//��ԭnode�б�ɾ��Ԫ�ص���һ��key��������
				//this_node �϶���leafNode���Բ���ô��
				//if(remove_pos<this_node->getTrueNum()) ԭnode���ֻ��һ���Ļ� Ӧ���Ѿ���underflow������
				(this_node)->getParent()->keys[parent_remove_pos] = this_node->getData()[remove_pos].first;
				return true;
			}
			//�����ڸ��ڵ����ϵ�λ�����е�
			else
			{
				interiorNode* next_node = this_node->getParent();
				while (next_node != nullptr)
				{
					int next_remove_pos = next_node->find(key);
					//��Ȼû�ҵ�������һ��
					if (next_remove_pos == -1)
					{
						next_node = next_node->getParent();
						continue;
					}
					else
					{
						//{//��Ҷ�ӽڵ���ɾ����key����һ��������ȥ
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
			//���key��parent��children�в�����С�������Ҳ��������Է�����
			int parent_remove_pos = (this_node->getParent())->find(key);
			if (parent_remove_pos == -1 && (key > (this_node)->getParent()->keys[0]))
			{
				solveUnderflow(this_node, key);
				return true;
			}
			//�����ڸ��ڵ������е�
			else if (parent_remove_pos != -1)
			{
				//��ԭnode�б�ɾ��Ԫ�ص���һ��key��������
				//this_node �϶���leafNode���Բ���ô��
				//if(remove_pos<this_node->getTrueNum()) ԭnode���ֻ��һ���Ļ� Ӧ���Ѿ���underflow������
				(this_node)->getParent()->keys[parent_remove_pos] = this_node->getData()[remove_pos].first;
				solveUnderflow(this_node, key);
				return true;
			}
			//�����ڸ��ڵ����ϵ�λ�����е�
			else
			{
				interiorNode* next_node = this_node->getParent();
				while (next_node != nullptr)
				{
					int next_remove_pos = next_node->find(key);
					//��Ȼû�ҵ�������һ��
					if (next_remove_pos == -1)
					{
						next_node = next_node->getParent();
						continue;
					}
					else
					{
						//{//��Ҷ�ӽڵ���ɾ����key����һ��������ȥ
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

//��ʱ�ϳ� �Ժ��ٿ��Ƿ�װ
void BPlusTree::updateParent(interiorNode * this_node, int key)
{
	//˵���� �ݹ���� ��ʼ��this_nodeӦ����ɾ����key��leafNode
	if (this_node == nullptr) return;
	
	int remove_pos = this_node->find(key);
	//z�ڴ�node���ҵ���key
	if (remove_pos != -1)
	{

	}
	auto iter = (this_node->keys).begin()+remove_pos;
	(this_node->keys).erase(iter);
	this_node->setTrueNum(this_node->keys.size());
	//TODO �����

}

interiorNode * BPlusTree::moveToLeaf(int key)
{
	interiorNode* this_node = root;
	if (this_node == nullptr)return nullptr;
	//���ƶ���Ҷ�ӽڵ�
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
		//��root�ڵ�Ͳ���root�ڵ�
		if (this_node->parent != nullptr)
		{
			//����root�ڵ�
			int insert_pos = (this_node->parent)->insert((this_node->getData())[split_pos].first, 0);
			interiorNode* new_node = new leafNode();
			//assert(new_node->getType() != "leafNode" || this_node->getType() != "leafNode");
			for (int i = split_pos; i < this_node->getData().size(); i++)
			{
				new_node->getData().push_back(this_node->getData()[i]);
			}
			//��parent��children�������new_node
			auto iter = (this_node->parent->children).begin()+insert_pos+1;
			//for (int i = 0; i <=insert_pos; i++) iter++;
			if (iter == (this_node->parent->children).end())
				(this_node->parent->children).push_back(new_node);
			else (this_node->parent->children).insert(iter,new_node);
			this_node->parent->setTrueNum(this_node->parent->keys.size());
			this_node->getData().resize(split_pos);
			//new_node��parent������
			new_node->setParent(this_node->parent);
			new_node->parent = this_node->parent;
			//��������true_num��ֵ
			this_node->setTrueNum(this_node->getData().size());
			new_node->setTrueNum(new_node->getData().size());
			
			//���ú�Ҷ��֮�������
			new_node->setNextLeaf(this_node->getNextLeaf());
			this_node->setNextLeaf(new_node);
		}
		else
		{
			//��root�ڵ�ʱ
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
			//���ú�Ҷ��֮�������
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
		//����root�ڵ�ʱ
		if (this_node->parent != nullptr)
		{
			//����root����һ���interiorNodeʱ����parent��keys�������ָ���key
			//step 1 ��split_node���keyд��parent��key����
			int insert_pos=(this_node->parent)->insert((this_node->keys)[split_pos], 0);
			//step2 �½�һ��node��д����Ҫ��keys��children
			auto new_node = new interiorNode();
			auto key_iter = (this_node->keys).begin()+split_pos+1;
			auto children_iter = (this_node->children).begin()+split_pos+1;
			auto parent_iter = ((this_node->parent)->children).begin()+insert_pos+1;
			//TODO ��һ��������� ����Խ��͵�����ʧЧ ����Ҫע��		 test done
			//��ԭnode��split���Ժ��ȫ��д���µ�node��
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
			//step3 ��new_nodeд�뵽children����
			//ָ��������childrenλ��			
			((this_node->parent)->children).insert(parent_iter, new_node);
			//step4 ����ԭ��node�Ķ��ಿ��
			//TODO �������resize�Ĵ�С��û������ test done
				(this_node->keys).resize(this_node->keys.size()/2);
				(this_node->children).resize((this_node->children.size() +1)/ 2);
		
			//step5�� ����һ��parent�ڵ�
			//��һ�¸�ĸ�ڵ�
			new_node->setParent(this_node->parent);
			new_node->parent = this_node->parent;
			//this_node->parent = this_node->parent->parent;
			//step6�� ����true_num_of_keys
			this_node->setTrueNum(this_node->keys.size());
			new_node->setTrueNum( new_node->keys.size());
			(this_node->parent)->setTrueNum( this_node->parent->keys.size());	
			//step7���ݹ鿴�����ڵ��ǲ���Ҳ������
			solveInnerOverflow(this_node->parent);
		}
		//rootҲҪoverflow�����
		else
		{
			//step1 �½�δ����root
			auto split_node = new interiorNode();
			split_node->keys.push_back((this_node->keys)[split_pos]);
			//step2 �½�������node
			auto new_node = new interiorNode();
			auto key_iter = (this_node->keys).begin()+split_pos+1;
			auto children_iter = (this_node->children).begin()+split_pos+1;
			//��ԭnode��split���Ժ��ȫ��д���µ�node��
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
			//Step3 ɾ��ԭnode���������
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
			//step 4 �޸�children��parent���ֹ�ϵ
			split_node->children.push_back(this_node);
			split_node->children.push_back(new_node);
			this_node->setParent(split_node);
			new_node->setParent(split_node);
			this_node->parent = split_node;
			new_node->parent = split_node;
			root=split_node;
			//step5�� ����true_num_of_keys
			this_node->setTrueNum(this_node->keys.size());
			new_node->setTrueNum( new_node->keys.size());
			//rootû��parent(this_node->parent)->setTrueNum(this_node->parent->keys.size());
			//step6�� �Ѿ��޸ĵ�����ĩ���� ���Խ����ݹ���
			return;
		}		
	}
}

void BPlusTree::solveUnderflow(interiorNode * this_node, int key)
{
	//���½������Ҷ�ӽڵ��underflow
	if (this_node->getTrueNum() >= this_node->getLimitNum() / 2) return;
	//case 1 p�Ǹ��ڵ�
	if (this_node == root)
	{
		if (this_node->getTrueNum() == 0) root = nullptr;
		return;
	}
	//���Ǹ��ڵ�
	//step1 �������ھӵ����
	auto leftSibling = this_node->getLeftSibling(this_node);
	auto rightSibling = this_node->getRightSibling(this_node);
	int min_limit = this_node->getLimitNum() / 2;
	int max_limit = this_node->getLimitNum();
	if (leftSibling != nullptr&&leftSibling->getTrueNum() > min_limit)
	{
		//����躢��
		//step1: �����ֵܵ����һλ�������������ŵ�this_node����ǰ�棬ɾ�����ֵ����һ��
		(this_node->getData()).insert(this_node->getData().begin(), (leftSibling->getData())[leftSibling->getData().size() - 1]);
		leftSibling->getData().pop_back();
		leftSibling->setTrueNum(leftSibling->getData().size());
		this_node->setTrueNum(this_node->getData().size());
		int adapt_pos = (this_node->getParent())->search(this_node->getData().back().first);
		this_node->getParent()->keys[adapt_pos] = this_node->getData()[0].first;
		//����true_num��ֵ
		//���¸��ڵ�����
	}
	else if (rightSibling != nullptr&&rightSibling->getTrueNum() > rightSibling->getLimitNum() / 2)
	{
		//���ҽ躢��
		//step �������ֵܵĵ�һλ �ӵ��Լ����� �������ֵ�����ɾ��
		//��parent��Ӧ��������Ϊ���ֵ�ɾ���Ժ�ĵ�һλ
		(this_node->getData()).insert(this_node->getData().end(), rightSibling->getData()[0]);
		rightSibling->getData().erase(rightSibling->getData().begin());
		rightSibling->setTrueNum(rightSibling->getData().size());
		this_node->setTrueNum(this_node->getData().size());
		int adapt_pos = (this_node->getParent())->search(rightSibling->getData().back().first)-1;
		this_node->getParent()->keys[adapt_pos] = rightSibling->getData()[0].first;
		//����true_num��ֵ
		//���¸��ڵ�����
	}
	else
	{
		if (leftSibling != nullptr && (this_node->getTrueNum() + leftSibling->getTrueNum()) <= this_node->getLimitNum())
		{
			//step1 ������Ϣ
			this_node->mergeWithLeftSibling(leftSibling, this_node);
			//step2 ����leaf֮�����ӵ���Ϣ
			leftSibling->setNextLeaf( this_node->getNextLeaf());
			//��parent��children��ɾȥthis_node
			int adapt_pos = (this_node->getParent())->search(this_node->getData().back().first);
			if ((this_node->getParent())->find(this_node->getData().back().first) != -1) adapt_pos++;
			auto adapt_iter =( (this_node->getParent())->children).begin()+adapt_pos;
			((this_node->getParent())->children).erase(adapt_iter);
			//����������parent��true_num TODO
			//ֻ��this_node�ĵ�һ�����ܳ����ڸ�����keys֮��
			int remove_pos = this_node->getParent()->remove(this_node->getData()[0].first);
			//������صĲ���-1  ˵��parent�������key���Ѿ�ɾ���� �ǾͲ��������ڸ���֮�ϴ����ˣ�����һ��parent��true_num����
			//���������Ӻϲ���˵������֮�ϱ�Ȼ������this_node�е�key�� ��=TODO check��
			if (remove_pos != -1)
			{
				this_node->getParent()->setTrueNum(this_node->getParent()->keys.size());
			}
			//�ϲ�this_node������
		}
		else if(rightSibling!=nullptr&&(this_node->getTrueNum()+rightSibling->getTrueNum())<= this_node->getLimitNum())
		{
			//�ϲ�this_node���Һ���
			this_node->mergeWithRightSibling(this_node, rightSibling);
			this_node->setNextLeaf(rightSibling->getNextLeaf());
			//��parent��children��ɾȥthis_node
			int adapt_pos = (this_node->getParent())->search(rightSibling->getData().back().first);
			auto adapt_iter = (this_node->getParent())->children.begin() + adapt_pos ;
			(this_node->getParent())->children.erase(adapt_iter);
			//����������parent��true_num TODO
			//ֻ��this_node�ĵ�һ�����ܳ����ڸ�����keys֮��			
			int remove_pos = this_node->getParent()->remove(rightSibling->getData()[0].first);
			//������صĲ���-1  ˵��parent�������key���Ѿ�ɾ���� �ǾͲ��������ڸ���֮�ϴ����ˣ�����һ��parent��true_num����
			//���������Ӻϲ���˵������֮�ϱ�Ȼ������this_node�е�key�� ��=TODO check��
			if (remove_pos != -1)
			{
				this_node->getParent()->setTrueNum(this_node->getParent()->keys.size());
			}
		}
		//���¸��ڵ��children
		//���¸��ڵ������
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
				//����rootΪt�ĵ�һ�����ӣ�����������TODO TODOTODOTODO!!!
				root = this_node->children[0];
				root->parent = nullptr;
			}
			break;
		}
		else if (this_node->getTrueNum() >= min_limit)
		{
			//�ԽӸ��µ�ǰ�ڵ�ĺ���
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
				//�����Ԫ��
				//���¸��ڵ�����
				//���¸��ڵ㺢��
				break;
			}
			else if (rightSibling != nullptr && (rightSibling->getTrueNum() > this_node->getLimitNum() / 2))
			{
				//���ҽ�
				//���¸�����
				//���¸�����
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
				//�ϲ�this_node������
				this_node = this_node->getParent();
			}
			else if (rightSibling != nullptr && (this_node->getTrueNum() + rightSibling->getTrueNum()) <= this_node->getLimitNum())
			{
				//�ϲ�this_node���Һ���
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

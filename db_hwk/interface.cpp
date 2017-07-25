#include "interface.h"

database::~database()
{
}

void database::db_open()
{
	fstream file("index.dat");
	inner_tree = new BPlusTree();
	//�����ݿ�ʱ����д��index
	if (file.is_open())
	{
		//test only code
		cout << "index read success" << endl;
		file.close();
		readFromFile((*inner_tree), "index.dat");
	}
	//���뻺��ռ�
	inner_write_buffer = new writeBuffer();
	inner_read_buffer = new readBuffer();
}

void database::db_close()
{
	//TODO:
	//step 1 ��д�뻺�������д������
	refreshWriteCache_force();
	//step 2 ����tree���Ѿ�������ĺ������
	//step 3 д��tree���ļ���
	saveToFile(*inner_tree);
}

bool database::db_store(int key, int value)
{
	if (db_fetch(key) != -1) return false;
	//step 1 �ȿ�д����������û��Ҫstore�� �еĻ����滻��
	if (inner_write_buffer->search(key) != nullptr)
	{
		inner_write_buffer->modify(key, value);	
	}
	else
	{
		inner_write_buffer->add(key, value, "add",-1);
	}
	//��Ҫ�Ļ����д�뻺�������
	refreshWriteCache();
	//����read������д��
	inner_read_buffer->modify(key, value);
	return true;
}

int database::db_fetch(int key)
{	
	//step1 ���ڶ�������Ѱ�� ��������������в��Ҳ�����д�뻺����
	//��Ϊ��д��write����ʱ Ҳ�и���read���� ����ֻҪ��read�����в��Ҿͺ���
	int b_result = inner_read_buffer->find(key);
	if (b_result != -1)return b_result;
	//step2 ��B+����Ѱ�Ҳ����뻺��
	int result = inner_read_buffer->find(key);
	if (result == -1)
	{
		auto tmp_ptr=inner_tree->search(key);
		if (tmp_ptr == nullptr) return -1;
		//TODO �˴�Ӧ���Ǹ���B+�����ص�ֵ���ļ�����ȥ��λ��ȡֵ
		//�������ݵ�readBuffer�У��˴��������Զ�ˢ�¶����棩
		readFromData(tmp_ptr->second, 128);
	}
	int tmp_result = inner_read_buffer->find(key);
	if (tmp_result == -1) return -1;
	else return tmp_result;
}

void database::db_clear()
{
	delete inner_read_buffer;
	delete inner_write_buffer;
	delete inner_tree;
	inner_read_buffer = new readBuffer();
	inner_write_buffer = new writeBuffer();
	inner_tree = new BPlusTree();
	fstream file1("data.dat",ios::out|ios::trunc);
	fstream file2("index.dat",ios::out|ios::trunc);
	file1.close();
	file2.close();
}

bool database::db_modify(int key, int value)
{
	if (db_fetch(key) == -1) return false;
	//step1 ����write�����read�����в��Һ��޸�
	auto ptr = inner_write_buffer->search(key);
	if (ptr != nullptr)
	{
		inner_write_buffer->modify(key, value);
		inner_read_buffer->modify(key, value);
		refreshWriteCache();
		return true;
	}
	else
	{
		int pos = (inner_tree->search(key))->second;
		if (pos == -1)return false;
		else
		{
			inner_write_buffer->add(key, value, "modify", pos);
			inner_read_buffer->modify(key, value);
			refreshWriteCache();
			return true;
		}
	}
	//step2 �ҵõ�����»��棬ע��Ҫ���ֶ����������µ�
	//����readBuffer�в��ң�readBuffer�пɿ�������ֻ�޶���key��valueʵʱ����
	//���Ҳ�����b+������ȥ��
	//step3����д�����refresh����
}

bool database::db_delete(int key)
{
	if (db_fetch(key) == -1) return false;
	else
	{
		auto write_ptr = inner_write_buffer->search_pro(key);
		//case 1 ��������д�������滹û��add��ȥ��
		if (write_ptr != nullptr && (*write_ptr).operataion == "add")
		{
			inner_write_buffer->remove(key);
			if (inner_read_buffer->find(key) != -1)	inner_read_buffer->remove(key);
			return true;
		}
		//case2 ������ǵĻ� ����һ��ԭ����������û��
		auto ptr = inner_tree->search(key);
		if (ptr == nullptr) return false;
		int offpos = (inner_tree->search(key))->second;
		if (offpos == -1)throw unexpectedError();
		inner_write_buffer->add(key, 0, "remove", offpos);
		if(inner_read_buffer->find(key)!=-1)	inner_read_buffer->remove(key);
		inner_tree->remove(key);
		refreshWriteCache();
		return true;
	}
}

void database::writeToFile(int key, int value,int offPos, string operation ,fstream& file)
{
	string tmp_key = to_string(key);
	while (tmp_key.length() != 9) tmp_key = "0" + tmp_key;
	string tmp_value = to_string(value);
	while (tmp_value.length() != 9) tmp_value = "0" + tmp_value;
	//tmp_str��Ҫ���ݸ��ײ�Ĳ���
	string tmp_str = tmp_key + " " + tmp_value + " ";
	//if (!file.is_open()) throw fileError();
	if (operation == "add")
	{
		int pos = appendwrite(tmp_str, 20, file);
		inner_tree->insert(key, pos);
	}
	else if (operation == "remove")
	{
		string tmp = "000000000 000000000 ";
		overwrite(tmp, offPos, 20, file);
		inner_tree->remove(key);
	}
	else if (operation == "modify")
	{
		overwrite(tmp_str, offPos, 20, file);
	}
	else throw operationError();
}

void database::readFromData(int offPos, int num)
{
	fstream file("data.dat", ios::binary | ios::in | ios::out);
	//if (!file.is_open()) throw fileError();
	string rawdata = readRawData(offPos, num, file);
	stringstream tmp_flow;
	tmp_flow << rawdata;
	int pos_record=offPos;
	for (int i = 0; i < num; i++)
	{
		int key;
		int value;
		tmp_flow >> key >> value;
		//���Ǳ�ɾ�����ǵ����
		if (key == 0 && value == 0)
		{
			pos_record += 20;
			continue;
		}
		//��ΪreadBuffer�е�ֵ��������writeBuffer�е�ֵ��������readBufferΪ׼
		if (inner_read_buffer->find(key) == -1)
		{
			inner_read_buffer->modify(key, value, pos_record);
		}
	}
}

void database::refreshWriteCache()
{
	if (inner_write_buffer->getTrueSize() < inner_write_buffer->getSize())	return;
	else
	{ 
		fstream file("data.dat", ios_base::binary | ios_base::in | ios_base::out);
		if (!file.is_open()) cout << "file error" << endl;
		for (auto iter = inner_write_buffer->getData().begin(); iter != inner_write_buffer->getData().end(); iter++)
		{
			int key = (*iter).second.key;
			int value = (*iter).second.value;
			int offPos = (*iter).second.offPos;
			string operation = (*iter).second.operataion;
			writeToFile(key, value, offPos, operation, file);
		}
		inner_write_buffer->clear();
		file.close();
	}
	
}
void database::refreshWriteCache_force()
{
	if (inner_write_buffer->getTrueSize() < inner_write_buffer->getSize())	return;
	else
	{
		fstream file("data.dat", ios_base::binary | ios_base::in | ios_base::out);
		for (auto iter = inner_write_buffer->getData().begin(); iter != inner_write_buffer->getData().end(); iter++)
		{
			int key = (*iter).second.key;
			int value = (*iter).second.value;
			int offPos = (*iter).second.offPos;
			string operation = (*iter).second.operataion;
			writeToFile(key, value, offPos, operation, file);
		}
		inner_write_buffer->clear();
		file.close();
	}
}

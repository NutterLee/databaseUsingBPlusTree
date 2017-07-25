#include "interface.h"

database::~database()
{
}

void database::db_open()
{
	fstream file("index.dat");
	inner_tree = new BPlusTree();
	//打开数据库时就先写入index
	if (file.is_open())
	{
		//test only code
		cout << "index read success" << endl;
		file.close();
		readFromFile((*inner_tree), "index.dat");
	}
	//申请缓存空间
	inner_write_buffer = new writeBuffer();
	inner_read_buffer = new readBuffer();
}

void database::db_close()
{
	//TODO:
	//step 1 将写入缓存的数据写入数据
	refreshWriteCache_force();
	//step 2 更新tree，已经在上面的函数完成
	//step 3 写入tree到文件中
	saveToFile(*inner_tree);
}

bool database::db_store(int key, int value)
{
	if (db_fetch(key) != -1) return false;
	//step 1 先看写缓存里面有没有要store的 有的话就替换他
	if (inner_write_buffer->search(key) != nullptr)
	{
		inner_write_buffer->modify(key, value);	
	}
	else
	{
		inner_write_buffer->add(key, value, "add",-1);
	}
	//必要的话解决写入缓存的问题
	refreshWriteCache();
	//并在read缓存中写入
	inner_read_buffer->modify(key, value);
	return true;
}

int database::db_fetch(int key)
{	
	//step1 先在读缓存中寻找 如果不存在则到树中查找并将其写入缓存中
	//因为在写入write缓存时 也有更新read缓存 所以只要在read缓存中查找就好了
	int b_result = inner_read_buffer->find(key);
	if (b_result != -1)return b_result;
	//step2 在B+树中寻找并加入缓存
	int result = inner_read_buffer->find(key);
	if (result == -1)
	{
		auto tmp_ptr=inner_tree->search(key);
		if (tmp_ptr == nullptr) return -1;
		//TODO 此处应该是根据B+树返回的值到文件流中去定位读取值
		//读入数据到readBuffer中（此处不允许自动刷新读缓存）
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
	//step1 现在write缓存和read缓存中查找和修改
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
	//step2 找得到则更新缓存，注意要保持读缓存是最新的
	//不在readBuffer中查找，readBuffer中可靠的数据只限定与key与value实时更新
	//再找不到到b+树里面去找
	//step3调用写缓存的refresh操作
}

bool database::db_delete(int key)
{
	if (db_fetch(key) == -1) return false;
	else
	{
		auto write_ptr = inner_write_buffer->search_pro(key);
		//case 1 可能是在写缓存里面还没有add进去的
		if (write_ptr != nullptr && (*write_ptr).operataion == "add")
		{
			inner_write_buffer->remove(key);
			if (inner_read_buffer->find(key) != -1)	inner_read_buffer->remove(key);
			return true;
		}
		//case2 如果不是的话 就找一下原来的树上有没有
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
	//tmp_str是要传递给底层的参数
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
		//这是被删除覆盖的情况
		if (key == 0 && value == 0)
		{
			pos_record += 20;
			continue;
		}
		//因为readBuffer中的值可能是在writeBuffer中的值，所以以readBuffer为准
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

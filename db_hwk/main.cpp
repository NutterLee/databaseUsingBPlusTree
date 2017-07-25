#include "error.h"
#include"interface.h"
#include<fstream>
#include<unordered_map>
#include<sstream>
#include<stdlib.h>
#include<ctime>

using namespace std;
int trueMain();
void test_write_to_file();
void test_read_from_file();
void test_modify();
void test_remove();
void test_general();
unordered_map<int, int>data_record;
int random(int low, int high)
{
	int r = rand() % (high - low + 1) + low;
	return r;
}

int main()
{	
	/*
	BPlusTree test_tree;	
	int key;
	int value;

	for (int i =50; i >0; i--)
	{
		int key = random(1, 1000);
		test_tree.insert(key, i);
	}
	cout << "insert finiish!" << endl;
	saveToFile(test_tree);
	BPlusTree  test_read_tree;
	readFromFile(test_read_tree, "index.dat");
	test_read_tree.traverse(1000);
	*/
	/*
	BPlusTree test_tree;
	fstream test_file("sample");
	string line;
	if (!test_file) { cerr << "file error" << endl; }
	for(int i =0;i<100;i++)
{
		int key, value;
		test_file >> key >> value;
		cout << "processing key: " << key+i<< " value: " << value << endl;
		test_tree.insert(key+i, value);
	}
	/*
	test_tree.traverse(1000);
	for (int i = 0; i < 100; i++)
	{
		test_tree.remove(i);
		cout << "---------------->"<< i <<"<-----------------" << endl;
	}
	test_tree.traverse(100);
	
	saveToFile(test_tree);
	*/
	//trueMain();
	srand(time(NULL));
	 test_write_to_file();
	test_read_from_file();
	test_modify();
	test_remove();
	system("pause");
	return 0;
}

//上面的main用于测试 以下是真正的main函数
int trueMain()
{
	database simple_db;
	int choice;
	cout << "--------------------简易数据库--------------------" << endl;
	cout << "#操作选项：" << endl;	
	cout << "<0> 根据key值查询数据" << endl;
	cout << "<1> 根据<key, value>添加数据" << endl;
	cout << "<2> 根据key值删除数据" << endl;
	cout << "<3> 根据<key, value>修改数据" << endl;
	cout << "<4> 清空数据库 " << endl;
	cout << "<5>退出程序" << endl;
	simple_db.db_open();
	while (true)
	{
		cout << "#请输入选择：" << endl;
		cin >> choice;
		switch (choice)
		{
		case 0 :
		{
			int key;
			cout  << "请输入key值：" << endl;
			cin >> key;
			int result = simple_db.db_fetch(key);
			if (result == -1)cout << "未找到此key对应的键值！" << endl;
			else
			{
				cout << "key: " << key << " 对应的值为：" << result << endl;
			}
			break;
		}
		case 1:
		{
			int key;
			int value;
			cout << "请输入key与value：" << endl;
			cin >> key >> value;
			if (simple_db.db_store(key, value))
			{
				cout <<"key: "<<key<<" value: "<<value<< " 写入完成！" << endl;
			}
			else
			{
				cout << "写入失败！此key：" << key << " 已存在" << endl;
			}
			break;
		}
		case 2:
		{
			int key;
			cout << "请输入要删除的key: " << endl;
			cin >> key;
			if (simple_db.db_delete(key))
			{
				cout << "key " << endl << key << " 删除成功！"<<endl;
			}
			else
			{
				cout << "删除失败，无法找到对应的key" << endl;
			}
			break;
		}
		case 3:
		{
			int key;
			int value;
			cout << "请输入key与value：" << endl;
			cin >> key >> value;
			if (simple_db.db_modify(key, value))
			{
				cout << "key: " << key << " value: " << value << " 修改完成！" << endl;
			}
			else
			{
				cout << "修改失败！此key：" << key << " 不存在" << endl;
			}
			break;
		}
		case 4:
		{
			cout << "注意：清空操作不可恢复！确认清空数据库吗？(y/n)" << endl;
			char inner_choice;
			cout << "请输入你的选择(y/n) :" << endl;
			cin >> inner_choice;
			if (inner_choice == 'y')
			{
				simple_db.db_clear();
				cout << "数据已清空！" << endl;
			}
			break;
		}
		case 5:
		{
			simple_db.db_close();
			return 0;
		}
		default:
		{
			cout << "选项输入错误，请重新输入！" << endl;
			break;
		}
		}
	}
	

	return 0;
}

//专门测试程序
void test_write_to_file()
{
	cout << "step1 写入100w条随机数据中" << endl;
	fstream test_file("test_data.txt", ios::in);
	if (!test_file.is_open())
	{
		cout << "测试文件不存在！" << endl;
		system("pause");
	}
	int key;
	int value;
	database test_base;
	test_base.db_clear();
	test_base.db_open();
	while (test_file >> key >> value)
	{
		data_record[key] = value;
		test_base.db_store(key, value);
	}
	test_base.db_close();
	cout << "写入完成！" << endl;
}

void test_read_from_file()
{
	cout << "step2 测试从已保存的文件里面读取" << endl;
	database test_base;
	test_base.db_open();
	auto iter = data_record.begin();
	for (int i = 0; i < 100; i++)
	{
		int pos = random(0, data_record.size() - 1);
		for (int k = 0; k < pos; k++) iter++;
		int key = (*iter).first;
		int target_result = (*iter).second;
		int result = test_base.db_fetch(key);
		if (result == target_result) cout << "passed!" << endl;
		else
		{
			cout << "failed!" << endl;
			cout << "the key: " << key << "the targeted value: " << target_result << endl;
			cout << "result from the program: " << result << endl;
			system("pause");
		}
	}	
	test_base.db_close();
	cout << "测试完成！" << endl;
}

void test_modify()
{
	cout << "step3 测试从已保存的文件修改文件" << endl;
	database test_base;
	test_base.db_open();
	auto iter = data_record.begin();
	for (int i = 0; i < 100; i++)
	{
		int pos = random(0, data_record.size() - 1);
		for (int k = 0; k < pos; k++) iter++;
		int key = (*iter).first;
		int result_original = data_record[key];
		int target_result = random(2,78965);
		data_record[key] = target_result;
		test_base.db_modify(key, target_result);
		int result = test_base.db_fetch(key);
		if (result == target_result) cout << "passed!" << endl;
		else
		{
			cout << "failed!" << endl;
			cout << "the key: " << key << "the targeted value: " << target_result << endl;
			cout << "result from the program: " << result << endl;
			system("pause");
		}
	}
	test_base.db_close();
	cout << "测试完成！" << endl;
}

void test_remove()
{
	cout << "step4 测试从已保存的文件里面读取然后删除" << endl;
	database test_base;
	test_base.db_open();
	auto iter = data_record.begin();
	for (int i = 0; i < 100; i++)
	{
		int pos = random(0, data_record.size() - 1);
		for (int k = 0; k < pos; k++) iter++;
		int key = (*iter).first;
		int target_result = (*iter).second;
		test_base.db_delete(key);
		int result = test_base.db_fetch(key);
		if (result == -1) cout << "passed!" << endl;
		else
		{
			cout << "failed!" << endl;
			cout << "the key: " << key << "the targeted value: " << "-1" << endl;
			cout << "result from the program: " << result << endl;
			system("pause");
		}
	}
	test_base.db_close();
	cout << "测试完成！" << endl;
}

void test_general()
{

}

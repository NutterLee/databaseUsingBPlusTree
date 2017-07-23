#include "error.h"
#include"interface.h"
#include<fstream>
#include<sstream>

using namespace std;


int main()
{
	
	BPlusTree test_tree;

	fstream test_file("sample");
	//std::locale::global(std::locale(""));
	//test_file.open("sample.txt",ios::in|ios::out);
	string line;
	if (!test_file) { cerr << "file error" << endl; }
	for(int i =0;i<99;i++)
{
		int key, value;
		test_file >> key >> value;
		cout << "processing key: " << key<< " value: " << value << endl;
		test_tree.insert(key, value);
	}
	test_tree.traverse(1000);
	for (int i = 0; i < 100; i++)
	{
		test_tree.remove(i);
		cout << "---------------->"<< i <<"<-----------------" << endl;
	}
	test_tree.traverse(100);
	
	saveToFile(test_tree);
	BPlusTree test_tree2;
	readFromFile(test_tree2,"index.dat");
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
	cout << "<3> 清空数据库" << endl;
	cout << "<4>退出程序" << endl;
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
				cout << "key: " << key << "对应的值为：" << result << endl;
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
				cout <<"key: "<<key<<"value: "<<value<< "写入完成！" << endl;
			}
			else
			{
				cout << "写入失败！此key：" << key << "已存在" << endl;
			}
			break;
		}
		case 2:
		{
			break;
		}
		case 3:
		{
			break;
		}
		case 4:
		{
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
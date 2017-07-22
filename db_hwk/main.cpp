#include "error.h"
#include "node.h"
#include"BPlusTree.h"
#include<fstream>
#include<sstream>

using namespace std;

int main()
{
	BPlusTree test_tree;
	/*
	test_tree.insert(10, 1011);
	//test_tree.traverse(4);
	test_tree.insert(11, 1102);
	//test_tree.traverse(4);
	test_tree.insert(12, 1034);
	//test_tree.traverse(4);
	test_tree.insert(13, 1025);
	test_tree.traverse(9);
	cout << "---------" << endl;
	test_tree.insert(9, 1233);
	test_tree.insert(16, 1999);
	test_tree.traverse(9);
	cout << "---------" << endl;
	test_tree.insert(15, 1818);
	test_tree.traverse(9);
	cout << "---------" << endl;
	test_tree.insert(23, 1777);
	test_tree.traverse(9);
	cout << "---------" << endl;
	test_tree.insert(19, 2999);
	test_tree.traverse(100);
	cout << "---------" << endl;
	test_tree.insert(28, 1888);
	test_tree.traverse(100);
	cout << "---------" << endl;
	test_tree.insert(30, 1767);
	test_tree.insert(48, 1997);
	test_tree.traverse(100);
	cout << "---------" << endl;
	test_tree.insert(47, 999);
	test_tree.insert(49, 10222);
	test_tree.traverse(100);
	cout << "---------" << endl;
	test_tree.insert(54, 1033);
	test_tree.traverse(100);
	cout << "---------" << endl;
	*/
	fstream test_file("sample");
	//std::locale::global(std::locale(""));
	//test_file.open("sample.txt",ios::in|ios::out);
	string line;
	if (!test_file) { cerr << "file error" << endl; }
	for(int i =0;i<100;i++)
{
		int key, value;
		test_file >> key >> value;
		cout << "processing key: " << key << " value: " << value << endl;
		test_tree.insert(key, value);
	}
	test_tree.traverse(1000);
	for (int i = 0; i < 99; i++)
	{
		test_tree.remove(i);
		cout << "---------------->"<< i <<"<-----------------" << endl;
		test_tree.traverse(100);
	}
	/*

	test_tree.remove(9);
	test_tree.traverse(100);
	test_tree.remove(2);
	test_tree.traverse(100);
	cout << "---------------------" << endl;
	test_tree.remove(6);
	test_tree.traverse(100);
	cout << "-----------------" << endl;
	test_tree.remove(5);
	test_tree.traverse(100);
	test_tree.remove(4);
	test_tree.traverse(100);
	test_tree.remove(7);
	test_tree.traverse(100);
	cout << "-------------------------" << endl;
	test_tree.remove(3);
	test_tree.traverse(100);
	cout << "------------------------" << endl;
	test_tree.remove(1);
	*/
	system("pause");
	return 0;
}
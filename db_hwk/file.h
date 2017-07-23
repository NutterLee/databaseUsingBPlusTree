#pragma once
#include"BPlusTree.h"
#include<fstream>
#include<queue>
#include<sstream>
#include<string>
#include<unordered_set>
using namespace std;
//文件读取操作的类

void saveToFile(BPlusTree& tree);
void readFromFile(BPlusTree& tree, string filename);

string ptrToString(interiorNode* node);

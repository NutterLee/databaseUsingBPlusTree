#pragma once
#include"BPlusTree.h"
#include<fstream>
#include<queue>
#include<sstream>
#include<string>
#include<unordered_set>
using namespace std;
//�ļ���ȡ��������

void saveToFile(BPlusTree& tree);
void readFromFile(BPlusTree& tree, string filename);

string ptrToString(interiorNode* node);

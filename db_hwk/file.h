#pragma once
#include"BPlusTree.h"
#include<fstream>
#include<queue>
#include<sstream>
#include<iostream>
#include<string>
#include<unordered_set>
using namespace std;
//文件读取操作的类

void saveToFile(BPlusTree& tree);
void readFromFile(BPlusTree& tree, string filename);

string ptrToString(interiorNode* node);

//new_info格式
//000001011 1100112101
//根据所提供的信息在覆盖写信息（modify或者是delete）
void overwrite(string new_info, int offpos, int size, fstream& file);
//在文件后面添加新的信息，返回的应当是待添加内容的偏移量
int appendwrite(string new_info, int size, fstream& file);
//从根据offPos一次去文件中读取出若干个<pos key value> 对,并组成string一次性返回
string readRawData(int offpos, int numbers, fstream& file);
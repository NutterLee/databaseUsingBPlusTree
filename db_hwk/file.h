#pragma once
#include"BPlusTree.h"
#include<fstream>
#include<queue>
#include<sstream>
#include<iostream>
#include<string>
#include<unordered_set>
using namespace std;
//�ļ���ȡ��������

void saveToFile(BPlusTree& tree);
void readFromFile(BPlusTree& tree, string filename);

string ptrToString(interiorNode* node);

//new_info��ʽ
//000001011 1100112101
//�������ṩ����Ϣ�ڸ���д��Ϣ��modify������delete��
void overwrite(string new_info, int offpos, int size, fstream& file);
//���ļ���������µ���Ϣ�����ص�Ӧ���Ǵ�������ݵ�ƫ����
int appendwrite(string new_info, int size, fstream& file);
//�Ӹ���offPosһ��ȥ�ļ��ж�ȡ�����ɸ�<pos key value> ��,�����stringһ���Է���
string readRawData(int offpos, int numbers, fstream& file);
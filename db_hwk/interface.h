#pragma once
#include"file.h"
#include "cache.h"
//˵����
//���������û������ݿ�ӿ�ͷ�ļ�
//testʱҲ���ô˴��Ľӿ�

class database
{
public:	
	~database();
	void db_open();
	void db_close();
	//��key�м����ֵ�ԣ���Ӧ�����Ѵ��ڵ�
	bool db_store(int key, int vvalue);
	//��ѯ��������db���ҳ�key����Ӧ��ֵ
	int db_fetch(int key);
	//���db
	void db_clear();
	//�޸�db��һ����ֵ��
	bool db_modify(int key, int value);
	//ɾ��db�еļ�ֵ��
	bool db_delete(int key);

private:
	//��write_cache�Ķ���д�뵽data.dat�У����趨����B+���е�ƫ����/ɾ���˽ڵ�
	//ָ���Ϊ ��remove����add����modify������
	//����remove�� value��offPos��ֵ�趨Ϊ-1
	//����add�� offPosӦ����-1
	void writeToFile(int key,int value, int offPos,string operation,fstream& file);
	//����ƫ������data.dat��ȡ�����ݣ�һ��ȡһ��Ҷ�ӽڵ�Ĵ�С�����ݣ����浽��������
	//�ڶ���������һ��ȡ���ļ�ֵ����Ŀ
	void readFromData(int offPos, int nums);
	//���ж��Ƿ��б�Ҫ����refresh
	void refreshWriteCache();
	//ǿ��ˢ��д����
	void refreshWriteCache_force();
	readBuffer* inner_read_buffer;
	writeBuffer* inner_write_buffer;
	BPlusTree* inner_tree;
};
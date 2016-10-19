#pragma once
#include "Public.h"

typedef unsigned char byte;
typedef unsigned int word;

const int Nr = 10;  // AES-128��Ҫ 10 �ּ���  
const int Nk = 4;   // Nk ��ʾ������Կ�� word ���� 

class AES_128
{
public:
	SINGLE_INSTANCE(AES_128)
	AES_128(){}
	~AES_128(){}

	//������Կ
	bool Set_Password(const string& pwd);
	//����
	string DeCrypt(const string& context);
	//����
	string EnCrypt(const string& context);
private:
	word password[4 * (Nr + 1)];
};
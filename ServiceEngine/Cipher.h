#pragma once
#include "Public.h"

typedef unsigned char byte;
typedef unsigned int word;

const int Nr = 10;  // AES-128需要 10 轮加密  
const int Nk = 4;   // Nk 表示输入密钥的 word 个数 

class AES_128
{
public:
	SINGLE_INSTANCE(AES_128)
	AES_128(){}
	~AES_128(){}

	//设置秘钥
	bool Set_Password(const string& pwd);
	//解密
	string DeCrypt(const string& context);
	//加密
	string EnCrypt(const string& context);
private:
	word password[4 * (Nr + 1)];
};
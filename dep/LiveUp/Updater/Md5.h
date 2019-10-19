#pragma once

#include <string>

class Md5
{
public:
	typedef struct {
		unsigned int count[2];
		unsigned int state[4];
		unsigned char buffer[64];
	}MD5_CTX;

public:
	static void Init(MD5_CTX *context);
	static void Update(MD5_CTX *context, unsigned char *input, unsigned int inputlen);
	static void Final(MD5_CTX *context, unsigned char digest[16]);
	static void Transform(unsigned int state[4], unsigned char block[64]);
	static void Encode(unsigned char *output, unsigned int *input, unsigned int len);
	static void Decode(unsigned int *output, unsigned char *input, unsigned int len);

	static std::string CalcFile(const char* szFileName);
};


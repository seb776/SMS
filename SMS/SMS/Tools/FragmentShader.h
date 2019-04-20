#pragma once

class FragmentShader
{
private:
	char *_shaderCode;
public:
	FragmentShader();


	bool Load(const char code[]);
	unsigned int Run();
	

	~FragmentShader();
};
#pragma once

class FragmentShader
{
private:
	char _shaderCode[2048];
public:
	FragmentShader();


	bool Load(const char *path, bool isPath = false);
	unsigned int Run();
	

	~FragmentShader();
};
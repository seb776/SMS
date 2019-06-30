#pragma once

#include "GLHelper.h"

struct ShaderParameters
{
	GLuint SourceTexture;
	GLuint DestinationTexture;
	float CurrentTime; // Since Composition start
	float DeltaTime; // Since last frame
	float FrameCurrentTime; // Since frame start
	float *SoundLevels; // Fake arrays of values representing beat => no FFT yet
	unsigned int SoundLevelsCount;
};

class FragmentShader
{
private:
	char *_shaderCode;
public:
	FragmentShader();
	~FragmentShader();

	GLuint _shaderId;
	GLuint _programId;

	bool Load(const char code[]);
	
	void Render(ShaderParameters parameters);
};


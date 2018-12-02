#ifndef __ModulePrograms_h__
#define __ModulePrograms_h__

#include "Module.h"
#include "GL/glew.h"

class ModulePrograms : public Module
{
public:
	ModulePrograms();
	~ModulePrograms();

	bool Init() override;
	bool CleanUp() override;

	GLuint LoadShader(const char* vsPath, const char* fsPath);

private:
	char* LoadFile(const char* file_name);
	bool  Compile(unsigned id, char* data);

public:
	GLuint def_program;
	GLuint tex_program;
};

#endif
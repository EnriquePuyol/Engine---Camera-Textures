#include "ModulePrograms.h"
#include "Application.h"
#include "ModuleUI.h"

#include "GL/glew.h"

ModulePrograms::ModulePrograms()
{
}

ModulePrograms::~ModulePrograms()
{
}

bool ModulePrograms::Init()
{
	//default_program = LoadShader("shaders/default.vs", "shaders/default.fs");

	programs[DEFAULT] = LoadShader("shaders/default.vs", "shaders/default.fs");
	programs[FLAT]	  = LoadShader("shaders/flat.vs", "shaders/flat.fs");
	programs[GOURAUD] = LoadShader("shaders/gouraud.vs", "shaders/gouraud.fs");
	programs[PHONG]   = LoadShader("shaders/phong.vs", "shaders/phong.fs");
	programs[BLINN]   = LoadShader("shaders/blinn.vs", "shaders/blinn.fs");

	return true;
}

bool ModulePrograms::CleanUp()
{
	if (default_program != 0)
	{
		glDeleteProgram(default_program);
	}

	return true;
}

bool ModulePrograms::Compile(unsigned id, char* data)
{
	glShaderSource(id, 1, &data, 0);
	glCompileShader(id);

	int res = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &res);

	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);

			glGetShaderInfoLog(id, len, &written, info);

			//LOG("Log Info: %s", info);
			App->ui->uiConsole->console.AddLog("Log Info: %s", info);

			free(info);
		}

		return false;
	}

	return true;
}

GLuint ModulePrograms::LoadShader(const char * vsPath, const char * fsPath)
{
	unsigned vertex_id = glCreateShader(GL_VERTEX_SHADER);
	unsigned fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

	char* vertex_data = LoadFile(vsPath);
	char* fragment_data = LoadFile(fsPath);

	bool ok = Compile(vertex_id, vertex_data) && Compile(fragment_id, fragment_data);

	free(vertex_data);
	free(fragment_data);

	GLuint program = 0;

	if (ok)
	{
		program = glCreateProgram();

		glAttachShader(program, vertex_id);
		glAttachShader(program, fragment_id);

		glLinkProgram(program);

		int len = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);

			glGetProgramInfoLog(program, len, &written, info);

			//LOG("Program Log Info: %s", info);
			App->ui->uiConsole->console.AddLog("Program Log Info: %s", info);

			free(info);
		}

	}

	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);

	return program;
}

char* ModulePrograms::LoadFile(const char* file_name)
{
	char* data = nullptr;

	FILE* file = 0;
	fopen_s(&file, file_name, "rb");

	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		data = (char*)malloc(size + 1);

		fread(data, 1, size, file);
		data[size] = 0;

		fclose(file);
	}

	return data;
}

#include "Globals.h"
#include "ModuleTextures.h"
#include "Application.h"
#include "ModuleUI.h"

#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilut.h>

using namespace std;

ModuleTextures::ModuleTextures()
{
}

// Destructor
ModuleTextures::~ModuleTextures()
{
	
}

// Called before render is available
bool ModuleTextures::Init()
{
	ilInit();
	iluInit();
	ilutInit();

	return true;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	return true;
}

// Load new texture from file path
unsigned ModuleTextures::Load(const char* path, bool mipmaps)
{
	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (ilLoadImage(path))
	{
		GLuint textureId = 0;
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		int channels = ilGetInteger(IL_IMAGE_CHANNELS);
		if (channels == 3)
		{
			ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		}
		else if (channels == 4)
		{
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		}

		ILubyte* data = ilGetData();
		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		ilDeleteImages(1, &imageId);

		glBindTexture(GL_TEXTURE_2D, 0);

		App->ui->uiConsole->console.AddLog("Texture loaded correctly!");

		return textureId;
	}

	return 0;
}

void ModuleTextures::Unload(unsigned id)
{
	if (id != 0)
	{
		glDeleteTextures(1, &id);
	}
}
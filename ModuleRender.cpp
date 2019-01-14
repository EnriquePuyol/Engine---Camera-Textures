#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModulePrograms.h"
#include "ModuleCameraEditor.h"
#include "ModuleModelLoader.h"
#include "ModuleDebugDraw.h"
#include "ModuleUI.h"

#include "debugdraw.h"
#include "SDL.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	//App->ui->uiConsole->console.AddLog("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->window->window);

	glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);

	int width, height;
	SDL_GetWindowSize(App->window->window, &width, &height);
	glViewport(0, 0, width, height);

	tri_model = math::float4x4::identity;

	CreateSceneImage();

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GetWindowSize(App->window->window, &App->window->width, &App->window->height);
	glViewport(0, 0, App->window->width, App->window->height);
	
	App->camera->UpdateFoV(App->window->width, App->window->height);
	for (list<ComponentCamera*>::iterator it = cameras.begin(); it != cameras.end(); ++it)
		(*it)->UpdateFrustum();

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	RenderEditorCamera();
	RenderGameCameras();

	/*glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(App->programs->tex_program);

	glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program, "model"), 1, GL_TRUE, &tri_model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program, "view"), 1, GL_TRUE, &App->camera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program, "proj"), 1, GL_TRUE, &App->camera->proj[0][0]);

	//Todo: Quitar esto
	if (App->modelLoader->scene != NULL)
	{

		for (unsigned i = 0; i < App->modelLoader->scene->mNumMeshes; i++)
		{
			unsigned Vbo = App->modelLoader->vbo[i];
			unsigned Ibo = App->modelLoader->ibo[i];
			unsigned numVerticesMesh = App->modelLoader->numVerticesMesh[i];
			unsigned numIndexesMesh = App->modelLoader->numIndexesMesh[i];

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, App->modelLoader->materials[App->modelLoader->textures[i]]);
			glUniform1i(glGetUniformLocation(App->programs->tex_program, "texture0"), 0);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, Vbo);
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
			);
			glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)(sizeof(float) * 3 * numVerticesMesh)
			);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibo);
			glDrawElements(GL_TRIANGLES, numIndexesMesh, GL_UNSIGNED_INT, nullptr);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

		}

	}
	glUseProgram(0);

	DrawCoords();

	glUseProgram(0);*/

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	App->ui->Draw();
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	//App->ui->uiConsole->console.AddLog("Destroying renderer");
	SDL_GL_DeleteContext(context);
	//Destroy window

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	App->window->width = width;
	App->window->height = height;
	glViewport(0, 0, width, height);
	// ToDo: Change FoV to take Scene window size
	App->camera->UpdateFoV(width, height);
	CreateSceneImage();
}

void ModuleRender::RenderEditorCamera()
{
	GenerateTexture(&(App->camera->fboSet));

	glBindFramebuffer(GL_FRAMEBUFFER, App->camera->fboSet.fbo);
	glViewport(0, 0, App->camera->fboSet.fb_width, App->camera->fboSet.fb_height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (list<ComponentMesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		math::AABB* aabb = (*it)->owner->boundingBox->aabb;
		if ((*it)->active && (*it)->owner->enable && App->camera->frustum.Intersects(*aabb))
		{
			MeshData meshData = (*it)->meshData;
			if (meshData.numVertices > 0) {

				if (meshData.type == Vao) {

					glBindVertexArray(meshData.vao);
					glDrawElements(GL_TRIANGLES, meshData.numFaces * 3, GL_UNSIGNED_INT, NULL);
					glBindVertexArray(0);

				}
				else
				{
					glUseProgram(App->programs->tex_program);

					glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program,
						"model"), 1, GL_TRUE, &(*it)->owner->transform->model[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program,
						"view"), 1, GL_TRUE, &App->camera->view[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program,
						"proj"), 1, GL_TRUE, &App->camera->proj[0][0]);


					glUniform3fv(glGetUniformLocation(App->programs->tex_program,
						"viewPosition"), 1, &App->camera->frustum.pos[0]);

					GLint drawText = glGetUniformLocation(App->programs->tex_program, "drawTexture");
					GLint color0 = glGetUniformLocation(App->programs->tex_program, "color0");

					if ((*it)->hasTexture) glUniform1i(drawText, 1);
					else {
						glUniform1i(drawText, 0);
						float color[4] = { 0.6, 1, 0.6, 1 };
						glUniform4fv(color0, 1, color);
					}

					unsigned vboActual = (*it)->meshData.vbo;
					unsigned numVerticesActual = meshData.numVertices;
					unsigned numIndexesActual = meshData.numIndexesMesh;

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, (*it)->meshData.materialIndex);
					glUniform1i(glGetUniformLocation(App->programs->tex_program, "texture0"), 0);

					glEnableVertexAttribArray(0);
					glEnableVertexAttribArray(1);
					glEnableVertexAttribArray(2);
					glBindBuffer(GL_ARRAY_BUFFER, vboActual);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * numVerticesActual));
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)((sizeof(float) * 3 + sizeof(float) * 2)* numVerticesActual));

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it)->meshData.ibo);
					glDrawElements(GL_TRIANGLES, numIndexesActual, GL_UNSIGNED_INT, nullptr);

					glDisableVertexAttribArray(0);
					glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(2);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindTexture(GL_TEXTURE_2D, 0);

					glUseProgram(0);
				}
			}
		}
	}
	if(showGrid)
		DrawCoords();

	App->debugDraw->Draw(App->camera->fboSet.fbo, App->camera->fboSet.fb_width, App->camera->fboSet.fb_height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModuleRender::RenderGameCameras()
{
	for (list<ComponentCamera*>::iterator cam = cameras.begin(); cam != cameras.end(); ++cam)
	{
		(*cam)->w = App->window->width;
		(*cam)->h = App->window->height;

		GenerateTexture(&(*cam)->fboSet);

		glBindFramebuffer(GL_FRAMEBUFFER, (*cam)->fboSet.fbo);
		glViewport(0, 0, (*cam)->fboSet.fb_width, (*cam)->fboSet.fb_height);
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (list<ComponentMesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{
			math::AABB* aabb = (*it)->owner->boundingBox->aabb;
			if ((*it)->active && (*it)->owner->enable && (*cam)->frustum.Intersects(*aabb))
				RenderMesh((*it), (*cam));
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void ModuleRender::RenderMesh(ComponentMesh * meshComp, ComponentCamera * cameraComp)
{
	MeshData meshData = meshComp->meshData;

	if (meshData.numVertices > 0) 
	{
		if (meshData.type == Vao) 
		{
			glBindVertexArray(meshData.vao);
			glDrawElements(GL_TRIANGLES, meshData.numFaces * 3, GL_UNSIGNED_INT, NULL);
			glBindVertexArray(0);
		}
		else 
		{
			glUseProgram(App->programs->tex_program);

			glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program,
				"model"), 1, GL_TRUE, &meshComp->owner->transform->model[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program,
				"view"), 1, GL_TRUE, &cameraComp->view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program,
				"proj"), 1, GL_TRUE, &cameraComp->proj[0][0]);

			glUniform3fv(glGetUniformLocation(App->programs->tex_program,
				"viewPosition"), 1, &cameraComp->frustum.pos[0]);

			GLint drawText = glGetUniformLocation(App->programs->tex_program, "drawTexture");
			GLint color0 = glGetUniformLocation(App->programs->tex_program, "color0");

			glUniform1i(drawText, 1);

			unsigned vboActual = meshComp->meshData.vbo;
			unsigned numVerticesActual = meshData.numVertices;
			unsigned numIndexesActual = meshData.numIndexesMesh;

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, meshComp->meshData.materialIndex);
			glUniform1i(glGetUniformLocation(App->programs->tex_program, "texture0"), 0);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, vboActual);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * numVerticesActual));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)((sizeof(float) * 3 + sizeof(float) * 2)* numVerticesActual));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshComp->meshData.ibo);
			glDrawElements(GL_TRIANGLES, numIndexesActual, GL_UNSIGNED_INT, nullptr);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glUseProgram(0);
		}
	}
}

void ModuleRender::GenerateTexture(FBOset* fboSet)
{
	int w = App->window->width;
	int h = App->window->height;

	if (w != fboSet->fb_width || h != fboSet->fb_height)
	{
		if (fboSet->fb_tex != 0)
		{
			glDeleteTextures(1, &(fboSet->fb_tex));
		}
		glGenFramebuffers(1, &(fboSet->fbo));

		glBindFramebuffer(GL_FRAMEBUFFER, fboSet->fbo);
		glGenTextures(1, &(fboSet->fb_tex));
		glBindTexture(GL_TEXTURE_2D, fboSet->fb_tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenRenderbuffers(1, &(fboSet->fb_depth));
		glBindRenderbuffer(GL_RENDERBUFFER, fboSet->fb_depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboSet->fb_depth);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboSet->fb_tex, 0);

		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		

		fboSet->fb_width = w;
		fboSet->fb_height = h;
	}
}

void ModuleRender::CreateSceneImage()
{
	glDeleteFramebuffers(1, &App->camera->fboSet.fbo);
	glDeleteTextures(1, &App->camera->fboSet.fb_tex);
	glDeleteRenderbuffers(1, &rbo);

	glGenFramebuffers(1, &App->camera->fboSet.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, App->camera->fboSet.fbo);

	glGenTextures(1, &App->camera->fboSet.fb_tex);
	glBindTexture(GL_TEXTURE_2D, App->camera->fboSet.fb_tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->window->width, App->window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, App->camera->fboSet.fb_tex, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->width, App->window->height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG("Error: Framebuffer issue");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void ModuleRender::DrawCoords()
{
	dd::xzSquareGrid(-1000.0f, 1000.0f, 0.0f, 1.0f, math::float3(0.65f, 0.65f, 0.65f));
}
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModulePrograms.h"
#include "ModuleCameraEditor.h"
#include "ModuleModelLoader.h"
#include "ModuleUI.h"

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
	//LOG("Creating Renderer context");
	App->ui->console.AddLog("Creating Renderer context");

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

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int width, height;
	SDL_GetWindowSize(App->window->window, &width, &height);
	glViewport(0, 0, width, height);
	App->camera->UpdateFoV(width,height);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	glUseProgram(App->programs->tex_program);

	glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program, "model"), 1, GL_TRUE, &tri_model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program, "view"), 1, GL_TRUE, &App->camera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programs->tex_program, "proj"), 1, GL_TRUE, &App->camera->proj[0][0]);

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
	glUseProgram(0);

	DrawCoords();

	glUseProgram(0);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (App->ui->io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	//LOG("Destroying renderer");
	App->ui->console.AddLog("Destroying renderer");
	SDL_GL_DeleteContext(context);
	//Destroy window

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
	App->camera->UpdateFoV(width, height);
}

void ModuleRender::DrawCoords()
{
	glUseProgram(App->programs->def_program);

	glUniformMatrix4fv(glGetUniformLocation(App->programs->def_program, "model"), 1, GL_TRUE, &tri_model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programs->def_program, "view"), 1, GL_TRUE, &App->camera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programs->def_program, "proj"), 1, GL_TRUE, &App->camera->proj[0][0]);

	glLineWidth(1.0f);
	int grid = glGetUniformLocation(App->programs->def_program, "newColor");
	float white[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glUniform4fv(grid, 1, white);

	glBegin(GL_LINES);

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();

	glLineWidth(3.0f);

	// red X
	int xAxis = glGetUniformLocation(App->programs->def_program, "newColor");
	float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glUniform4fv(xAxis, 1, red);

	glBegin(GL_LINES);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	glEnd();

	// green Y
	int yAxis = glGetUniformLocation(App->programs->def_program, "newColor");
	float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glUniform4fv(xAxis, 1, green);

	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	glEnd();

	// blue Z
	int zAxis = glGetUniformLocation(App->programs->def_program, "newColor");
	float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glUniform4fv(xAxis, 1, blue);

	glBegin(GL_LINES);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();

	glLineWidth(1.0f);
}


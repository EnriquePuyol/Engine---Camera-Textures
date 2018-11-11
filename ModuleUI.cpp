#include "ModuleUI.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

static void MainBar();

ModuleUI::ModuleUI()
{
}


ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Init()
{
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Window

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Setup style
	ImGui::StyleColorsDark();

	return true;
}

update_status ModuleUI::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update()
{
	MainBar();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleUI::Events(SDL_Event & event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void MainBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Hello")) {}
			if (ImGui::MenuItem("Exit")) { App->input->quit = true; }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::BeginMenu("Resolution"))
			{
				if (ImGui::MenuItem("Fullscreen")) {}
				if (ImGui::MenuItem("1920x1080")) { App->window->SetResolution(1920, 1080); }
				if(ImGui::MenuItem("1280x720")) { App->window->SetResolution(1280, 720); }
				if (ImGui::MenuItem("960x540")) { App->window->SetResolution(960, 540); }
				if (ImGui::Checkbox("Borderless", &App->window->borderless)) {}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

#include "ModuleUI.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

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
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup style
	ImGui::StyleColorsDark();

	uiWindows.push_back(uiAbout = new UI_About("About"));
	uiWindows.push_back(uiPerformance = new UI_Performance("Performance"));
	uiWindows.push_back(uiConsole = new UI_Console("Console"));

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

	/*if (uiAbout->active)
		uiAbout->Draw();
	if (uiConsole->active)
		uiConsole->Draw();
	if (uiPerformance->active)
		uiPerformance->Draw();*/

	for (list<UI*>::iterator it = uiWindows.begin(); it != uiWindows.end(); ++it)
	{
		if ((*it)->getActive())
		{
			ImGui::SetNextWindowSizeConstraints({ 10,10 }, { (float)App->window->width, (float)App->window->height });
			(*it)->Draw();
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	ImGui::EndFrame();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleUI::Events(SDL_Event & event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void ModuleUI::MainBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Hello")) {}
			if (ImGui::MenuItem("Exit")) { App->input->quit = true; }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo")) {}
			if (ImGui::MenuItem("Redo")) {}
			if (ImGui::MenuItem("Preferences")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::BeginMenu("Display"))
			{
				if (ImGui::MenuItem("1920x1080"))	{ App->window->SetResolution(1920, 1080); }
				if(ImGui::MenuItem("1280x720"))		{ App->window->SetResolution(1280, 720); }
				if (ImGui::MenuItem("960x540"))		{ App->window->SetResolution(960, 540); }
				if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen)) { App->window->SetFullScreen(); }
				if (ImGui::Checkbox("Borderless", &App->window->borderless)) { App->window->SetBorderless(); }

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::Checkbox("Console", &uiConsole->active)) {}
				if (ImGui::Checkbox("Performance", &uiPerformance->active)) {}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			if (ImGui::MenuItem("SAG Engine...")) { uiAbout->active = true; }
			if (ImGui::MenuItem("Github...")) { ShellExecute(0, 0, "https://github.com/EnriquePuyol/Engine---Camera-Textures", 0, 0, SW_SHOW); }

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

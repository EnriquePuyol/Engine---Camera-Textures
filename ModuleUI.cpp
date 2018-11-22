#include "ModuleUI.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

static void MainBar();
static void Info();
static void Console();
static void Performance();

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

	logMSIterator = 0;
	logFPSIterator = 0;
	lastFrameTime = SDL_GetTicks();
	lastSecondTime = SDL_GetTicks();

	fps_log = new float[50];
	ms_log = new float[50];

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

	if (showInfo)
		Info();
	if (showConsole)
		Console();
	if (showPerformace)
		Performance();

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
				if (ImGui::Checkbox("Console", &App->ui->showConsole)) {}
				if (ImGui::Checkbox("Performance", &App->ui->showPerformace)) {}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			if (ImGui::MenuItem("SAG Engine...")) { App->ui->showInfo = true; }
			if (ImGui::MenuItem("Github...")) { ShellExecute(0, 0, "https://github.com/EnriquePuyol/Engine---Camera-Textures", 0, 0, SW_SHOW); }

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Info()
{
	ImGui::SetNextWindowPos(ImVec2(App->window->width/2 - 200, App->window->height/2 - 200));
	ImGui::Begin("About", &App->ui->showInfo);

	ImGui::Text(TITLE);
	ImGui::Text("Version: 1.0");
	ImGui::Text("3D game engine developed by Enrique Puyol Martín");
	ImGui::Text("Libraries: ");
	ImGui::Text("	- glew-2.1.0");
	ImGui::Text("	- Imgui");
	ImGui::Text("	- DevIL");
	ImGui::Text("	- OpenGL 3.0");
	ImGui::Text("	- MathGeoLib");
	ImGui::Text("	- SDL 2.0");
	ImGui::Text("MIT License Copyright (c) [2018] [Enrique Puyol]");

	ImGui::End();
}

void Console()
{
	ImGui::SetNextWindowPos(ImVec2(0, App->window->height - 100), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(App->window->width, 100), ImGuiCond_Always);

	App->ui->console.Draw("Console");

}

void Performance()
{
	ImGui::SetNextWindowPos(ImVec2(App->window->width - 300, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, App->window->height - 120), ImGuiCond_Always);

	ImGui::Begin("Performance", &App->ui->showPerformace);

	ImGui::Text("App Lifetime = %d seconds", SDL_GetTicks() / 1000);
	char* title = new char[50];
	App->ui->updateFramerates();
	sprintf_s(title, 50, "Framerate %.1f", App->ui->fps_log[App->ui->logFPSIterator]);
	ImGui::PlotHistogram("", App->ui->fps_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));
	sprintf_s(title, 50, "Milliseconds %.1f", App->ui->ms_log[App->ui->logMSIterator]);
	ImGui::PlotHistogram("", App->ui->ms_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));

	ImGui::End();
}

void ModuleUI::updateFramerates() {
	double timeElapsed = SDL_GetTicks() - lastSecondTime;
	//fps calculation
	lastSecondTime = SDL_GetTicks();
	fps_log[logFPSIterator] = 1000 / timeElapsed;
	++logFPSIterator;
	if (logFPSIterator > 49) logFPSIterator = 0;
	//ms calculation
	ms_log[logMSIterator] = timeElapsed;
	lastFrameTime = SDL_GetTicks();
	//iterator increase
	++logMSIterator;
	if (logMSIterator > 49) logMSIterator = 0;
}

void ModuleUI::ResizeUI()
{



}

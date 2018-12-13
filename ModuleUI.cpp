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
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Window

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup style
	ImGui::StyleColorsDark();

	uiWindows.push_back(uiAbout = new UI_About("About"));
	uiWindows.push_back(uiPerformance = new UI_Performance("Performance"));
	uiWindows.push_back(uiConsole = new UI_Console("Console"));
	uiWindows.push_back(uiScene = new UI_Scene("Scene"));
	uiWindows.push_back(uiHierarchy = new UI_Hierarchy("Hierarchy"));
	uiWindows.push_back(uiInspector = new UI_Inspector("Inspector"));

	return true;
}

update_status ModuleUI::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	Docking();

	return UPDATE_CONTINUE;
}

void ModuleUI::Draw()
{
	MainBar();

	// Ventana de ejemplo de ImGui
	// ImGui::ShowDemoWindow();

	for (list<UI*>::iterator it = uiWindows.begin(); it != uiWindows.end(); ++it)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.24f, 0.24f, 0.24f, 1));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 0.6f, 0.2f, 1));
		if ((*it)->getActive())
		{
			ImGui::SetNextWindowSizeConstraints({ 10,10 }, { (float)App->window->width, (float)App->window->height });
			(*it)->Draw();
		}
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

update_status ModuleUI::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	//ImGui::EndFrame();

	for (std::list<UI*>::iterator it = uiWindows.begin(); it != uiWindows.end(); ++it)
	{
		RELEASE((*it));
	}

	uiWindows.clear();

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
				DrawDisplay();

				ImGui::Separator();
				ImGui::Spacing();
				if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen)) { App->window->SetFullScreen(); }
				ImGui::SameLine(110.0f);
				if (ImGui::Checkbox("Borderless", &App->window->borderless)) { App->window->SetBorderless(); }

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::Checkbox("Scene", &uiScene->active)) {}
				ImGui::Separator();
				if (ImGui::Checkbox("Hierarchy", &uiHierarchy->active)) {}
				if (ImGui::Checkbox("Inspector", &uiScene->active)) {}
				ImGui::Separator();
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

void ModuleUI::DrawDisplay()
{
	int w = App->window->width;
	int h = App->window->height;

	if (w == 1920 && h == 1080)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		if (ImGui::MenuItem("1920 x 1080")) { App->window->SetResolution(1920, 1080); }
		ImGui::PopStyleColor();
	}
	else
		if (ImGui::MenuItem("1920 x 1080")) { App->window->SetResolution(1920, 1080); }

	if (w == 1920 && h == 864)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		if (ImGui::MenuItem("1920 x 864")) { App->window->SetResolution(1920, 864); }
		ImGui::PopStyleColor();
	}
	else
		if (ImGui::MenuItem("1920 x 864")) { App->window->SetResolution(1920, 864); }

	if (w == 1920 && h == 720)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		if (ImGui::MenuItem("1920 x 720")) { App->window->SetResolution(1920, 720); }
		ImGui::PopStyleColor();
	}
	else
		if (ImGui::MenuItem("1920 x 720")) { App->window->SetResolution(1920, 720); }

	if (w == 1536 && h == 864)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		if (ImGui::MenuItem("1536 x 864")) { App->window->SetResolution(1536, 864); }
		ImGui::PopStyleColor();
	}
	else
		if (ImGui::MenuItem("1536 x 864")) { App->window->SetResolution(1536, 864); }

	if (w == 1536 && h == 720)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		if (ImGui::MenuItem("1536 x 720")) { App->window->SetResolution(1536, 864); }
		ImGui::PopStyleColor();
	}
	else
		if (ImGui::MenuItem("1536 x 720")) { App->window->SetResolution(1536, 720); }

	if (w == 1280 && h == 720)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		if (ImGui::MenuItem("1280 x 720")) { App->window->SetResolution(1280, 720); }
		ImGui::PopStyleColor();
	}
	else
		if (ImGui::MenuItem("1280 x 720")) { App->window->SetResolution(1280, 720); }

	if (w == 1280 && h == 540)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		if (ImGui::MenuItem("1280 x 540")) { App->window->SetResolution(1280, 720); }
		ImGui::PopStyleColor();
	}
	else
		if (ImGui::MenuItem("1280 x 540")) { App->window->SetResolution(1280, 540); }

	if (w == 960 && h == 720)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		if (ImGui::MenuItem("960 x 720")) { App->window->SetResolution(960, 720); }
		ImGui::PopStyleColor();
	}
	else
		if (ImGui::MenuItem("960 x 720")) { App->window->SetResolution(960, 720); }

	if (w == 960 && h == 540)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		if (ImGui::MenuItem("960 x 540")) { App->window->SetResolution(960, 540); }
		ImGui::PopStyleColor();
	}
	else
		if (ImGui::MenuItem("960 x 540")) { App->window->SetResolution(960, 540); }
}

void ModuleUI::Docking()
{
	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ (float)App->window->width, (float)App->window->height });
	
	ImGui::SetNextWindowBgAlpha(1.0f);

	//TODO: change this to a simple define
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_DockingPreview, ImVec4(1, 0, 0, 1));
	ImGui::Begin("DockSpace", 0, windowFlags);
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor();

	ImGuiID dockspaceId = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
}

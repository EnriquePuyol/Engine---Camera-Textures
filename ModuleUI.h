#ifndef __ModuleUI_h__
#define __ModuleUI_h__

#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL.h"

class ModuleUI : public Module
{
	struct ConsoleLog
	{
		ImGuiTextBuffer     Buf;
		bool                ScrollToBottom;

		void    Clear() { Buf.clear(); }

		void    AddLog(const char* fmt, ...) //IM_PRINTFARGS(2)
		{
			va_list args;
			va_start(args, fmt);
			Buf.appendf(fmt, args);
			Buf.appendf("\n", args);
			va_end(args);
			ScrollToBottom = true;
		}

		void    Draw(const char* title, bool* p_opened = NULL)
		{
			ImGui::Begin(title, p_opened);
			ImGui::TextUnformatted(Buf.begin());
			if (ScrollToBottom)
				ImGui::SetScrollHere(1.0f);
			ScrollToBottom = false;
			ImGui::End();
		}
	};

public:
	ModuleUI();
	~ModuleUI();
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Events(SDL_Event& event);

public:
	ImGuiIO io;
	ConsoleLog console;

	bool showInfo = false;
	bool showConsole = true;
	bool showPerformace = false;

};

#endif
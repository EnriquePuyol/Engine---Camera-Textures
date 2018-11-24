#ifndef __UI_Console_h__
#define __UI_Console_h__

#include "UI.h"
#include "Application.h"
#include "ModuleWindow.h"

class UI_Console : public UI
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
	UI_Console(char* name);
	~UI_Console();

	void Draw();

public:
	ConsoleLog console;
};

#endif // !__UI_Console_h__
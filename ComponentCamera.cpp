#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	type = Camera;

	if (App->scene->selectedGO->GetNumComponentsOfType(Transform) == 0)
		App->scene->selectedGO->AddComponent(Transform);
}

ComponentCamera::ComponentCamera(ComponentCamera* component)
{
	type = Camera;
	active = component->active;
	parent = component->parent;
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Update()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &renderedTexture);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->window->width, App->window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void ComponentCamera::CleanUp()
{

}

void ComponentCamera::Draw(int id)
{
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("CAMERA").x / 2);
	ImGui::Text("CAMERA");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("     X     ").x / 2);
	ImGui::PushID(id);
	if (ImGui::Button("X"))
	{
		Delete();
	}
	ImGui::PopID();

	const char* types[] = { "Primary", "Secondary", "Other" };
	ImGui::Text("Type: ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##hidelabel", GetCameraTypeToString(cameraType), ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < ARRAYSIZE(types); n++)
		{
			bool is_selected = (GetCameraTypeToString(cameraType) == types[n]);
			if (ImGui::Selectable(types[n], is_selected))
			{
				if (types[n] == "Primary")
				{
					App->ui->uiGame->primaryCamera = this;
				}
				SetCameraTypeFromString(types[n]);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
		}
		ImGui::EndCombo();
	}

}

char* ComponentCamera::GetCameraTypeToString(CameraType type)
{
	switch (type)
	{
	case Primary:
		return "Primary";
	case Secondary:
		return "Secondary";
	case Other:
		return "Other";
	}

	return "--none--";
}

void ComponentCamera::SetCameraTypeFromString(const char * type)
{
	if (type == "Primary")
		cameraType = Primary;
	else if (type == "Secondary")
		cameraType = Secondary;
	else if (type == "Other")
		cameraType = Other;
}

#include "ComponentLight.h"
#include "Application.h"
#include "ModuleScene.h"

#include <string>

ComponentLight::ComponentLight(GameObject* parent) : Component(parent)
{
	type = Light;

	if (App->scene->selectedGO->GetNumComponentsOfType(Transform) == 0)
		App->scene->selectedGO->AddComponent(Transform);
}

ComponentLight::ComponentLight(ComponentLight* component)
{
	type = Light;
	active = component->active;
	parent = component->parent;
}

ComponentLight::~ComponentLight()
{
}

void ComponentLight::Update()
{

}

void ComponentLight::CleanUp()
{

}

void ComponentLight::Draw(int id)
{
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("LIGHT").x / 2);
	ImGui::Text("LIGHT");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("     X     ").x / 2);
	ImGui::PushID(id);
	if (ImGui::Button("X"))
	{
		Delete();
	}
	ImGui::PopID();

	const char* types[] = {"Ambient", "Point", "Directional", "SpotLight"};
	ImGui::Text("Light Type:");
	ImGui::PushID(id);
	if (ImGui::BeginCombo("##hidelabel", GetLightTypeToString(lightType), ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < ARRAYSIZE(types); n++)
		{
			bool is_selected = (GetLightTypeToString(lightType) == types[n]);
			if (ImGui::Selectable(types[n], is_selected))
				SetLightTypeFromString(types[n]);
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();
}

char* ComponentLight::GetLightTypeToString(LightType type)
{
	switch (type)
	{
	case Ambient:
		return "Ambient";
	case Point:
		return "Point";
	case Directional:
		return "Directional";
	case SpotLight:
		return "SpotLight";
	}

	return "--none--";
}

void ComponentLight::SetLightTypeFromString(const char * type)
{
	if (type == "Ambient")
		lightType = Ambient;
	else if (type == "Point")
		lightType = Point;
	else if (type == "Directional")
		lightType = Directional;
	else if (type == "SpotLight")
		lightType = SpotLight;
}

#include "ComponentLight.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRender.h"

#include <string>

ComponentLight::ComponentLight(GameObject* parent) : Component(parent)
{
	type = Light;

	uID = App->GenerateUUID();

	App->renderer->lights.push_back(this);
}

ComponentLight::ComponentLight(ComponentLight* component)
{
	type = Light;
	active = component->active;
	owner = component->owner;

	uID = component->uID;

	App->renderer->lights.push_back(this);
}

ComponentLight::~ComponentLight()
{
}

PreComponentReturn ComponentLight::PreUpdate()
{
	// Si borramos el componente quitamos la camara del Render
	if (nextPreReturn == COMP_DELETED)
	{
		for (list<ComponentLight*>::iterator it = App->renderer->lights.begin(); it != App->renderer->lights.end(); )
		{
			if ((*it) != this)
				++it;
			else
				App->renderer->lights.erase(it++);
		}
	}

	return nextPreReturn;
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
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::Text("Intensity: "); ImGui::SameLine();
	ImGui::DragFloat("##hidelabel", &intensity, 0.005f, 0.0, 10000);

	if(owner->showMetadata)
		ShowMetadata();

	ImGui::PopID();
}

void ComponentLight::ShowMetadata()
{
	ImGui::SeparatorCustom(50, ImGui::GetWindowWidth() - 100);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4, 0.4, 0.4, 1));
	ImGui::Text("uID:");
	ImGui::Text(uID);
	ImGui::PopStyleColor();
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

void ComponentLight::Save(System * system)
{
	system->StartObject();

	system->AddComponentType("compType", type);
	system->AddString("ownerUID", owner->uID);
	system->AddString("uID", uID);

	system->AddFloat("intensity", intensity);

	system->EndObject();
}

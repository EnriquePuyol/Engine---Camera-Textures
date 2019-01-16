#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "ComponentLight.h"
#include "ModulePrograms.h"
#include "GameObject.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent)
{
	type = Material;

	uID = App->GenerateUUID();
}

ComponentMaterial::ComponentMaterial(unsigned material)
{
	this->matIndex = material;

	uID = App->GenerateUUID();
}

ComponentMaterial::ComponentMaterial(ComponentMaterial * component)
{
	type = Material;
	active = component->active;
	owner = component->owner;

	uID = component->uID;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Render()
{
	unsigned program = App->programs->programs[mat];

	switch (mat)
	{
	case DEFAULT:
		glBindTexture(GL_TEXTURE_2D, matIndex);
		glUniform1i(glGetUniformLocation(App->programs->default_program, "texture0"), 0);
		
		break;
	case BLINN:
		GameObject* obj = App->renderer->lights.front()->owner;

		glUniform3fv(glGetUniformLocation(program, "light_pos"), 1, (float*)&obj->transform->GetWorldPosition());

		glActiveTexture(GL_TEXTURE0);

		glUniform4f(glGetUniformLocation(program, "diffuse_color"), material.diffuse_color.x, material.diffuse_color.y, material.diffuse_color.z, 1.0f);
		glUniform4f(glGetUniformLocation(program, "specular_color"), material.specular_color.x, material.specular_color.y, material.specular_color.z, 1.0f);

		glUniform1f(glGetUniformLocation(program, "k_diffuse"), material.k_diffuse);
		glUniform1f(glGetUniformLocation(program, "k_specular"), material.k_specular);
		glUniform1f(glGetUniformLocation(program, "k_ambient"), material.k_ambient);
		glUniform1f(glGetUniformLocation(program, "shininess"), material.shininess);
		glUniform1f(glGetUniformLocation(program, "light_intensity"), ((ComponentLight*)obj->GetComponentOfType(Light))->intensity);

		glActiveTexture(GL_TEXTURE0);
		if (material.diffuse_map != 0u)
			glBindTexture(GL_TEXTURE_2D, material.diffuse_map);
		else
			glBindTexture(GL_TEXTURE_2D, App->renderer->defaultTexture);
		glUniform1i(glGetUniformLocation(program, "diffuse_map"), 0);

		glActiveTexture(GL_TEXTURE1);
		if(material.specular_map != 0u)
			glBindTexture(GL_TEXTURE_2D, material.specular_map);
		else
			glBindTexture(GL_TEXTURE_2D, App->renderer->defaultTexture);
		glUniform1i(glGetUniformLocation(program, "specular_map"), 1);

		glActiveTexture(GL_TEXTURE2);
		if(material.occlusion_map != 0u)
			glBindTexture(GL_TEXTURE_2D, material.occlusion_map);
		else
			glBindTexture(GL_TEXTURE_2D, App->renderer->defaultTexture);
		glUniform1i(glGetUniformLocation(program, "occlusion_map"), 2);

		//glDisable(GL_TEXTURE_2D);
		
		break;
	}
	// Lights render
	/*for (list<ComponentLight*>::iterator it = App->renderer->lights.begin(); it != App->renderer->lights.end(); ++it)
	{
		if ((*it)->owner->enable)
		{
			glUniform3fv(glGetUniformLocation(program, "light_pos"), 1, (const float*)&(*it)->owner->transform->position);
		}
		else
		{
			glUniform3fv(glGetUniformLocation(App->programs->default_program,
				"viewPosition"), 1, (const float*)&(*it)->owner->transform->position);
		}


		// Diffuse
		glUniform4fv(glGetUniformLocation(program, "material.diffuse_color"), 1, (GLfloat*)&material.diffuse_color);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material.diffuse_map != 0u ? material.diffuse_map : App->renderer->defaultTexture);
		glUniform1i(glGetUniformLocation(program, "material.diffuse_map"), 0);
		glDisable(GL_TEXTURE_2D);

		// Specular
		glUniform3fv(glGetUniformLocation(program, "material.specular_color"), 1, (GLfloat*)&material.specular_color);
		glUniform1fv(glGetUniformLocation(program, "material.shininess"), 1, (GLfloat*)&material.shininess);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material.specular_map != 0u ? material.specular_map : App->renderer->defaultTexture);
		glUniform1i(glGetUniformLocation(program, "material.specular_map"), 1);
		glDisable(GL_TEXTURE_2D);

		// Ambient
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, material.occlusion_map != 0u ? material.occlusion_map : App->renderer->defaultTexture);
		glUniform1i(glGetUniformLocation(program, "material.occlusion_map"), 2);
		glDisable(GL_TEXTURE_2D);

		// Emissive
		glUniform3fv(glGetUniformLocation(program, "material.emissive_color"), 1, (GLfloat*)&material.emissive_color);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, material.emissive_map != 0u ? material.emissive_map : App->renderer->defaultTexture);
		glUniform1i(glGetUniformLocation(program, "material.emissive_map"), 3);
		glDisable(GL_TEXTURE_2D);

		glUniform1fv(glGetUniformLocation(program, "material.k_ambient"), 1, (GLfloat*)&material.k_ambient);
		glUniform1fv(glGetUniformLocation(program, "material.k_diffuse"), 1, (GLfloat*)&material.k_diffuse);
		glUniform1fv(glGetUniformLocation(program, "material.k_specular"), 1, (GLfloat*)&material.k_specular);
	}*/
}

void ComponentMaterial::Draw(int id)
{
	ImGui::Spacing();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("MATERIAL").x / 2);
	ImGui::Text("MATERIAL");

	ImGui::PushID(id);
	if (!defaultMat)
	{
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("     X     ").x / 2);
		if (ImGui::Button("X"))
		{
			Delete();
		}
	}
	//ImGui::Text("ToDo: Material");
	char* program_names[TOTAL] = { "Default", "Flat", "Gouraud", "Phong", "Blinn" };
	ImGui::Text("Shader:"); ImGui::SameLine();
	//ImGui::PushID(id);
	if (ImGui::BeginCombo("##hidelabel", program_names[mat], ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < ARRAYSIZE(program_names); n++)
		{
			bool is_selected = (mat == n);
			if (ImGui::Selectable(program_names[n], is_selected))
				mat = n;
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::SetCursorPosX(10);
	ImGui::Text("Diffuse");
	ImGui::SeparatorCustom(0, ImGui::CalcTextSize("Diffuse").x + 20);

	ImGui::PushID(100);
	ImGui::Text("Path:"); ImGui::SameLine();
	ImGui::InputText("", diffusePath, ARRAYSIZE(diffusePath)); ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		material.diffuse_map = App->textures->Load(diffusePath, false);
	}
	ImGui::Image((ImTextureID)material.diffuse_map, ImVec2(80, 80), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
	ImGui::PopID();

	ImGui::ColorEdit4("D", (float*)&material.diffuse_color);
	ImGui::SliderFloat("K Diffuse", &material.k_diffuse, 0.0f, 1.0f);


	ImGui::SetCursorPosX(10);
	ImGui::Text("Specular");
	ImGui::SeparatorCustom(0, ImGui::CalcTextSize("Specular").x + 20);

	ImGui::PushID(101);
	ImGui::Text("Path:"); ImGui::SameLine();
	ImGui::InputText("", specularPath, ARRAYSIZE(specularPath)); ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		material.specular_map = App->textures->Load(specularPath, false);
	}
	ImGui::Image((ImTextureID)material.specular_map, ImVec2(80, 80), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
	ImGui::PopID();

	ImGui::ColorEdit3("S", (float*)&material.specular_color);
	ImGui::SliderFloat("K Specular", &material.k_specular, 0.0f, 1.0f);
	ImGui::SliderFloat("Shininess", &material.shininess, 8.0f, 128.0f);

	ImGui::SetCursorPosX(10);
	ImGui::Text("Ambient");
	ImGui::SeparatorCustom(0, ImGui::CalcTextSize("Ambient").x + 20);

	ImGui::PushID(102);
	ImGui::Text("Path:"); ImGui::SameLine();
	ImGui::InputText("", ambientPath, ARRAYSIZE(ambientPath)); ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		material.occlusion_map = App->textures->Load(ambientPath, false);
	}
	ImGui::Image((ImTextureID)material.occlusion_map, ImVec2(80, 80), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
	ImGui::PopID();

	ImGui::SliderFloat("K Ambient", &material.k_ambient, 0.0f, 1.0f);

	ImGui::SetCursorPosX(10);
	ImGui::Text("Emissive");
	ImGui::SeparatorCustom(0, ImGui::CalcTextSize("Emissive").x + 20);

	ImGui::PushID(103);
	ImGui::Text("Path:"); ImGui::SameLine();
	ImGui::InputText("", emissivePath, ARRAYSIZE(emissivePath)); ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		material.emissive_map = App->textures->Load(emissivePath, false);
	}
	ImGui::Image((ImTextureID)material.emissive_map, ImVec2(80, 80), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
	ImGui::PopID();

	ImGui::ColorEdit3("E", (float*)&material.emissive_color);

	if (owner->showMetadata)
		ShowMetadata();

	ImGui::PopID();
}

void ComponentMaterial::ShowMetadata()
{
	ImGui::SeparatorCustom(50, ImGui::GetWindowWidth() - 100);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4, 0.4, 0.4, 1));
	ImGui::Text("uID:");
	ImGui::Text(uID);
	ImGui::PopStyleColor();
}

void ComponentMaterial::Save(System * system)
{
	system->StartObject();

	system->AddComponentType("compType", type);
	system->AddString("ownerUID", owner->uID);
	system->AddString("uID", uID);

	system->AddUnsigned("matIndex", matIndex);
	system->AddUnsigned("mat", mat);
	system->AddString("diffusePath", diffusePath);
	system->AddString("specularPath", specularPath);
	system->AddString("ambientPath", ambientPath);
	system->AddString("emissivePath", emissivePath);

	system->AddBool("defaultMat", defaultMat);

	system->EndObject();
}

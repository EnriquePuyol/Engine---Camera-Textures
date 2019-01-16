#ifndef __ComponentMaterial_h__
#define __ComponentMaterial_h__

#include "Component.h"
#include "MathGeoLib.h"

struct MaterialData
{
	// Diffuse
	unsigned		diffuse_map = 0u;
	math::float4	diffuse_color = math::float4::one;
	float			k_diffuse = 0.5f;
	int				diffuse_width = 0;
	int				diffuse_height = 0;

	//Specular
	unsigned		specular_map = 0u;
	math::float3	specular_color = math::float3::zero;
	float			shininess = 64.0f;
	float			k_specular = 1.0f;
	int				specular_width = 0;
	int				specular_height = 0;

	// Ambient
	unsigned		occlusion_map = 0u;
	float			k_ambient = 0.5f;
	int				occlusion_width = 0;
	int				occlusion_height = 0;

	// Emissive
	unsigned		emissive_map = 0u;
	math::float3	emissive_color = math::float3::zero;
	int				emissive_width = 0;
	int				emissive_height = 0;
};

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* parent);
	ComponentMaterial(unsigned material);
	ComponentMaterial(ComponentMaterial* component);
	~ComponentMaterial();

	void Render();

	void Draw(int id) override;
	void ShowMetadata() override;

	void Save(System* system) override;

public:
	unsigned matIndex = 0;
	unsigned mat = 0;

	char diffusePath[100] = "";
	char specularPath[100] = "";
	char ambientPath[100] = "";
	char emissivePath[100] = "";

	MaterialData material;

	bool defaultMat = false;
};

#endif // !_ComponentMaterial_h__
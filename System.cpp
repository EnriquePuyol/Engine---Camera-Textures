#include "System.h"
#include "Component.h"
#include "Application.h"
#include "ModuleFileSystem.h"

System::System() {
	stringBuffer = new rapidjson::StringBuffer();
	writer = new rapidjson::PrettyWriter<rapidjson::StringBuffer>(*stringBuffer);
	writer->StartObject();
}

System::~System() {
	delete writer;
	delete stringBuffer;

	writer = nullptr;
	stringBuffer = nullptr;
}

void System::AddName(const char* name) {
	writer->String(name);
}

void System::AddComponentType(const char* name, Type componentType) {
	writer->String(name);

	switch (componentType) {
	case Type::Camera:
		writer->String("CAMERA");
		break;
	case Type::Material:
		writer->String("MATERIAL");
		break;
	case Type::Mesh:
		writer->String("MESH");
		break;
	case Type::Transform:
		writer->String("TRANSFORM");
		break;
	case Type::Light:
		writer->String("LIGHT");
		break;
	}

}

void System::AddInt(const char* name, int value) {
	writer->String(name);
	writer->Int(value);
}

void System::AddUnsigned(const char* name, unsigned value) {
	writer->String(name);
	writer->Uint(value);
}

void System::AddFloat(const char* name, float value) {
	writer->String(name);
	writer->Double(value);
}

void System::AddString(const char* name, const char* value) {
	writer->String(name);
	writer->String(value);
}

void System::AddBool(const char* name, bool value) {
	writer->String(name);
	writer->Bool(value);
}

void System::AddFloat3(const char* name, math::float3 value) {
	StartObject(name);
	writer->String("x");
	writer->Double(value.x);
	writer->String("y");
	writer->Double(value.y);
	writer->String("z");
	writer->Double(value.z);
	EndObject();
}

void System::AddFloat4(const char* name, math::float4 value) {
	StartObject(name);
	writer->String("x");
	writer->Double(value.x);
	writer->String("y");
	writer->Double(value.y);
	writer->String("z");
	writer->Double(value.z);
	writer->String("w");
	writer->Double(value.w);
	EndObject();
}

void System::AddQuat(const char* name, math::Quat value) {
	StartObject(name);
	writer->String("x");
	writer->Double(value.x);
	writer->String("y");
	writer->Double(value.y);
	writer->String("z");
	writer->Double(value.z);
	writer->String("w");
	writer->Double(value.w);
	EndObject();
}

Type System::GetComponentType(const char* name, rapidjson::Value& value) {
	const char* stringComponentType = value[name].GetString();

	if (strcmp(stringComponentType, "TRANSFORM") == 0) {
		return Type::Transform;
	}
	if (strcmp(stringComponentType, "MESH") == 0) {
		return Type::Mesh;
	}
	if (strcmp(stringComponentType, "MATERIAL") == 0) {
		return Type::Material;
	}
	if (strcmp(stringComponentType, "CAMERA") == 0) {
		return Type::Camera;
	}
}

int System::GetInt(const char* name, rapidjson::Value& value) {
	return value[name].GetInt();
}

unsigned System::GetUnsigned(const char* name, rapidjson::Value& value) {
	return value[name].GetUint();
}

float System::GetFloat(const char* name, rapidjson::Value& value) {
	return value[name].GetFloat();
}

const char* System::GetString(const char* name, rapidjson::Value& value) {
	return value[name].GetString();
}

bool System::GetBool(const char* name, rapidjson::Value& value) {
	return value[name].GetBool();
}

math::float3 System::GetFloat3(const char* name, rapidjson::Value& value) {
	math::float3 result = {
		value[name]["x"].GetFloat(),
		value[name]["y"].GetFloat(),
		value[name]["z"].GetFloat()
	};

	return result;
}

math::float4 System::GetFloat4(const char* name, rapidjson::Value& value) {
	math::float4 result = {
		value[name]["x"].GetFloat(),
		value[name]["y"].GetFloat(),
		value[name]["z"].GetFloat(),
		value[name]["w"].GetFloat()
	};

	return result;
}

math::Quat System::GetQuat(const char* name, rapidjson::Value& value) {
	math::Quat result = {
		value[name]["x"].GetFloat(),
		value[name]["y"].GetFloat(),
		value[name]["z"].GetFloat(),
		value[name]["w"].GetFloat()
	};

	return result;
}

void System::StartObject(const char* name) {
	writer->String(name);
	writer->StartObject();
}

void System::StartObject() {
	writer->StartObject();
}

void System::EndObject() {
	writer->EndObject();
}

void System::StartArray(const char* name) {
	writer->String(name);
	writer->StartArray();
}

void System::EndArray() {
	writer->EndArray();
}

void System::WriteToDisk() {
	writer->EndObject();
	App->fileSystem->Save("/Data/Scenes/scene.json", stringBuffer->GetString(), strlen(stringBuffer->GetString()), false);
}

rapidjson::Document System::LoadFromDisk() {
	rapidjson::Document result = nullptr;

	char* fileBuffer;
	unsigned lenghBuffer = App->fileSystem->Load("/Library/Scenes/scene.json", &fileBuffer);

	if (fileBuffer) {
		if (result.Parse<rapidjson::kParseStopWhenDoneFlag>(fileBuffer).HasParseError()) {
			result = nullptr;
		}
	}

	delete[] fileBuffer;

	return result;
}

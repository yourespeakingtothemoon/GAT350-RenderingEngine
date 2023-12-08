#include "Material.h"

#include <imgui/ImFileDialog.h>

#include "Core/Core.h"
#include "Program.h"
#include "Texture.h"
#include "Cubemap.h"

namespace nc {
	bool Material::Create(std::string filename, ...) {
		// load program json document
		rapidjson::Document document;
		bool success = Json::Load(filename, document);
		if(!success) {
			INFO_LOG("Could not load program file (%s)." << filename);
			return false;
		}

		// read the program name
		std::string program;
		READ_DATA(document, program);
		// get program resource
		this->program = GET_RESOURCE(Program, program);

		// read the textures
		std::string albedoTextureName;
		if(READ_NAME_DATA(document, "albedoTexture", albedoTextureName)) {
			this->albedoTexture = GET_RESOURCE(Texture, albedoTextureName);
		}
		std::string specularTextureName;
		if(READ_NAME_DATA(document, "specularTexture", specularTextureName)) {
			this->specularTexture = GET_RESOURCE(Texture, specularTextureName);
		}
		std::string emissiveTextureName;
		if(READ_NAME_DATA(document, "emissiveTexture", emissiveTextureName)) {
			this->emissiveTexture = GET_RESOURCE(Texture, emissiveTextureName);
		}
		std::string normalTextureName;
		if(READ_NAME_DATA(document, "normalTexture", normalTextureName)) {
			this->normalTexture = GET_RESOURCE(Texture, normalTextureName);
		}

		std::string cubemapName;
		if(READ_NAME_DATA(document, "cubemap", cubemapName)) {
			std::vector<std::string> cubemaps;
			READ_DATA(document, cubemaps);

			this->cubemapTexture = GET_RESOURCE(Cubemap, cubemapName, cubemaps);
		}

		READ_DATA(document, albedo);
		READ_DATA(document, specular);
		READ_DATA(document, emissive);
		READ_DATA(document, shininess);
		READ_DATA(document, tiling);
		READ_DATA(document, offset);

		return true;
	}

	void Material::Bind() {
		this->program->Use();
		this->program->SetUniform("material.params", this->params);

		this->program->SetUniform("material.albedo", this->albedo);
		this->program->SetUniform("material.specular", this->specular);
		this->program->SetUniform("material.emissive", this->emissive);
		this->program->SetUniform("material.shininess", this->shininess);

		this->program->SetUniform("material.tiling", this->tiling);
		this->program->SetUniform("material.offset", this->offset);

		if(albedoTexture != nullptr) {
			params |= ALBEDO_TEXTURE_MASK;
			albedoTexture->SetActive(GL_TEXTURE0);
			albedoTexture->Bind();
		}
		if(specularTexture != nullptr) {
			params |= SPECULAR_TEXTURE_MASK;
			specularTexture->SetActive(GL_TEXTURE1);
			specularTexture->Bind();
		}
		if(normalTexture != nullptr) {
			params |= NORMAL_TEXTURE_MASK;
			normalTexture->SetActive(GL_TEXTURE2);
			normalTexture->Bind();
		}
		if(emissiveTexture != nullptr) {
			params |= EMISSIVE_TEXTURE_MASK;
			emissiveTexture->SetActive(GL_TEXTURE3);
			emissiveTexture->Bind();
		}
		if(cubemapTexture != nullptr) {
			params |= CUBEMAP_TEXTURE_MASK;
			cubemapTexture->SetActive(GL_TEXTURE4);
			cubemapTexture->Bind();
		}
		if(depthTexture != nullptr) {
			depthTexture->SetActive(GL_TEXTURE5);
			depthTexture->Bind();
		}
	}

	void Material::ProcessGUI() {
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Name: %s", name.c_str());

		// Shader
		ImGui::Text("Shader:");
		ImGui::SameLine();
		ImGui::Text("%s", this->program->name.c_str());
		Gui::GetDialogResource<Program>(this->program, "ShaderTextureDialog", "Open Shader", "Shader file (*.prog){.prog},.*");

		// Albedo
		ImGui::Text("Albedo  ");
		ImGui::SameLine();
		ImGui::ColorEdit3("Albedo", glm::value_ptr(albedo), ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		if(albedoTexture != nullptr) {
			ImGui::Text("%s", albedoTexture->name.c_str());
		} else {
			ImGui::Text("None");
		}
		Gui::GetDialogResource<Texture>(albedoTexture, "AlbedoTextureDialog", "Open texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");

		// Specular
		ImGui::Text("Specular");
		ImGui::SameLine();
		ImGui::ColorEdit3("Specular", glm::value_ptr(specular), ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		if(specularTexture != nullptr) {
			ImGui::Text("%s", specularTexture->name.c_str());
		} else {
			ImGui::Text("None");
		}
		Gui::GetDialogResource<Texture>(specularTexture, "SpecularTextureDialog", "Open texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");

		// Emissive
		ImGui::Text("Emissive");
		ImGui::SameLine();
		ImGui::ColorEdit3("Emissive", glm::value_ptr(emissive), ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		if(emissiveTexture != nullptr) {
			ImGui::Text("%s", emissiveTexture->name.c_str());
		} else {
			ImGui::Text("None");
		}
		Gui::GetDialogResource<Texture>(emissiveTexture, "EmissiveTextureDialog", "Open texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");

		// Normal
		ImGui::Text("Normal      ");
		ImGui::SameLine();
		if(normalTexture != nullptr) {
			ImGui::Text("%s", normalTexture->name.c_str());
		} else {
			ImGui::Text("None");
		}
		Gui::GetDialogResource<Texture>(normalTexture, "NormalTextureDialog", "Open texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");

		ImGui::DragFloat("Shininess", &(this->shininess), 0.1f, 2.0f, 216.0f);

		// UV
		ImGui::DragFloat2("Tiling", glm::value_ptr(this->tiling), 0.1f);
		ImGui::DragFloat2("Offset", glm::value_ptr(this->offset), 0.01f);
	}
}
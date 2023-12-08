#pragma once

#include "Framework/Resource/Resource.h"

#include <glm/glm/glm.hpp>
#include <vector>
#include <memory>

#include "GUI.h"

namespace nc {
	class Texture;
	class Cubemap;
	class Program;

	class Material : public Resource {
		public:
			const uint32_t ALBEDO_TEXTURE_MASK   =  1;
			const uint32_t SPECULAR_TEXTURE_MASK =  2;
			const uint32_t NORMAL_TEXTURE_MASK   =  4;
			const uint32_t EMISSIVE_TEXTURE_MASK =  8;
			const uint32_t CUBEMAP_TEXTURE_MASK  = 16;

		public:
			virtual bool Create(std::string filename, ...) override;

			void Bind();

			res_t<Program> GetProgram() {return this->program;}
			void ProcessGUI();

		public:
			uint32_t params = 0;
			glm::vec3 albedo = glm::vec3(1);
			glm::vec3 specular = glm::vec3(1);
			glm::vec3 emissive = glm::vec3(0);
			float shininess = 2.0f;

			glm::vec2 tiling = glm::vec2(1);
			glm::vec2 offset = glm::vec2(0);

			res_t<Texture> albedoTexture;
			res_t<Texture> specularTexture;
			res_t<Texture> normalTexture;
			res_t<Texture> emissiveTexture;
			res_t<Cubemap> cubemapTexture;
			res_t<Texture> depthTexture;

		private:
			res_t<Program> program;
	};
}
#pragma once

namespace nc {
	class Editor {
		public:
			void Update();

			void ProcessGUI(class Scene* scene);

		private:
			bool active = true;
			class Object* selectedObject = nullptr;
			class Object* renameObject = nullptr;
	};
}
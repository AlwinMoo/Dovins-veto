#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include <array>
#include "AEEngine.h"
namespace UI 
{
	enum UI_MESH 
	{
		MESH_BOX,
		NUM_MESH
	};
	enum UI_TEX 
	{
		TEX_BUTTON,
		NUM_TEX
	};
	class UI_Manager
	{
	public:
		s8 m_fontID;
		std::array<AEGfxTexture*, NUM_TEX> m_textures;
		std::array<AEGfxVertexList*, NUM_MESH> m_mesh;

		void LoadFont(char const* path);
		void Unload();
		UI_Manager();
		~UI_Manager();
	};
}
#endif


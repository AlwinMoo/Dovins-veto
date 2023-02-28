#ifndef UI_TYPES_H
#define UI_TYPES_H
namespace UI
{
	enum BUTTON_TYPE {
		WHITE_BUTTON,
		END_PHASE_BUTTON,
		BUILD_TOWER_BUTTON,
		BUILD_NEXUS_BUTTON,
		BUILD_WALL_BUTTON,
		NUM_BUTTONS
	};
	enum UI_MESH
	{
		MESH_BOX,
		NUM_MESH
	};
	enum UI_TEX
	{
		TEX_BUTTON,
		TEX_END_PHASE,
		TEX_NEXUS,
		TEX_TOWER,
		TEX_WALL,
		NUM_TEX
	};
}
#endif

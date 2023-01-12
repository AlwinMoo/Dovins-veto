#include "Rendering.hpp"
#include "AEEngine.h"
namespace render {
	AEGfxVertexList* GenerateQuad() {
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);
		AEGfxTriAdd(
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
		return AEGfxMeshEnd();
	}
}
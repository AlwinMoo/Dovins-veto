/******************************************************************************/
/*!
\file		Rendering.cpp
\author		Alwin Moo
\par        email: moo.g\@digipen.edu
\date       January 26, 2023
\brief		Alwin (Primary 100%)

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Rendering.hpp"
#include "AEEngine.h"
namespace render {
	AEGfxVertexList* GenerateQuad() {
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f);
		AEGfxTriAdd(
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f);
		return AEGfxMeshEnd();
	}
}
/******************************************************************************/
/*!
\file		Rendering.hpp
\author		Alonzo Nalpon
\par        email: a.nalpon@digipen.edu
\date       January 12, 2023
\brief		Alonzo (Primary 100%)
Contains declarations to create different meshes

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#ifndef RENDERING_H
#define RENDERING_H
struct AEGfxVertexList;
namespace render {
	AEGfxVertexList* GenerateQuad();
}
#endif

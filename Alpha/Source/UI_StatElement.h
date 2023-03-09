#ifndef UI_STAT_ELEMENT_H
#define UI_STAT_ELEMENT_H
#include "UI_Def.h"
#include "UI_TextArea.h"
#include <array>
#include <string>
namespace UI {

	struct UI_StatInfo 
	{
		AEVec2	posN;			//!< normalized pos for text
		AEVec2	posWS;			//!< world space pos of object
		AEVec2	meshPos;		//!< world space pos of mesh start
		AEVec2	meshScale;		//!< mesh scale (unmanipulated)
		UI_TextArea* textArea;	//!< text area
		
		double value;			//!< rendering bars

		UI_Color colorTint;		//!< color (including alpha) of image

		// textures
		UI_TEX m_tex;
		// Mesh
		UI_MESH m_mesh;
	};


	// Prints out image followed by text
	class UI_StatElement
	{
		//void (*m_drawFuncArr[UI_STAT_ELEMENT_TOTAL])();
		// TODO: remove this
		std::array<void (*)(), UI_STAT_ELEMENT_TOTAL>m_drawFuncArr;
		UI_STAT_ELEMENT_TYPE m_type;
		

		UI_StatInfo data;

		double m_counter;
		bool m_bTimed;
		std::string m_text;		//!< any beginning text (before value)
	
		//AEVec2 m_textNPosStart;	//!< screen space pos (NORMALIZED) for text
		AEVec2 m_vel;			//!< moving texts

		

		// Private Methods
	public:
		UI_StatElement(UI_TextArea* text = nullptr);
		~UI_StatElement() = default;
		UI_StatElement& operator=(const UI_StatElement&) = delete;
		void CalculatePositions();

		void Update(double dt);
		void Draw();
	};
}
#endif

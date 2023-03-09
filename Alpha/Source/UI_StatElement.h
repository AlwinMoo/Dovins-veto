#ifndef UI_STAT_ELEMENT_H
#define UI_STAT_ELEMENT_H
#include "UI_Def.h"
#include <array>
#include <string>
namespace UI {

	struct UI_StatInfo 
	{
		f32 xN, yN;
		f32 x, y; //!< world space pos
		double value; //!< rendering bars

		UI_Color colorTint; //!< color (including alpha) of image and text
		UI_Color colorFont; //!< color (including alpha) of image and text
		
		float m_scale;			//!< scale of text and image
		std::string str;

		// textures
		AEGfxTexture* m_tex;
		// Mesh
		AEGfxVertexList* m_mesh;
	};


	// Prints out image followed by text
	class UI_StatElement
	{
		//void (*m_drawFuncArr[UI_STAT_ELEMENT_TOTAL])();
		std::array<void (*)(), UI_STAT_ELEMENT_TOTAL>m_drawFuncArr;
		UI_STAT_ELEMENT_TYPE m_type;
		

		UI_StatInfo data;

		double m_counter;
		bool m_bTimed;
		std::string m_text;		//!< any beginning text (before value)
	
		//AEVec2 m_textNPosStart;	//!< screen space pos (NORMALIZED) for text
		AEVec2 m_vel;			//!< moving texts

		

		// Private Methods
		void CalculatePositions();
	public:
		UI_StatElement();
		~UI_StatElement() = default;
		UI_StatElement& operator=(const UI_StatElement&) = delete;

		void Update(double dt);
		void Draw();
	};
}
#endif

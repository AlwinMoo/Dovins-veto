#ifndef UI_STAT_ELEMENT_H
#define UI_STAT_ELEMENT_H
#include "UI_Def.h"
#include <array>
#include <string>
namespace UI {
// Prints out image followed by text
class UI_StatElement
{
	//void (*m_drawFuncArr[UI_STAT_ELEMENT_TOTAL])();
	std::array<void (*)(), UI_STAT_ELEMENT_TOTAL>m_drawFuncArr;
	UI_STAT_ELEMENT_TYPE m_type;
	double m_value;
	double m_counter;
	bool m_bTimed;
	std::string m_text;		//!< any beginning text (before value)

	AEVec2 m_WSPos;			//!< world space pos
	AEVec2 m_textNPosStart;	//!< screen space pos (NORMALIZED) for text
	//AEVec2 m_vel;			//!< moving texts

	AEGfxTexture* m_tex;
	AEGfxVertexList*m_mesh;
	float m_scale;			//!< scale of text and image
	float m_alpha;			//!< alpha of image and text

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

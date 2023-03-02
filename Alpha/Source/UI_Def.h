#ifndef UI_DEF_H
#define UI_DEF_H
#include "AEEngine.h"
enum UI_STAT_ELEMENT_TYPE
{
	UI_STAT_ELEMENT_INT = 0,
	UI_STAT_ELEMENT_FLOAT,
	UI_STAT_TEXT,
	UI_STAT_ELEMENT_TOTAL
};

struct UI_Color { f32 r, g, b; }; //!< simple color struct to work with AE engine. In order: [r]ed, [g]reen, [b]lue



#endif

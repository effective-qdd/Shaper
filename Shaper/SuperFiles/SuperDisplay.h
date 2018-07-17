#ifndef SUPER_DISPLAY_H
#define SUPER_DISPLAY_H

#include "Display.h"
#include "DisplayInit.h"

namespace SHAPER
{
	class CSuperDisplay : public CDisplay, public CDisplayInit
	{
	public:
		CSuperDisplay() {}

		virtual ~CSuperDisplay() {}

	};
}

#endif

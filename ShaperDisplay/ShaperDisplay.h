#ifndef SHAPER_DISPLAY_H
#define SHAPER_DISPLAY_H

#include <memory>

namespace SHAPER
{
	class CBufferCore;
	class CSuperDisplay;
	std::shared_ptr<CSuperDisplay> DisplayInstance(CBufferCore*);
}

#endif


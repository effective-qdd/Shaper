#ifndef DISPLAY_H
#define DISPLAY_H

#include "Defines.h"

namespace SHAPER
{
	class CDisplay
	{
	public:
		CDisplay() {}

		virtual ~CDisplay() {}

		/// Show image
		virtual void Show(IID) noexcept(false) = 0;
	};
}


#endif

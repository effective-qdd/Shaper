#ifndef SUPER_INTENSITY_H
#define SUPER_INTENSITY_H

#include "Intensity.h"
#include "IntensityCore.h"

namespace SHAPER
{
	class CSuperIntensity : public CIntensity, public CIntensityCore
	{
	public:
		CSuperIntensity() = default;

		virtual ~CSuperIntensity() = default;

	};
}

#endif
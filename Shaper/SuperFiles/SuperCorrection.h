#ifndef SUPER_CORRECTION_H
#define SUPER_CORRECTION_H

#include "Correction.h"
#include "CorrectionCore.h"

namespace SHAPER
{
	class CSuperCorrection : public CCorrection, public CCorrectionCore
	{
	public:
		CSuperCorrection() = default;

		virtual ~CSuperCorrection() = default;

	};
}

#endif
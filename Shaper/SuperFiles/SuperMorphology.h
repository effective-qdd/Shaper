#ifndef SUPER_MORPHOLOGY_H
#define SUPER_MORPHOLOGY_H

#include "Morphology.h"
#include "MorphologyCore.h"

namespace SHAPER
{
	class CSuperMorphology : public CMorphology, public CMorphologyCore
	{
	public:
		CSuperMorphology() = default;

		virtual ~CSuperMorphology() = default;
	};
}

#endif
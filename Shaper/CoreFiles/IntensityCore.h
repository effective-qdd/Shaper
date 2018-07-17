#ifndef INTENSITY_CORE_H
#define INTENSITY_CORE_H

#include "AlgorithmCore.h"

namespace SHAPER
{		
	class CIntensityCore : public CAlgorithmCore
	{
	public:
		CIntensityCore() = default;

		virtual ~CIntensityCore() = default;

		virtual std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> LoadInner(ELDER::ImageInfo const&) noexcept(false) = 0;
	};
}	

#endif
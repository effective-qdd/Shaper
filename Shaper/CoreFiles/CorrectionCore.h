#ifndef CORRECTION_CORE_H
#define CORRECTION_CORE_H

#include "AlgorithmCore.h"

namespace SHAPER
{		
	class CCorrectionCore : public CAlgorithmCore
	{
	public:
		CCorrectionCore() = default;

		virtual ~CCorrectionCore() = default;

		//virtual std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> LoadInner(ELDER::ImageInfo const&) noexcept(false) = 0;
	};
}	

#endif
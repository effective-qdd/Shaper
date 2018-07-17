#ifndef FFT_CORE_H
#define FFT_CORE_H

#include "AlgorithmCore.h"

namespace SHAPER
{		
	class CFFTCore : public CAlgorithmCore
	{
	public:
		CFFTCore() = default;

		virtual ~CFFTCore() = default;

		virtual std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> LoadInner(ELDER::ImageInfo const&) noexcept(false) = 0;
	};
}	

#endif
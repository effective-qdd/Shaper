#ifndef ALGORITHM_CORE_H
#define ALGORITHM_CORE_H

#include "Defines.h"
#include "DefinesCore.h"


#include "../../Elder/ImageProcessor/ImageProcessor.hpp"
#include "../../Elder/NonCopyable/NonCopyable.hpp"

namespace SHAPER
{
	class CAlgorithmCore : public ELDER::CNonCopyable
	{
	public:
		CAlgorithmCore() = default;

		virtual~CAlgorithmCore() = default;

		virtual std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> Load(ELDER::ImageInfo const&) noexcept(false) = 0;
	};
}

#endif
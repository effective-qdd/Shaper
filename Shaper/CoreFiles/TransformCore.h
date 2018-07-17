#ifndef TRANSFORM_CORE_H
#define TRANSFORM_CORE_H

#include "AlgorithmCore.h"

namespace SHAPER
{
	class CTransformCore : public CAlgorithmCore
	{
	public:
		CTransformCore() = default;

		virtual ~CTransformCore() = default;

		virtual std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> LoadInner(ELDER::ImageInfo const&) noexcept(false) = 0;
	};
}

#endif
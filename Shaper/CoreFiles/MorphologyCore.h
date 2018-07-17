#ifndef MORPHOLOGY_CORE_H
#define MORPHOLOGY_CORE_H

#include "AlgorithmCore.h"

namespace SHAPER
{		
	class CMorphologyCore : public CAlgorithmCore
	{
	public:
		CMorphologyCore() = default;

		virtual ~CMorphologyCore() = default;

		//virtual std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> const& Available(SID) noexcept(false) = 0;
	};
}	

#endif
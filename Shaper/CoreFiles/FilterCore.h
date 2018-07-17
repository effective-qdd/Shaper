#ifndef FILTER_CORE_H
#define FILTER_CORE_H

#include "AlgorithmCore.h"

namespace SHAPER
{		
	class CFilterCore : public CAlgorithmCore
	{
	public:
		CFilterCore() = default;

		virtual ~CFilterCore() = default;

// 		virtual void LoadMulti(SID, ELDER::ImageInfo const&, int) noexcept(false) = 0;
// 
// 		virtual std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> const& Available(SID) noexcept(false) = 0;
// 		
// 		virtual std::queue<std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>> const& AvailableMulti(SID) noexcept(false) = 0;
	};
}	

#endif
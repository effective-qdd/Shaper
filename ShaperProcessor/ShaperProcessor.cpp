#include "ShaperProcessor.h"

#include "../../Elder/Singleton/Singleton.hpp"

#include "ShaperProcessorImp.h"

namespace SHAPER
{
	std::shared_ptr<CSuperProcessor> ProcessorInstance(bool enable, int num)
	{
		return ELDER::CSingleton<CProcessorImp>::GetInstance(enable, num);
	}
}
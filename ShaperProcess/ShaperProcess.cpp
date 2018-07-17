#include "ShaperProcess.h"
#include "ShaperProcessImp.h"

#include "../../Elder/Singleton/Singleton.hpp"

namespace SHAPER
{
	std::shared_ptr<CSuperProcess> ProcessInstance(CBufferCore* bufferCore, CProcessorCore* processorCore)
	{
		return ELDER::CSingleton<CProcessImp>::GetInstance(bufferCore, processorCore);
	}
}
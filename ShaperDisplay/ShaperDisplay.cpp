#include "ShaperDisplay.h"

#include "../../Elder/Singleton/Singleton.hpp"

#include "ShaperDisplayImp.h"

namespace SHAPER
{
	std::shared_ptr<CSuperDisplay> DisplayInstance(CBufferCore* bufferCore)
	{
		return ELDER::CSingleton<CDisplayImp>::GetInstance(bufferCore);
	}
}


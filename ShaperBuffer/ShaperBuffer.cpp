#include "ShaperBuffer.h"

#include "../../Elder/Singleton/Singleton.hpp"

#include "ShaperBufferImp.h"

namespace SHAPER
{
	std::shared_ptr<CSuperBuffer> BufferInstance(CProcessorInit* proceseorInit)
	{
		return ELDER::CSingleton<CBufferImp>::GetInstance(proceseorInit);
	}
}


#include "ShaperFile.h"

#include "../../Elder/Singleton/Singleton.hpp"

#include "ShaperFileImp.h"

namespace SHAPER
{
	std::shared_ptr<CSuperFile> FileInstance(CBufferInit* bufferInit, CBufferCore* bufferCore)
	{
		return ELDER::CSingleton<CFileImp>::GetInstance(bufferInit, bufferCore);
	}
}

#include "ShaperOperator.h"
#include "ShaperOperatorImp.h"

#include "../../Elder/Singleton/Singleton.hpp"

namespace SHAPER
{
	std::shared_ptr<CSuperOperator> OperatorInstance(CBufferCore *bufferCore)
	{
		return ELDER::CSingleton<COperatorImp>::GetInstance(bufferCore);
	}
}


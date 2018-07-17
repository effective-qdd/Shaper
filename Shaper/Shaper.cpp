#include "Shaper.h"

#include "../../Elder/Singleton/Singleton.hpp"

#include "ShaperImp.h"


namespace SHAPER
{
	std::shared_ptr<CShaper> ShaperInstance()
	{
		return ELDER::CSingleton<CShaperImp>::GetInstance();
	}
}

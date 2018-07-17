#ifndef FILE_INIT_H
#define FILE_INIT_H

#include "Defines.h"

namespace SHAPER
{
	class CFileInit
	{
	public:
		CFileInit() {}

		virtual ~CFileInit() {}

		virtual void  Initialize() noexcept(false) = 0;
	};
}

#endif
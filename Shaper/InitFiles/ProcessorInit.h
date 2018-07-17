#ifndef PROCESSOR_INIT_H
#define PROCESSOR_INIT_H

#include "Defines.h"
#include "DefinesCore.h"

namespace SHAPER
{
	class CProcessorInit
	{
	public:
		CProcessorInit() {}

		virtual ~CProcessorInit() {}

		virtual void Initialize(SID, const ELDER::ImageInfo &) noexcept(false) = 0;

		virtual void UnInitialize() noexcept(false) = 0;
	};
}

#endif
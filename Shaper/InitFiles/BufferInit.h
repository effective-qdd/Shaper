#ifndef BUFFER_INIT_H
#define BUFFER_INIT_H

#include "Defines.h"
#include "DefinesCore.h"

class IImagePool;
namespace SHAPER
{
	class CBufferInit
	{
	public:
		CBufferInit() {}

		virtual ~CBufferInit() {}

		virtual SID Initialize(const ELDER::ImageInfo &) noexcept(false) = 0;

		virtual bool IsInitialized() noexcept(true) = 0;

		virtual void UnInitialize() noexcept(false) = 0;
	};
}

#endif

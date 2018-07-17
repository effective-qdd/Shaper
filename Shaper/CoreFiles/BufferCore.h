#ifndef BUFFER_CORE_H
#define BUFFER_CORE_H

#include <any>

#include "Defines.h"
#include "DefinesCore.h"

#include "../../Elder/NonCopyable/NonCopyable.hpp"

namespace SHAPER
{
	class CBufferCore : public ELDER::CNonCopyable
	{
	public:
		CBufferCore() {}

		virtual ~CBufferCore() {}

		//virtual const ELDER::ImageInfo &Info() noexcept(false) = 0;

		virtual SID BufferID(ELDER::ImageInfo const&) noexcept(false) = 0;

		virtual std::any AcquireRaw(SID) noexcept(false) = 0;

		virtual std::any AcquireProcess(SID) noexcept(false) = 0;

		virtual IID MakeID(std::any&&, SID) noexcept(false) = 0;

		virtual std::any const& Resume(IID) noexcept(false) = 0;
	};
}

#endif

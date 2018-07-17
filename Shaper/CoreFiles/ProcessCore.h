#ifndef PROCESS_CORE_H
#define PROCESS_CORE_H

#include "DefinesCore.h"
#include <list>

#include "../../Elder/NonCopyable/NonCopyable.hpp"

namespace SHAPER
{
	class CProcessCore : public ELDER::CNonCopyable
	{
	public:
		CProcessCore() {}

		virtual ~CProcessCore() {}

		virtual void Initialize() noexcept(false) = 0;

		virtual void UnInitialize() noexcept(false) = 0;

	};
}

#endif
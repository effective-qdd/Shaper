#ifndef STREAM_CORE_H
#define STREAM_CORE_H

#include "DefinesCore.h"
#include <string>

#include "../../Elder/NonCopyable/NonCopyable.hpp"

namespace SHAPER
{
	class CStreamCore : public ELDER::CNonCopyable
	{
	public:
		CStreamCore() = default;

		virtual ~CStreamCore() = default;

		virtual void Open(std::string const&) noexcept(false) = 0;

		virtual ELDER::FileInfo Info() noexcept(false) = 0;

	};
}

#endif
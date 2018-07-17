#ifndef SUPER_STREAM_H
#define SUPER_STREAM_H

#include "Stream.h"
#include "StreamCore.h"

namespace SHAPER
{
	class CSuperStream : public CStream, public CStreamCore
	{
	public:
		CSuperStream() = default;

		virtual ~CSuperStream() = default;
	};
}

#endif
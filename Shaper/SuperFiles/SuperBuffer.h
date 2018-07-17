#ifndef SUPER_BUFFER_H
#define SUPER_BUFFER_H

#include "Buffer.h"
#include "BufferInit.h"
#include "BufferCore.h"


namespace SHAPER
{
	class CSuperBuffer : public CBuffer, public CBufferInit, public CBufferCore
	{
	public:
		CSuperBuffer() {}

		virtual ~CSuperBuffer() {}

	};
}


#endif

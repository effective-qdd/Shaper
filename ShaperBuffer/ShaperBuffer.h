#ifndef SHAPER_BUFFER_H
#define SHAPER_BUFFER_H

#include <memory>

namespace SHAPER
{
	class CSuperBuffer;
	class CProcessorInit;
	std::shared_ptr<CSuperBuffer> BufferInstance(CProcessorInit*);
}


#endif


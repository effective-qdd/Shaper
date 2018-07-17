#ifndef SUPER_PROCESSOR_H
#define SUPER_PROCESSOR_H

#include "Processor.h"
#include "ProcessorInit.h"
#include "ProcessorCore.h"


namespace SHAPER
{
	class CSuperProcessor : public CProcessor, public CProcessorInit, public CProcessorCore
	{
	public:
		CSuperProcessor() {}

		virtual ~CSuperProcessor() {}
	};
}
#endif
#ifndef SHAPER_PROCESSOR_H
#define SHAPER_PROCESSOR_H

#include <memory>

namespace SHAPER
{
	class CSuperProcessor;
	std::shared_ptr<CSuperProcessor> ProcessorInstance(bool, int);
}

#endif

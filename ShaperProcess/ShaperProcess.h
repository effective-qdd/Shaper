#ifndef SHAPER_PROCESS_H
#define SHAPER_PROCESS_H

#include <memory>

namespace SHAPER
{
	class CSuperProcess;
	class CBufferCore;
	class CProcessorCore;
	std::shared_ptr<CSuperProcess> ProcessInstance(CBufferCore*, CProcessorCore*);
}

#endif
#ifndef SUPER_PROCESS_H
#define SUPER_PROCESS_H

#include "Process.h"
#include "ProcessCore.h"

namespace SHAPER
{
	class CSuperProcess : public CProcess, public CProcessCore
	{
	public:
		CSuperProcess() {}

		virtual ~CSuperProcess() {}

	};
}

#endif
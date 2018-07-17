#ifndef SUPER_FILTER_H
#define SUPER_FILTER_H

#include "Filter.h"
#include "FilterCore.h"

namespace SHAPER
{
	class CSuperFilter : public CFilter, public CFilterCore
	{
	public:
		CSuperFilter() = default;

		virtual ~CSuperFilter() = default;

	};
}

#endif
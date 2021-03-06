#ifndef SHAPER_OPERATOR_H
#define SHAPER_OPERATOR_H

#include <memory>

namespace SHAPER
{
	class CSuperOperator;
	class CBufferCore;
	std::shared_ptr<CSuperOperator> OperatorInstance(CBufferCore *);
}

#endif


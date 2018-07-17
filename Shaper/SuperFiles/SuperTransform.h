#ifndef SUPER_TRANSFORM_H
#define SUPER_TRANSFORM_H

#include "Transform.h"
#include "TransformCore.h"

namespace SHAPER
{
	class CSuperTransform : public CTransform, public CTransformCore
	{
	public:
		CSuperTransform() = default;

		virtual ~CSuperTransform() = default;

	};
}

#endif
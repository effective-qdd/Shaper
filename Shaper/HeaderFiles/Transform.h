#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Defines.h"

namespace SHAPER
{		
		
	class CTransform
	{
	public:
		CTransform() = default;

		virtual ~CTransform() = default;

		/// Set rotation special angle which is clockwise degrees. Such as k90Degrees, k180Degrees and k270Degrees.
		virtual void SetRotationSpecialAngle(SpecialAngles) noexcept(false) = 0;

		/// Set rotation arbitrary angle. e.g. k90Degrees = -90.0
		virtual void SetRotationArbitraryAngle(double) noexcept(false) = 0;

		/// Set flip mode. Such as kHorizintal, kVertical and kHorizontalVertical.
		virtual void SetMode(FlipModes) noexcept(false) = 0;
	};
}	

#endif
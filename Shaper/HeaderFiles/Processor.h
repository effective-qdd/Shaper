#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Defines.h"
#include "Filter.h"
#include "Transform.h"
#include "Intensity.h"

namespace SHAPER
{		
	class CFilter;
	class CRotation;
	class CFlip;
	class CIntensity;
	class CProcessor
	{
	public:

		CProcessor() {}

		virtual ~CProcessor() {}

		/// Get CFilter ptr.
		virtual CFilter* Filter() const noexcept(false) = 0;

		/// Get CRotation ptr.
		virtual CTransform* Transform() const noexcept(false) = 0;

		/// Get CEnhancement ptr
		virtual CIntensity* Intensity() const noexcept(false) = 0;
	};
}	

#endif
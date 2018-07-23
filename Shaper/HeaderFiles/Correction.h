#ifndef CORRECTION_H
#define CORRECTION_H

#include "Defines.h"
	
namespace SHAPER
{
	class CCorrection
	{
	public:
		CCorrection() = default;

		virtual ~CCorrection() = default;

		/// Set dark reference image to Dark Correction
		virtual void SetDarkReference(float const*const darkRefImage) = 0;

		/// Set dark reference image to Dark Gain Correction
		virtual void SetDarkGainReferences(float const*const darkRefImage, float const*const gainRefImage) = 0;

		/// Set template reference image to normalized cross-correlation correction
		virtual void SetTemplateReference(void const*const templateRefImage) = 0;
	};
}
#endif
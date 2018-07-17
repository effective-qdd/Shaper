#ifndef FILTER_H
#define FILTER_H

#include "Defines.h"

namespace SHAPER
{		
	class CFilter
	{
	public:
		CFilter() = default;

		virtual ~CFilter() = default;

		/// Set gaussian sigma which is the standard deviation of the gaussian kernel.
		virtual void SetGaussianSigma(float) noexcept(false) = 0;
		
	};
}	

#endif
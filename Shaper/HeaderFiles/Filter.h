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

		/// Set bilateral pixel neighborhood size (max distance).
		virtual void SetBilateralRadius(int) noexcept(false) = 0;

		/// Set bilateral sigma color. Width param for color weight function
		virtual void SetBilateralSigmaColor(double) noexcept(false) = 0;

		/// Set bilateral sigma space. Width param for spatial weight function
		virtual void SetBilateralSigmaSpace(double) noexcept(false) = 0;
		
	};
}	

#endif
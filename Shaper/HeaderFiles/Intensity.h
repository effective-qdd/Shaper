#ifndef INTENSITY_H
#define INTENSITY_H

#include "Defines.h"
	
namespace SHAPER
{
	class CIntensity
	{
	public:
		CIntensity() = default;

		virtual ~CIntensity() = default;

		/// Set gamma value.
		virtual void SetGammaValue(float gammaValue) = 0;

		/// Set log value.
		virtual void SetLogValue(double logValue) = 0;

		/// Set linear point with P1(lowIn, lowOut) and P2(highIn, highOut).
		virtual void SetLinearPoint(int lowIn, int lowOut, int highIn, int highOut) = 0;

		/// Set linear point with P1(lowIn, 0) and P2(highIn, MaxIntensityValue).
		virtual void SetLinearPoint(int lowIn, int highIn) = 0;

		/// Set piecewise linear point with P1(lowIn, lowOut) and P2(highIn, highOut).
		virtual void SetPiecewiseLinearPoint(int lowIn, int lowOut, int highIn, int highOut) = 0;

		/// Set contrast and brightness.
		virtual void SetContrastBrightness(double contrast, double brightness) = 0;

		/// Set threshold.
		virtual void SetThreshold(int thresholdLT, int valueLT, int thresholdGT, int valueGT) = 0;
	};
}
#endif
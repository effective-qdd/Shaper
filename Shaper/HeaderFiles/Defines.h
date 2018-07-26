#ifndef DEFINES_H
#define DEFINES_H

#include <list>
#include <memory>
#include <thread>

#include "Structs.h"

namespace SHAPER
{
	/// Defines
	typedef unsigned long ImageID;
	typedef ImageID IID;

	const IID kMaxIID = 0xFFFF;

	typedef unsigned long FileID;
	typedef FileID FID;

	typedef unsigned long ProcessID;
	typedef ProcessID PID;

	const int kNone = 0;

	/// Enumerators 
	enum class FileTypes
	{
		kTif = 1,
		kMrc,
		kSeq,
	};

	enum class ProcessorTypes
	{
		/// Correction
		kDarkRef = 1,
		kDarkGainRef,

		/// Filter
		kMedian3x3,
		kMedian5x5,
		kGaussian3x3,
		kGaussian5x5,		
		kMean3x3,
		kMean5x5,
		kMaximum3x3,
		kMaximum5x5,
		kMinimum3x3,
		kMinimum5x5,
		kLaplace3x3,
		kLaplace5x5,
		kHighPass3x3,
		kHighPass5x5,
		kLowPass3x3,
		kLowPass5x5,
		KBilateral,

		/// Enhancement
		kHistogramEqualization, ///< Histogram equalization
		kHistogramLocal, ///< Histogram local
		kIntensityNegative, ///< Intensity negative
		kIntensityLog, ///< Intensity Log
		kIntensityGamma, ///< Intensity Gamma
		kIntensityLinear, ///< Intensity Linear
		kIntensityPiecewiseLinear, ///< Intensity Piecewise Linear
		kIntensityContrastBrightness, ///< Intensity Brightness and Contrast 
		kIntensityThreshold,

		/// Morphology
		kDilate3x3,
		kDilate5x5,
		kErode3x3,
		kErode5x5,
		kOpen3x3,
		kOpen5x5,
		kClose3x3,
		kClose5x5,
	};		

	enum class BorderTypes
	{
		kConst = 0,
		kRepl = 1,
		kInMem = 6
	};

	enum class SpecialAngles
	{
		k90Degrees = 1,
		k180Degrees,
		k270Degrees
	};

	enum class FlipModes
	{
		kHorizintal = 1,
		kVertical,
		kHorizontalVertical
	};
}

#endif

#ifndef CORE_DEFINES_H
#define CORE_DEFINES_H

#include <list>
#include <map>
#include <deque>
#include <mutex>

namespace SHAPER
{
	/// Defines
// 	typedef unsigned long BufferID;
// 	typedef BufferID BID;
// 
// 	typedef unsigned long ProcessorID;
// 	typedef ProcessorID PRID;

	/// 0 ~ 65535
	using ShaperID = unsigned short;
	using SID = ShaperID;

	/// The enum of processor types for core.
	enum class InnerProcessorTypes
	{
		/// Filter
// 		kMedian3x3 = 1,
// 		kMedian5x5,
// 		kGaussian3x3,
// 		kGaussian5x5,

		/// Enhancement
// 		kHistogramEqualization, ///< Histogram equalization
// 		kIntensityNegative, ///< Intensity negative
// 		kIntensityLog, ///< Intensity Log
// 		kIntensityGamma, ///< Intensity Gamma
// 		kIntensityLinear, ///< Intensity Linear
// 		kIntensityPiecewiseLinear, ///< Intensity Piecewise Linear
// 		kIntensityContrastBrightness, ///< Intensity Brightness and Contrast 
// 		kIntensityThreshold,

		/// Rotation
		kRotationSepcialAngle,
		kRotationArbitraryAngle,

		/// Flit
		kFlip,

		/// Accumulation
		kAccumulation,

		/// Average
		kAverage,

		/// FFT
		kFFT,
	};

}

#endif

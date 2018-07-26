#ifndef PROCESS_H
#define PROCESS_H

#include "Defines.h"
#include "Processor.h"

namespace SHAPER
{
	class CProcess
	{
	public:
		CProcess() {}

		virtual ~CProcess() {}	

		/// Create a process list which contains some processors.
		virtual PID CreateList() noexcept(false) = 0;

		/// None attached processor
		virtual bool Empty(PID) noexcept(false) = 0;

		/// Is process list changed (attached or detached).
		virtual bool Changed(PID) noexcept(false) = 0;

		/// Attach a processor to the process list with the pid.
		virtual void Attach(PID, ProcessorTypes) noexcept(false) = 0;

		/// Detach a processor from the process list with the pid.
		/// If there are more than one of the same ProcessorType, it will detach the last one.
		virtual void Detach(PID, ProcessorTypes) noexcept(false) = 0;

		/// Whether the process list contains the processor type.
		virtual bool Has(PID, ProcessorTypes) noexcept(false) = 0;

		/// Process a iid with specify processing list.
		virtual IID Apply(PID, IID) noexcept(false) = 0;

		/// Multi core process a iid with specify processing list.
		virtual IID ApplyMultiCore(PID, IID) noexcept(false) = 0;

		/// Corrections
		/// Just apply the dark reference correction.
		virtual IID ApplyCorrectionDarkRef(IID) noexcept(false) = 0;

		/// Just apply the dark and gain reference correction.
		virtual IID ApplyCorrectionDarkGainRef(IID) noexcept(false) = 0;

		/// Filter
		/// Just apply the median 3x3 filter.
		virtual IID ApplyFilterMedian3x3(IID) noexcept(false) = 0;

		/// Just apply the median 5x5 filter.
		virtual IID ApplyFilterMedian5x5(IID) noexcept(false) = 0;

		/// Just apply the gaussian 3x3 filter.
		virtual IID ApplyFilterGaussian3x3(IID) noexcept(false) = 0;

		/// Just apply the gaussian 5x5 filter.
		virtual IID ApplyFilterGaussian5x5(IID) noexcept(false) = 0;		

		/// Just apply the mean 3x3 filter.
		virtual IID ApplyFilterMean3x3(IID) noexcept(false) = 0;

		/// Just apply the mean 5x5 filter.
		virtual IID ApplyFilterMean5x5(IID) noexcept(false) = 0;

		/// Just apply the maximum 3x3 filter.
		virtual IID ApplyFilterMaximum3x3(IID) noexcept(false) = 0;

		/// Just apply the maximum 5x5 filter.
		virtual IID ApplyFilterMaximum5x5(IID) noexcept(false) = 0;

		/// Just apply the minimum 3x3 filter.
		virtual IID ApplyFilterMinimum3x3(IID) noexcept(false) = 0;

		/// Just apply the minimum 5x5 filter.
		virtual IID ApplyFilterMinimum5x5(IID) noexcept(false) = 0;

		/// Just apply the laplace 3x3 filter.
		virtual IID ApplyFilterLaplace3x3(IID) noexcept(false) = 0;

		/// Just apply the laplace 5x5 filter.
		virtual IID ApplyFilterLaplace5x5(IID) noexcept(false) = 0;

		/// Just apply the High Pass 3x3 filter.
		virtual IID ApplyFilterHighPass3x3(IID) noexcept(false) = 0;

		/// Just apply the High Pass 5x5 filter.
		virtual IID ApplyFilterHighPass5x5(IID) noexcept(false) = 0;

		/// Just apply the Low Pass 3x3 filter.
		virtual IID ApplyFilterLowPass3x3(IID) noexcept(false) = 0;

		/// Just apply the Low Pass 5x5 filter.
		virtual IID ApplyFilterLowPass5x5(IID) noexcept(false) = 0;

		/// Just apply the bilateral filter.
		virtual IID ApplyFilterBilateral(IID) noexcept(false) = 0;

		/// Intensity
		/// Just apply the histogram equalization.
		virtual IID ApplyIntensityHistogramEqual(IID) noexcept(false) = 0;

		/// Just apply the histogram local.
		virtual IID ApplyIntensityHistogramLocal(IID) noexcept(false) = 0;

		/// Just apply the intensity negative.
		virtual IID ApplyIntensityNegative(IID) noexcept(false) = 0;

		/// Just apply the intensity log.
		virtual IID ApplyIntensityLog(IID) noexcept(false) = 0;

		/// Just apply the intensity gamma.
		virtual IID ApplyIntensityGamma(IID) noexcept(false) = 0;

		/// Just apply the intensity linear.
		virtual IID ApplyIntensityLinear(IID) noexcept(false) = 0;

		/// Just apply the intensity piecewise linear.
		virtual IID ApplyIntensityPiecewiseLinear(IID) noexcept(false) = 0;

		/// Just apply the intensity brightness and contrast.
		virtual IID ApplyIntensityBrightnessContrast(IID) noexcept(false) = 0;

		///Just apply the intensity threshold.
		virtual IID ApplyIntensityThreshold(IID) noexcept(false) = 0;

		/// Morphology
		/// Just apply the dilate3x3 with the specify type.
		virtual IID ApplyMorphologyDilate3x3(IID) noexcept(false) = 0;

		/// Just apply the dilate5x5 with the specify type.
		virtual IID ApplyMorphologyDilate5x5(IID) noexcept(false) = 0;

		/// Just apply the erode3x3 with the specify type.
		virtual IID ApplyMorphologyErode3x3(IID) noexcept(false) = 0;

		/// Just apply the erode5x5 with the specify type.
		virtual IID ApplyMorphologyErode5x5(IID) noexcept(false) = 0;

		/// Just apply the open3x3 with the specify type.
		virtual IID ApplyMorphologyOpen3x3(IID) noexcept(false) = 0;

		/// Just apply the open5x5 with the specify type.
		virtual IID ApplyMorphologyOpen5x5(IID) noexcept(false) = 0;

		/// Just apply the close3x3 with the specify type.
		virtual IID ApplyMorphologyClose3x3(IID) noexcept(false) = 0;

		/// Just apply the close5x5 with the specify type.
		virtual IID ApplyMorphologyClose5x5(IID) noexcept(false) = 0;

		/// Just apply the accumulation with the iid list.
		virtual IID ApplyAccumulation(std::list<IID> const&) noexcept(false) = 0;

		/// Just apply the average with the iid list.
		virtual IID ApplyAverage(std::list<IID> const&) noexcept(false) = 0;
		
		/// Transform
		/// Just apply the rotation with the specify type.
		virtual IID ApplyTransformRotation(IID) noexcept(false) = 0;

		/// Just apply the flip with the specify type.
		virtual IID ApplyTransformFlip(IID) noexcept(false) = 0;

		/// Frequency
		/// Just apply the FFT with the specify type.
		virtual IID ApplyFFT(IID) noexcept(false) = 0;
		
	};
}

#endif
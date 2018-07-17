#ifndef SHAPER_PROCESS_IMP_H
#define SHAPER_PROCESS_IMP_H

#include <map>
#include <mutex>

#include "SuperProcess.h"

namespace SHAPER
{
	class CBufferCore;
	class CProcessorCore;
	class CProcessImp final : public CSuperProcess
	{
	public:
		CProcessImp(CBufferCore*, CProcessorCore*);

		~CProcessImp();

		void Initialize();

		void UnInitialize();

		PID CreateList();

		bool Empty(PID);

		bool Changed(PID);

		void Attach(PID, ProcessorTypes);

		void Detach(PID, ProcessorTypes);

		bool Has(PID, ProcessorTypes);

		IID Apply(PID, IID);

		IID ApplyMultiCore(PID, IID);

		IID ApplyFilterMedian3x3(IID);

		IID ApplyFilterMedian5x5(IID);

		IID ApplyFilterGaussian3x3(IID);

		IID ApplyFilterGaussian5x5(IID);

		IID ApplyFilterMean3x3(IID);

		IID ApplyFilterMean5x5(IID);

		IID ApplyFilterMaximum3x3(IID);

		IID ApplyFilterMaximum5x5(IID);

		IID ApplyFilterMinimum3x3(IID);

		IID ApplyFilterMinimum5x5(IID);

		IID ApplyFilterLaplace3x3(IID);

		IID ApplyFilterLaplace5x5(IID);

		IID ApplyFilterHighPass3x3(IID);

		IID ApplyFilterHighPass5x5(IID);

		IID ApplyFilterLowPass3x3(IID);

		IID ApplyFilterLowPass5x5(IID);

		IID ApplyFilterBilateral(IID);

		IID ApplyIntensityHistogramEqual(IID);

		IID ApplyIntensityHistogramLocal(IID);

		IID ApplyIntensityNegative(IID);

		IID ApplyIntensityLog(IID);

		IID ApplyIntensityGamma(IID);

		IID ApplyIntensityLinear(IID);

		IID ApplyIntensityPiecewiseLinear(IID);

		IID ApplyIntensityBrightnessContrast(IID);

		IID ApplyIntensityThreshold(IID);

		IID ApplyMorphologyDilate3x3(IID);

		IID ApplyMorphologyDilate5x5(IID);

		IID ApplyMorphologyErode3x3(IID);

		IID ApplyMorphologyErode5x5(IID);

		IID ApplyMorphologyOpen3x3(IID);

		IID ApplyMorphologyOpen5x5(IID);

		IID ApplyMorphologyClose3x3(IID);

		IID ApplyMorphologyClose5x5(IID);

		IID ApplyAccumulation(std::list<IID> const&);

		IID ApplyAverage(std::list<IID> const&);

		IID ApplyTransformRotation(IID);				

		IID ApplyTransformFlip(IID);

		IID ApplyFFT(IID);

	private:
		CBufferCore* m_bufferCore;
		CProcessorCore* m_processorCore;
		PID m_pid;
		std::mutex m_mutex;		
		std::mutex m_applyMutex;
		std::list<ProcessorTypes> m_supportedProcessorTypeList;
		std::map<PID, std::list<ProcessorTypes>> m_pidProcessorMap;
		std::map<PID, bool> m_pidListChangedMap;
	};
}

#endif
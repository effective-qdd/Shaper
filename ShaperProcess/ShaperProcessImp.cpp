#include "ShaperProcessImp.h"

#include "../../Elder/Logger/Logger.hpp"
#include "../../Elder/Singleton/Singleton.hpp"
#include "../../Elder/ImageProcessor/ImageProcessor.hpp"

#include "BufferCore.h"
#include "ProcessorCore.h"
#include "FilterCore.h"
#include "TransformCore.h"

namespace SHAPER
{
	CProcessImp::CProcessImp(CBufferCore* bufferCore, CProcessorCore* processorCore)
		: CSuperProcess()
		, m_bufferCore(bufferCore)
		, m_processorCore(processorCore)
		, m_pid(0)
	{
		ASSERT_LOG(m_bufferCore != nullptr, "m_bufferCore is null");
		ASSERT_LOG(processorCore != nullptr, "processorCore is null");

		m_supportedProcessorTypeList.push_back(ProcessorTypes::kDarkRef);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kDarkGainRef);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kCrossCorrelationNormal);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kMedian3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kMedian5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kGaussian3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kGaussian5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kMean3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kMean5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kMaximum3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kMaximum5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kMinimum3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kMinimum5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kLaplace3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kLaplace5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kHighPass3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kHighPass5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kLowPass3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kLowPass5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::KBilateral);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kHistogramEqualization);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kIntensityNegative);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kIntensityLog);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kIntensityGamma);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kIntensityLinear);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kIntensityPiecewiseLinear);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kIntensityContrastBrightness);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kDilate3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kDilate5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kErode3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kErode5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kOpen3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kOpen5x5);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kClose3x3);
		m_supportedProcessorTypeList.push_back(ProcessorTypes::kClose5x5);
		

// 		m_supportedFilterTypeList.push_back(kMedian3x3);
// 		m_supportedFilterTypeList.push_back(kMedian5x5);
// 		m_supportedFilterTypeList.push_back(kGaussian3x3);
// 		m_supportedFilterTypeList.push_back(kGaussian5x5);
// 
// 		m_supportedRotationTypeList.push_back(kRotationSepcialAngle);
// 		m_supportedRotationTypeList.push_back(kRotationArbitraryAngle);
// 
// 		m_supportedFlipTypeList.push_back(kFlip);

// 		m_supportedEnhancementTypeList.push_back(kHistogramEqualization);
// 		m_supportedEnhancementTypeList.push_back(kIntensityNegative);
// 		m_supportedEnhancementTypeList.push_back(kIntensityLog);
// 		m_supportedEnhancementTypeList.push_back(kIntensityGamma);
// 		m_supportedEnhancementTypeList.push_back(kIntensityLinear);
// 		m_supportedEnhancementTypeList.push_back(kIntensityPiecewiseLinear);
// 		m_supportedEnhancementTypeList.push_back(kIntensityContrastBrightness);
// 		m_supportedEnhancementTypeList.push_back(kAccumulation);
	}

	CProcessImp::~CProcessImp()
	{
	}

	void CProcessImp::Initialize()
	{

	}

	void CProcessImp::UnInitialize()
	{
		m_pid = 0;
	}

	PID CProcessImp::CreateList()
	{
		auto pid = ++m_pid;
		m_pidProcessorMap.emplace(pid, std::move(std::list<ProcessorTypes>()));
		m_pidListChangedMap.emplace(pid, false);

		return pid;
	}

	bool CProcessImp::Empty(PID pid)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(m_pidProcessorMap.find(pid) != m_pidProcessorMap.end(), "Can not find such pid");

		return m_pidProcessorMap[pid].empty();
	}

	bool CProcessImp::Changed(PID pid)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(m_pidProcessorMap.find(pid) != m_pidProcessorMap.end(), "Can not find such pid");

		auto changed = m_pidListChangedMap[pid];
		m_pidListChangedMap[pid] = false;

		return changed;
	}

	void CProcessImp::Attach(PID pid, ProcessorTypes processorType)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(m_pidProcessorMap.find(pid) != m_pidProcessorMap.end(), "Can not find such pid");
		ENSURE_THROW_MSG(m_pidListChangedMap.find(pid) != m_pidListChangedMap.end(), "Can not find such pid");
		ENSURE_THROW_MSG(std::find(m_supportedProcessorTypeList.begin(), m_supportedProcessorTypeList.end(), processorType) != m_supportedProcessorTypeList.end(), "Can not find such processor type");
		m_pidProcessorMap[pid].push_back(processorType);
		m_pidListChangedMap[pid] = true;
	}

	void CProcessImp::Detach(PID pid, ProcessorTypes processorType)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(m_pidProcessorMap.find(pid) != m_pidProcessorMap.end(), "Can not find such pid");
		ENSURE_THROW_MSG(m_pidListChangedMap.find(pid) != m_pidListChangedMap.end(), "Can not find such pid");
		ENSURE_THROW_MSG(std::find(m_supportedProcessorTypeList.begin(), m_supportedProcessorTypeList.end(), processorType) != m_supportedProcessorTypeList.end(), "Can not find such processor type");
		auto &processList = m_pidProcessorMap[pid];
		for (auto it = processList.rbegin(); it != processList.rend(); ++it)
		{
			if (*it == processorType)
			{
				std::list<ProcessorTypes>::reverse_iterator(processList.erase((++it).base()));
				m_pidListChangedMap[pid] = true;
				break;
			}
		}
	}

	bool CProcessImp::Has(PID pid, ProcessorTypes processorType)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(m_pidProcessorMap.find(pid) != m_pidProcessorMap.end(), "Can not find such pid");
		ENSURE_THROW_MSG(std::find(m_supportedProcessorTypeList.begin(), m_supportedProcessorTypeList.end(), processorType) != m_supportedProcessorTypeList.end(), "Can not find such processor type");
		auto processList = m_pidProcessorMap[pid];

		return (std::find(processList.begin(), processList.end(), processorType) != processList.end());
	}

	IID CProcessImp::Apply(PID pid, IID iid)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		ELDER::ImageInfo outImageInfo;
		auto processorList = m_processorCore->Active(sid, m_pidProcessorMap[pid]);
		for (auto processor : processorList)
		{
			auto buffer = std::move(m_bufferCore->AcquireRaw(sid));
			if (!processor->Apply(std::move(buf), std::move(buffer), outImageInfo))
			{
				sid = std::move(m_bufferCore->BufferID(outImageInfo));
				buffer = std::move(m_bufferCore->AcquireRaw(sid));
				ENSURE_THROW_MSG(processor->Apply(std::move(buf), std::move(buffer), outImageInfo), "Apply Failed");
			}
			buf = std::move(buffer);
		}

		return m_bufferCore->MakeID(std::move(buf), sid);
	}

	IID CProcessImp::ApplyMultiCore(PID pid, IID iid)
	{
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto availableProcessorMultiCoreMap = m_processorCore->PopMultiCore(sid, m_pidProcessorMap[pid]);
		std::list<std::shared_ptr<ELDER::CImageProcessor>> activeProcessorProcList;
		auto processorTypeList = m_pidProcessorMap[pid];
		ELDER::ImageInfo outImageInfo;
		for (const auto processorType : processorTypeList)
		{
			activeProcessorProcList.push_back(availableProcessorMultiCoreMap.at(processorType));
		}

		for (auto processor : activeProcessorProcList)
		{
			auto buffer = std::move(m_bufferCore->AcquireRaw(sid));
			if (!processor->Apply(std::move(buf), std::move(buffer), outImageInfo))
			{
				sid = std::move(m_bufferCore->BufferID(outImageInfo));
				buffer = std::move(m_bufferCore->AcquireRaw(sid));
				ENSURE_THROW_MSG(processor->Apply(std::move(buf), std::move(buffer), outImageInfo), "Apply Failed");
			}
			buf = std::move(buffer);
		}
		m_processorCore->PushMultiCore(sid, std::move(availableProcessorMultiCoreMap));

		return m_bufferCore->MakeID(std::move(buf), sid);
	}

	IID CProcessImp::ApplyCorrectionDarkRef(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto correction = m_processorCore->Active(sid, ProcessorTypes::kDarkRef);
		auto correctionBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(correction->Apply(std::move(buf), std::move(correctionBuffer), outImageInfo), "Apply dark reference Failed");

		return m_bufferCore->MakeID(std::move(correctionBuffer), sid);
	}

	IID CProcessImp::ApplyCorrectionDarkGainRef(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto correction = m_processorCore->Active(sid, ProcessorTypes::kDarkGainRef);
		auto correctionBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(correction->Apply(std::move(buf), std::move(correctionBuffer), outImageInfo), "Apply dark gain reference Failed");

		return m_bufferCore->MakeID(std::move(correctionBuffer), sid);
	}

	IID CProcessImp::ApplyCorrectionCrossCorrelationNormal(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto correction = m_processorCore->Active(sid, ProcessorTypes::kCrossCorrelationNormal);
		auto correctionBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(correction->Apply(std::move(buf), std::move(correctionBuffer), outImageInfo), "Apply normalized cross correlation Failed");

		return m_bufferCore->MakeID(std::move(correctionBuffer), sid);
	}

	IID CProcessImp::ApplyFilterMedian3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kMedian3x3);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply median 3x3 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterMedian5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kMedian5x5);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply median 5x5 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterGaussian3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kGaussian3x3);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply gaussian 3x3 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterGaussian5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kGaussian5x5);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply gaussian 5x5 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterMean3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kMean3x3);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply mean 3x3 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterMean5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kMean5x5);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply mean 5x5 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterMaximum3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kMaximum3x3);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply maximum 3x3 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterMaximum5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kMaximum5x5);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply maximum 5x5 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterMinimum3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kMinimum3x3);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply minimum 3x3 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterMinimum5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kMinimum5x5);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply minimum 5x5 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterLaplace3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kLaplace3x3);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply laplace 3x3 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterLaplace5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kLaplace5x5);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply laplace 5x5 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterHighPass3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kHighPass3x3);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply high pass 3x3 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterHighPass5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kHighPass5x5);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply high pass 3x3 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterLowPass3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kLowPass5x5);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply low pass 3x3 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterLowPass5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::kLowPass5x5);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply low pass 5x5 Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyFilterBilateral(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto filter = m_processorCore->Active(sid, ProcessorTypes::KBilateral);
		auto filterBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(filter->Apply(std::move(buf), std::move(filterBuffer), outImageInfo), "Apply bilateral Failed");

		return m_bufferCore->MakeID(std::move(filterBuffer), sid);
	}

	IID CProcessImp::ApplyIntensityHistogramEqual(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto enhancement = m_processorCore->Active(sid, ProcessorTypes::kHistogramEqualization);
		auto enhancementBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(enhancement->Apply(std::move(buf), std::move(enhancementBuffer), outImageInfo), "Apply Flip Failed");

		return m_bufferCore->MakeID(std::move(enhancementBuffer), sid);
	}

	IID CProcessImp::ApplyIntensityHistogramLocal(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto enhancement = m_processorCore->Active(sid, ProcessorTypes::kHistogramLocal);
		auto enhancementBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(enhancement->Apply(std::move(buf), std::move(enhancementBuffer), outImageInfo), "Apply Flip Failed");

		return m_bufferCore->MakeID(std::move(enhancementBuffer), sid);
	}

	IID CProcessImp::ApplyIntensityNegative(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto enhancement = m_processorCore->Active(sid, ProcessorTypes::kIntensityNegative);
		auto enhancementBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(enhancement->Apply(std::move(buf), std::move(enhancementBuffer), outImageInfo), "Apply Flip Failed");

		return m_bufferCore->MakeID(std::move(enhancementBuffer), sid);
	}

	IID CProcessImp::ApplyIntensityLog(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto enhancement = m_processorCore->Active(sid, ProcessorTypes::kIntensityLog);
		auto enhancementBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(enhancement->Apply(std::move(buf), std::move(enhancementBuffer), outImageInfo), "Apply Flip Failed");

		return m_bufferCore->MakeID(std::move(enhancementBuffer), sid);
	}

	IID CProcessImp::ApplyIntensityGamma(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto enhancement = m_processorCore->Active(sid, ProcessorTypes::kIntensityGamma);
		auto enhancementBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(enhancement->Apply(std::move(buf), std::move(enhancementBuffer), outImageInfo), "Apply Flip Failed");

		return m_bufferCore->MakeID(std::move(enhancementBuffer), sid);
	}

	IID CProcessImp::ApplyIntensityLinear(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto enhancement = m_processorCore->Active(sid, ProcessorTypes::kIntensityLinear);
		auto enhancementBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(enhancement->Apply(std::move(buf), std::move(enhancementBuffer), outImageInfo), "Apply Flip Failed");

		return m_bufferCore->MakeID(std::move(enhancementBuffer), sid);
	}

	IID CProcessImp::ApplyIntensityPiecewiseLinear(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto enhancement = m_processorCore->Active(sid, ProcessorTypes::kIntensityPiecewiseLinear);
		auto enhancementBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(enhancement->Apply(std::move(buf), std::move(enhancementBuffer), outImageInfo), "Apply Flip Failed");

		return m_bufferCore->MakeID(std::move(enhancementBuffer), sid);
	}

	IID CProcessImp::ApplyIntensityBrightnessContrast(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto enhancement = m_processorCore->Active(sid, ProcessorTypes::kIntensityContrastBrightness);
		auto enhancementBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(enhancement->Apply(std::move(buf), std::move(enhancementBuffer), outImageInfo), "Apply Flip Failed");

		return m_bufferCore->MakeID(std::move(enhancementBuffer), sid);
	}

	IID CProcessImp::ApplyIntensityThreshold(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto enhancement = m_processorCore->Active(sid, ProcessorTypes::kIntensityThreshold);
		auto enhancementBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(enhancement->Apply(std::move(buf), std::move(enhancementBuffer), outImageInfo), "Apply Flip Failed");

		return m_bufferCore->MakeID(std::move(enhancementBuffer), sid);
	}

	IID CProcessImp::ApplyMorphologyDilate3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto dilate = m_processorCore->Active(sid, ProcessorTypes::kDilate3x3);
		auto dilateBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(dilate->Apply(std::move(buf), std::move(dilateBuffer), outImageInfo), "Apply Dilate3x3 Failed");

		return m_bufferCore->MakeID(std::move(dilateBuffer), sid);
	}

	IID CProcessImp::ApplyMorphologyDilate5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto dilate = m_processorCore->Active(sid, ProcessorTypes::kDilate5x5);
		auto dilateBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(dilate->Apply(std::move(buf), std::move(dilateBuffer), outImageInfo), "Apply Dilate5x5 Failed");

		return m_bufferCore->MakeID(std::move(dilateBuffer), sid);
	}

	IID CProcessImp::ApplyMorphologyErode3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto erode = m_processorCore->Active(sid, ProcessorTypes::kErode3x3);
		auto erodeBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(erode->Apply(std::move(buf), std::move(erodeBuffer), outImageInfo), "Apply Erode3x3 Failed");

		return m_bufferCore->MakeID(std::move(erodeBuffer), sid);
	}

	IID CProcessImp::ApplyMorphologyErode5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto erode = m_processorCore->Active(sid, ProcessorTypes::kErode5x5);
		auto erodeBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(erode->Apply(std::move(buf), std::move(erodeBuffer), outImageInfo), "Apply Erode5x5 Failed");

		return m_bufferCore->MakeID(std::move(erodeBuffer), sid);
	}

	IID CProcessImp::ApplyMorphologyOpen3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto open = m_processorCore->Active(sid, ProcessorTypes::kOpen3x3);
		auto openBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(open->Apply(std::move(buf), std::move(openBuffer), outImageInfo), "Apply Open3x3 Failed");

		return m_bufferCore->MakeID(std::move(openBuffer), sid);
	}

	IID CProcessImp::ApplyMorphologyOpen5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto open = m_processorCore->Active(sid, ProcessorTypes::kOpen5x5);
		auto openBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(open->Apply(std::move(buf), std::move(openBuffer), outImageInfo), "Apply Open5x5 Failed");

		return m_bufferCore->MakeID(std::move(openBuffer), sid);
	}

	IID CProcessImp::ApplyMorphologyClose3x3(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto close = m_processorCore->Active(sid, ProcessorTypes::kClose3x3);
		auto closeBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(close->Apply(std::move(buf), std::move(closeBuffer), outImageInfo), "Apply Close3x3 Failed");

		return m_bufferCore->MakeID(std::move(closeBuffer), sid);
	}

	IID CProcessImp::ApplyMorphologyClose5x5(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto close = m_processorCore->Active(sid, ProcessorTypes::kClose5x5);
		auto closeBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(close->Apply(std::move(buf), std::move(closeBuffer), outImageInfo), "Apply Close5x5 Failed");

		return m_bufferCore->MakeID(std::move(closeBuffer), sid);
	}

	IID CProcessImp::ApplyTransformRotation(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto rotation = m_processorCore->Active(sid, InnerProcessorTypes::kRotationSepcialAngle);
		auto rotationBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		if (!rotation->Apply(std::move(buf), std::move(rotationBuffer), outImageInfo))
		{
			sid = std::move(m_bufferCore->BufferID(outImageInfo));
			rotationBuffer = std::move(m_bufferCore->AcquireRaw(sid));
			ENSURE_THROW_MSG(rotation->Apply(std::move(buf), std::move(rotationBuffer), outImageInfo), "Apply Rotation Failed");
		}

		return m_bufferCore->MakeID(std::move(rotationBuffer), sid);
	}

	IID CProcessImp::ApplyTransformFlip(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto flip = m_processorCore->Active(sid, InnerProcessorTypes::kFlip);
		auto flipBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(flip->Apply(std::move(buf), std::move(flipBuffer), outImageInfo), "Apply Flip Failed");

		return m_bufferCore->MakeID(std::move(flipBuffer), sid);
	}	

	IID CProcessImp::ApplyFFT(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		auto buf = m_bufferCore->Resume(iid);
		SID sid = iid >> 16;
		auto fft = m_processorCore->Active(sid, InnerProcessorTypes::kFFT);
		auto fftBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		if (!fft->Apply(std::move(buf), std::move(fftBuffer), outImageInfo))
		{
			sid = std::move(m_bufferCore->BufferID(outImageInfo));
			fftBuffer = std::move(m_bufferCore->AcquireRaw(sid));
			ENSURE_THROW_MSG(fft->Apply(std::move(buf), std::move(fftBuffer), outImageInfo), "Apply FFT Failed");
		}

		return m_bufferCore->MakeID(std::move(fftBuffer), sid);
	}

	IID CProcessImp::ApplyAccumulation(std::list<IID> const& iidList)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		ENSURE_THROW_MSG(!iidList.empty(), "IID list is empty");
		std::list<std::any> bufList;
		for (auto iid : iidList)
		{
			bufList.push_back(m_bufferCore->Resume(iid));
		}
		auto iid = iidList.front();
		SID sid = iid >> 16;
		auto accumulation = m_processorCore->Active(sid, InnerProcessorTypes::kAccumulation);
		auto accumulationBuffer = std::move(m_bufferCore->AcquireProcess(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(accumulation->Apply(std::move(bufList), std::move(accumulationBuffer), outImageInfo), "Apply Accumulation Failed");
		
		return m_bufferCore->MakeID(std::move(accumulationBuffer), sid);
	}

	IID CProcessImp::ApplyAverage(std::list<IID> const& iidList)
	{
		std::lock_guard<std::mutex> lock(m_applyMutex);
		ENSURE_THROW_MSG(!iidList.empty(), "IID list is empty");
		std::list<std::any> bufList;
		for (auto iid : iidList)
		{
			bufList.push_back(m_bufferCore->Resume(iid));
		}
		auto iid = iidList.front();
		SID sid = iid >> 16;
		auto average = m_processorCore->Active(sid, InnerProcessorTypes::kAverage);
		auto averageBuffer = std::move(m_bufferCore->AcquireRaw(sid));
		ELDER::ImageInfo outImageInfo;
		ENSURE_THROW_MSG(average->Apply(std::move(bufList), std::move(averageBuffer), outImageInfo), "Apply Average Failed");

		return m_bufferCore->MakeID(std::move(averageBuffer), sid);
	}
}
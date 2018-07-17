#include "ShaperProcessorImp.h"

#include "ShaperFilterImp.h"
#include "ShaperIntensityImp.h"
#include "ShaperMorphologyImp.h"
#include "ShaperTransformImp.h"
#include "ShaperFFTImp.h"

namespace SHAPER
{
	CProcessorImp::CProcessorImp(bool enable, int num)
		: CSuperProcessor()
		, m_isMultiCoreEnable(enable)
		, m_multiCoreNum(num)
		, m_filterImp(std::make_shared<CFilterImp>())
		, m_intensityImp(std::make_shared<CIntensityImp>())
		, m_morphologyImp(std::make_shared<CMorphologyImp>())
		, m_transformImp(std::make_shared<CTransformImp>())
		, m_fftImp(std::make_shared<CFFTImp>())
	{
	
	}

	CProcessorImp::~CProcessorImp()
	{
		m_availableProcessorMap.swap(std::map<SID, std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>>());
		m_availableProcessorMultiCoreDeque.swap(std::map<SID, std::deque<std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>>>());
		m_availableInnerProcessorMap.swap(std::map<SID, std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>>());
	}

	void CProcessorImp::Initialize(SID sid, ELDER::ImageInfo const& imageInfo)
	{
		if (m_sidMap.find(imageInfo) == m_sidMap.end())
		{
			/// Load available processors
			m_availableProcessorMap.emplace(sid, LoadProcessors(imageInfo));
			
			/// Load available core processors
			m_availableInnerProcessorMap.emplace(sid, LoadInnerProcessors(imageInfo));
			
			if (m_isMultiCoreEnable)
			{
				std::deque<std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>> processorMultiQueue;
				for (auto i = 0; i < m_multiCoreNum; ++i)
				{
					processorMultiQueue.push_back(LoadProcessors(imageInfo));
				}
				m_availableProcessorMultiCoreDeque.emplace(sid, std::move(processorMultiQueue));
			}

			m_sidMap.emplace(imageInfo, sid);
		}
	}

	void CProcessorImp::UnInitialize()
	{

	}

	std::list<std::shared_ptr<ELDER::CImageProcessor>> CProcessorImp::Active(SID sid, std::list<ProcessorTypes> const& processorTypeList)
	{
		ENSURE_THROW_MSG(m_availableProcessorMap.find(sid) != m_availableProcessorMap.end(), "Can not find such sid");

		auto const& availableProcessorMap = m_availableProcessorMap.at(sid);
		std::list<std::shared_ptr<ELDER::CImageProcessor>> activeProcessorProcList;
		for (const auto processorType : processorTypeList)
		{
			activeProcessorProcList.push_back(availableProcessorMap.at(processorType));
		}

		return std::move(activeProcessorProcList);
	}

	std::shared_ptr<ELDER::CImageProcessor> const& CProcessorImp::Active(SID sid, ProcessorTypes processorType)
	{
		ENSURE_THROW_MSG(m_availableProcessorMap.find(sid) != m_availableProcessorMap.end(), "Can not find such sid");

		return m_availableProcessorMap.at(sid).at(processorType);
	}

	std::list<std::shared_ptr<ELDER::CImageProcessor>> CProcessorImp::Active(SID sid, std::list<InnerProcessorTypes> const& processorTypeList)
	{
		ENSURE_THROW_MSG(m_availableInnerProcessorMap.find(sid) != m_availableInnerProcessorMap.end(), "Can not find such sid");

		const auto &availableProcessorMap = m_availableInnerProcessorMap.at(sid);
		std::list<std::shared_ptr<ELDER::CImageProcessor>> activeProcessorProcList;
		for (const auto processorType : processorTypeList)
		{
			activeProcessorProcList.push_back(availableProcessorMap.at(processorType));
		}

		return std::move(activeProcessorProcList);
	}

	std::shared_ptr<ELDER::CImageProcessor> const& CProcessorImp::Active(SID sid, InnerProcessorTypes processorType)
	{
		ENSURE_THROW_MSG(m_availableInnerProcessorMap.find(sid) != m_availableInnerProcessorMap.end(), "Can not find such sid");

		return m_availableInnerProcessorMap.at(sid).at(processorType);
	}

	std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CProcessorImp::PopMultiCore(SID sid, std::list<ProcessorTypes> const& processorTypeList)
	{
		ENSURE_THROW_MSG(m_availableProcessorMultiCoreDeque.find(sid) != m_availableProcessorMultiCoreDeque.end(), "Can not find such sid");
		ENSURE_THROW_MSG(m_isMultiCoreEnable, "Not Supported, as disabled the multi core processing");

		auto& availableProcessorMultiCoreDeque = m_availableProcessorMultiCoreDeque.at(sid);
		while (availableProcessorMultiCoreDeque.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		auto availableProcessorMultiCoreMap = availableProcessorMultiCoreDeque.front();
		availableProcessorMultiCoreDeque.pop_front();

		return std::move(availableProcessorMultiCoreMap);
	}

	void CProcessorImp::PushMultiCore(SID sid, std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>&& availableProcessorMultiCoreMap)
	{
		ENSURE_THROW_MSG(m_availableProcessorMultiCoreDeque.find(sid) != m_availableProcessorMultiCoreDeque.end(), "Can not find such sid");
		ENSURE_THROW_MSG(m_isMultiCoreEnable, "Not Supported, as disabled the multi core processing");

		auto& availableProcessorMultiCoreDeque = m_availableProcessorMultiCoreDeque.at(sid);
		availableProcessorMultiCoreDeque.push_back(std::move(availableProcessorMultiCoreMap));
	}

	CFilter* CProcessorImp::Filter() const
	{
		ASSERT_LOG(m_filterImp != nullptr, "m_filterImp is null");

		return dynamic_cast<CFilter *>(m_filterImp.get());
	}

	CTransform* CProcessorImp::Transform() const
	{
		ASSERT_LOG(m_transformImp != nullptr, "m_transformImp is null");

		return dynamic_cast<CTransform *>(m_transformImp.get());
	}

	CIntensity* CProcessorImp::Intensity() const
	{
		ASSERT_LOG(m_intensityImp != nullptr, "m_flipImp is null");

		return dynamic_cast<CIntensity *>(m_intensityImp.get());
	}

	std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CProcessorImp::LoadProcessors(ELDER::ImageInfo const& imageInfo)
	{
		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> availableProcessors;

		auto availabelIntensitys = m_intensityImp->Load(imageInfo);
		availableProcessors.insert(availabelIntensitys.begin(), availabelIntensitys.end());

		auto availableFilters = m_filterImp->Load(imageInfo);
		availableProcessors.insert(availableFilters.begin(), availableFilters.end());

		auto availableMorphologys = m_morphologyImp->Load(imageInfo);
		availableProcessors.insert(availableMorphologys.begin(), availableMorphologys.end());

		return availableProcessors;
	}

	std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CProcessorImp::LoadInnerProcessors(ELDER::ImageInfo const& imageInfo)
	{
		std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> availableInnerProcessors;

		auto availabelIntensityCores = m_intensityImp->LoadInner(imageInfo);
		availableInnerProcessors.insert(availabelIntensityCores.begin(), availabelIntensityCores.end());

		auto availableTransformCores = m_transformImp->LoadInner(imageInfo);
		availableInnerProcessors.insert(availableTransformCores.begin(), availableTransformCores.end());

		auto availableFftCores = m_fftImp->LoadInner(imageInfo);
		availableInnerProcessors.insert(availableFftCores.begin(), availableFftCores.end());

		return availableInnerProcessors;
	}
}
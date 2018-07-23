#include "ShaperCorrectionImp.h"

namespace SHAPER
{
	CCorrectionImp::CCorrectionImp()
		: CSuperCorrection()
	{
	}

	CCorrectionImp::~CCorrectionImp()
	{
		m_darkRefList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::CORRECTION::CDarkRef>>());
		m_darkGainRefList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::CORRECTION::CDarkGainRef>>());
		m_crossCorrNormList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal>>());
	}

	std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CCorrectionImp::Load(ELDER::ImageInfo const& imageInfo)
	{
		ASSERT_LOG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> availableCorrectionMap;
		
		if (imageInfo.bitDepth == 8)
		{
			auto darkRef8u1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CDarkRef8u1c>();
			darkRef8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> darkRef8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CDarkRef8u1c::Apply, darkRef8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableCorrectionMap.emplace(ProcessorTypes::kDarkRef, std::move(darkRef8u1cPro));
			m_darkRefList.push_back(darkRef8u1c);

			auto darkGainRef8u1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CDarkGainRef8u1c>();
			darkGainRef8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> darkGainRef8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CDarkGainRef8u1c::Apply, darkGainRef8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableCorrectionMap.emplace(ProcessorTypes::kDarkGainRef, std::move(darkGainRef8u1cPro));
			m_darkGainRefList.push_back(darkGainRef8u1c);

			auto ncc8u1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal8u1c>();
			ncc8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> ncc8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal8u1c::Apply, ncc8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableCorrectionMap.emplace(ProcessorTypes::kCrossCorrelationNormal, std::move(ncc8u1cPro));
			m_crossCorrNormList.push_back(ncc8u1c);
		}
		else if (imageInfo.bitDepth == 16)
		{
			auto darkRef16u1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CDarkRef16u1c>();
			darkRef16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> darkRef16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CDarkRef16u1c::Apply, darkRef16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableCorrectionMap.emplace(ProcessorTypes::kDarkRef, std::move(darkRef16u1cPro));
			m_darkRefList.push_back(darkRef16u1c);

			auto darkGainRef16u1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CDarkGainRef16u1c>();
			darkGainRef16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> darkGainRef16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CDarkGainRef16u1c::Apply, darkGainRef16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableCorrectionMap.emplace(ProcessorTypes::kDarkGainRef, std::move(darkGainRef16u1cPro));
			m_darkGainRefList.push_back(darkGainRef16u1c);

			auto ncc16u1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal16u1c>();
			ncc16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> ncc16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal16u1c::Apply, ncc16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableCorrectionMap.emplace(ProcessorTypes::kCrossCorrelationNormal, std::move(ncc16u1cPro));
			m_crossCorrNormList.push_back(ncc16u1c);
		}
		else if (imageInfo.bitDepth == 32)
		{
			auto darkRef32f1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CDarkRef32f1c>();
			darkRef32f1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> darkRef32f1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CDarkRef32f1c::Apply, darkRef32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableCorrectionMap.emplace(ProcessorTypes::kDarkRef, std::move(darkRef32f1cPro));
			m_darkRefList.push_back(darkRef32f1c);

			auto darkGainRef32f1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CDarkGainRef32f1c>();
			darkGainRef32f1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> darkGainRef32f1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CDarkGainRef32f1c::Apply, darkGainRef32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableCorrectionMap.emplace(ProcessorTypes::kDarkGainRef, std::move(darkGainRef32f1cPro));
			m_darkGainRefList.push_back(darkGainRef32f1c);

			auto accu32f1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal32f1c>();
			accu32f1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> accu32f1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal32f1c::Apply, accu32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableCorrectionMap.emplace(ProcessorTypes::kCrossCorrelationNormal, std::move(accu32f1cPro));
			m_crossCorrNormList.push_back(accu32f1c);
		}

		return availableCorrectionMap;
	}

// 	std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CCorrectionImp::LoadInner(ELDER::ImageInfo const& imageInfo)
// 	{
// 		ASSERT_LOG(imageInfo.bitDepth == 8 ||
// 			imageInfo.bitDepth == 16 ||
// 			imageInfo.bitDepth == 32,
// 			"Unsupported bit depth");
// 
// 		// processor core types
// 		std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> availableCorrectionCoreMap;
// 		if (imageInfo.bitDepth == 8)
// 		{
// 			auto ncc8u1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal8u1c>();
// 			ncc8u1c->Initialize(imageInfo.size);
// 			std::shared_ptr<ELDER::CImageProcessor> ncc8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal8u1c::Apply, ncc8u1c, std::placeholders::_1, std::placeholders::_2)));
// 			availableCorrectionCoreMap.emplace(InnerProcessorTypes::kAccumulation, std::move(ncc8u1cPro));
// 			m_crossCorrNormList.push_back(ncc8u1c);
// 		}
// 		else if (imageInfo.bitDepth == 16)
// 		{
// 			auto ncc16u1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal16u1c>();
// 			ncc16u1c->Initialize(imageInfo.size);
// 			std::shared_ptr<ELDER::CImageProcessor> ncc16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal16u1c::Apply, ncc16u1c, std::placeholders::_1, std::placeholders::_2)));
// 			availableCorrectionCoreMap.emplace(InnerProcessorTypes::kAccumulation, std::move(ncc16u1cPro));
// 			m_crossCorrNormList.push_back(ncc16u1c);
// 		}
// 		else
// 		{
// 			auto accu32f1c = std::make_shared<ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal32f1c>();
// 			accu32f1c->Initialize(imageInfo.size);
// 			std::shared_ptr<ELDER::CImageProcessor> accu32f1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal32f1c::Apply, accu32f1c, std::placeholders::_1, std::placeholders::_2)));
// 			availableCorrectionCoreMap.emplace(InnerProcessorTypes::kAccumulation, std::move(accu32f1cPro));
// 			m_crossCorrNormList.push_back(accu32f1c);
// 		}
// 
// 		return availableCorrectionCoreMap;
// 	}

	void CCorrectionImp::SetDarkReference(float const*const darkRefImage)
	{
		for (const auto& darkRef : m_darkRefList)
		{
			darkRef->SetReference(darkRefImage);
		}
	}

	void CCorrectionImp::SetDarkGainReferences(float const*const darkRefImage, float const*const gainRefImage)
	{
		for (const auto& darkGainRef : m_darkGainRefList)
		{
			darkGainRef->SetReferences(darkRefImage, gainRefImage);
		}
	}

	void CCorrectionImp::SetTemplateReference(void const*const templateRefImage)
	{
		for (const auto& crossCorrNorm : m_crossCorrNormList)
		{
			crossCorrNorm->SetTemplateReference(templateRefImage);
		}
	}
}

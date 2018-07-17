#include "ShaperIntensityImp.h"

namespace SHAPER
{
	CIntensityImp::CIntensityImp()
		: CSuperIntensity()
	{
	}

	CIntensityImp::~CIntensityImp()
	{
		m_histEqList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CHistogramEqualization>>());
		m_histLocalList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CHistogramLocal>>());
		m_negativeList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CNegative>>());
		m_logList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CLog>>());
		m_gammaList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CGamma>>());
		m_linearList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CLinear>>());
		m_piecewiseLinearList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CPiecewiseLinear>>());
		m_contrastBrightnessList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CContrastBrightness>>());
		m_thresholdList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CThreshold>>());
	}

	std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CIntensityImp::Load(ELDER::ImageInfo const& imageInfo)
	{
		ASSERT_LOG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> availableIntensityMap;
		
		if (imageInfo.bitDepth == 8)
		{
			auto histEq8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CHistogramEqualization8u1c>();
			histEq8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> histEq8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CHistogramEqualization8u1c::Apply, histEq8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kHistogramEqualization, std::move(histEq8u1cPro));
			m_histEqList.push_back(histEq8u1c);

			auto histLocal8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CHistogramLocal8u1c>();
			histLocal8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> histLocal8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CHistogramLocal8u1c::Apply, histLocal8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kHistogramLocal, std::move(histLocal8u1cPro));
			m_histLocalList.push_back(histLocal8u1c);

			auto negative8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CNegative8u1c>();
			negative8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> negative8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CNegative8u1c::Apply, negative8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityNegative, std::move(negative8u1cPro));
			m_negativeList.push_back(negative8u1c);

			auto log8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CLog8u1c>();
			log8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> log8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CLog8u1c::Apply, log8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityLog, std::move(log8u1cPro));
			m_logList.push_back(log8u1c);

			auto gamma8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CGamma8u1c>();
			gamma8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> gamma8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CGamma8u1c::Apply, gamma8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityGamma, std::move(gamma8u1cPro));
			m_gammaList.push_back(gamma8u1c);

			auto linear8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CLinear8u1c>();
			linear8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> inteLinear_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CLinear8u1c::Apply, linear8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityLinear, std::move(inteLinear_8u1c_pro));
			m_linearList.push_back(linear8u1c);

			auto piecewiseLinear8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CPiecewiseLinear8u1c>();
			piecewiseLinear8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> piecewiseLinear8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CPiecewiseLinear8u1c::Apply, piecewiseLinear8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityPiecewiseLinear, std::move(piecewiseLinear8u1cPro));
			m_piecewiseLinearList.push_back(piecewiseLinear8u1c);

			auto contrastBrightness8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CContrastBrightness8u1c>();
			contrastBrightness8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> contrastBrightness8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CContrastBrightness8u1c::Apply, contrastBrightness8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityContrastBrightness, std::move(contrastBrightness8u1cPro));
			m_contrastBrightnessList.push_back(contrastBrightness8u1c);

			auto threshold8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CThreshold8u1c>();
			threshold8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> threshold8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CThreshold8u1c::Apply, threshold8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityThreshold, std::move(threshold8u1cPro));
			m_thresholdList.push_back(threshold8u1c);
		}
		else if (imageInfo.bitDepth == 16)
		{
			auto histEq16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CHistogramEqualization16u1c>();
			histEq16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> histEq16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CHistogramEqualization16u1c::Apply, histEq16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kHistogramEqualization, std::move(histEq16u1cPro));
			m_histEqList.push_back(histEq16u1c);

			auto histLocal16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CHistogramLocal16u1c>();
			histLocal16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> histLocal16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CHistogramLocal16u1c::Apply, histLocal16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kHistogramLocal, std::move(histLocal16u1cPro));
			m_histLocalList.push_back(histLocal16u1c);

			auto negative16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CNegative16u1c>();
			negative16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> negative16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CNegative16u1c::Apply, negative16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityNegative, std::move(negative16u1cPro));
			m_negativeList.push_back(negative16u1c);

			auto log16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CLog16u1c>();
			log16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> log18u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CLog16u1c::Apply, log16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityLog, std::move(log18u1cPro));
			m_logList.push_back(log16u1c);

			auto gamma16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CGamma16u1c>();
			gamma16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> gamma16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CGamma16u1c::Apply, gamma16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityGamma, std::move(gamma16u1cPro));
			m_gammaList.push_back(gamma16u1c);

			auto linear16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CLinear16u1c>();
			linear16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> linear16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CLinear16u1c::Apply, linear16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityLinear, std::move(linear16u1cPro));
			m_linearList.push_back(linear16u1c);

			auto piecewiseLinear16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CPiecewiseLinear16u1c>();
			piecewiseLinear16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> piecewiseLinear16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CPiecewiseLinear16u1c::Apply, piecewiseLinear16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityPiecewiseLinear, std::move(piecewiseLinear16u1cPro));
			m_piecewiseLinearList.push_back(piecewiseLinear16u1c);

			auto contrastBrightness16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CContrastBrightness16u1c>();
			contrastBrightness16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> contrastBrightness16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CContrastBrightness16u1c::Apply, contrastBrightness16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityContrastBrightness, std::move(contrastBrightness16u1cPro));
			m_contrastBrightnessList.push_back(contrastBrightness16u1c);

			auto threshold16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CThreshold16u1c>();
			threshold16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> threshold16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CThreshold16u1c::Apply, threshold16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityMap.emplace(ProcessorTypes::kIntensityThreshold, std::move(threshold16u1cPro));
			m_thresholdList.push_back(threshold16u1c);
		}

		return availableIntensityMap;
	}

	std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CIntensityImp::LoadInner(ELDER::ImageInfo const& imageInfo)
	{
		ASSERT_LOG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		// processor core types
		std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> availableIntensityCoreMap;
		if (imageInfo.bitDepth == 8)
		{
			auto accu8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CAccumulation8u1c>();
			accu8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> accu8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CAccumulation8u1c::Apply, accu8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityCoreMap.emplace(InnerProcessorTypes::kAccumulation, std::move(accu8u1cPro));
			m_accumulationList.push_back(accu8u1c);

			auto aver8u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CAverage8u1c>();
			aver8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> aver8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CAverage8u1c::Apply, aver8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityCoreMap.emplace(InnerProcessorTypes::kAverage, std::move(aver8u1cPro));
			m_averageList.push_back(aver8u1c);
		}
		else if (imageInfo.bitDepth == 16)
		{
			auto accu16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CAccumulation16u1c>();
			accu16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> accu16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CAccumulation16u1c::Apply, accu16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityCoreMap.emplace(InnerProcessorTypes::kAccumulation, std::move(accu16u1cPro));
			m_accumulationList.push_back(accu16u1c);

			auto aver16u1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CAverage16u1c>();
			aver16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> aver16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CAverage16u1c::Apply, aver16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityCoreMap.emplace(InnerProcessorTypes::kAverage, std::move(aver16u1cPro));
			m_averageList.push_back(aver16u1c);
		}
		else
		{
			auto accu32f1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CAccumulation32f1c>();
			accu32f1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> accu32f1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CAccumulation32f1c::Apply, accu32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityCoreMap.emplace(InnerProcessorTypes::kAccumulation, std::move(accu32f1cPro));
			m_accumulationList.push_back(accu32f1c);

			auto aver32f1c = std::make_shared<ELDER::ALGORITHM::INTENSITY::CAverage32f1c>();
			aver32f1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> aver32f1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::INTENSITY::CAverage32f1c::Apply, aver32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableIntensityCoreMap.emplace(InnerProcessorTypes::kAverage, std::move(aver32f1cPro));
			m_averageList.push_back(aver32f1c);
		}

		return availableIntensityCoreMap;
	}

	void CIntensityImp::SetGammaValue(float gammaValue)
	{
		for (const auto& gamma : m_gammaList)
		{
			gamma->SetGammaValue(gammaValue);
		}
	}

	void CIntensityImp::SetLogValue(double logValue)
	{
		for (const auto& log : m_logList)
		{
			log->SetValue(logValue);
		}
	}

	void CIntensityImp::SetLinearPoint(int lowIn, int lowOut, int highIn, int highOut)
	{
		for (const auto& linear : m_linearList)
		{
			linear->SetPoint(lowIn, lowOut, highIn, highOut);
		}
	}

	void CIntensityImp::SetLinearPoint(int lowIn, int highIn)
	{
		for (const auto& linear : m_linearList)
		{
			linear->SetPoint(lowIn, highIn);
		}
	}

	void CIntensityImp::SetPiecewiseLinearPoint(int lowIn, int lowOut, int highIn, int highOut)
	{
		for (const auto& piecewiseLinear : m_piecewiseLinearList)
		{
			piecewiseLinear->SetPiecewisePoint(lowIn, lowOut, highIn, highOut);
		}
	}

	void CIntensityImp::SetContrastBrightness(double contrast, double brightness)
	{
		for (const auto& contrastBrightness : m_contrastBrightnessList)
		{
			contrastBrightness->SetContrastBrightness(contrast, brightness);
		}
	}

	void CIntensityImp::SetThreshold(int thresholdLT, int valueLT, int thresholdGT, int valueGT)
	{
		for (const auto& threshold : m_thresholdList)
		{
			threshold->SetThresholdValue(thresholdLT, valueLT, thresholdGT, valueGT);
		}
	}
}

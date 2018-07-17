#include "ShaperFFTImp.h"

namespace SHAPER
{
	CFFTImp::CFFTImp()
		: CSuperFFT()
	{
	}

	CFFTImp::~CFFTImp()
	{
		m_fftList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::CFFT>>());
	}

	std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CFFTImp::Load(ELDER::ImageInfo const&)
	{
		return std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>();
	}

	std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CFFTImp::LoadInner(ELDER::ImageInfo const& imageInfo)
	{
		ASSERT_LOG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> availableFFTCoreMap;
		if (imageInfo.bitDepth == 8)
		{
			auto fft8u1c = std::make_shared<ELDER::ALGORITHM::CFFT8u1c>();
			fft8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> fft8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CFFT8u1c::Apply, fft8u1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableFFTCoreMap.emplace(InnerProcessorTypes::kFFT, std::move(fft8u1cPro));
			m_fftList.push_back(fft8u1c);
		}
		else if (imageInfo.bitDepth == 16)
		{
			auto fft16u1c = std::make_shared<ELDER::ALGORITHM::CFFT16u1c>();
			fft16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> fft16u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CFFT16u1c::Apply, fft16u1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableFFTCoreMap.emplace(InnerProcessorTypes::kFFT, std::move(fft16u1cPro));
			m_fftList.push_back(fft16u1c);
		}
		else if (imageInfo.bitDepth == 32)
		{
			auto fft32f1c = std::make_shared<ELDER::ALGORITHM::CFFT32f1c>();
			fft32f1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> fft32f1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::CFFT32f1c::Apply, fft32f1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableFFTCoreMap.emplace(InnerProcessorTypes::kFFT, std::move(fft32f1cPro));
			m_fftList.push_back(fft32f1c);
		}
		
		return availableFFTCoreMap;
	}
}

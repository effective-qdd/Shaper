#include "ShaperFilterImp.h"

namespace SHAPER
{
	CFilterImp::CFilterImp()
		: CSuperFilter()
	{
	}

	CFilterImp::~CFilterImp()
	{
		m_medianList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CMedian>>());
		m_gaussianList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CGaussian>>());
		m_meanList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CMean>>());
		m_maximumList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CMaximum>>());
		m_minimumList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CMinimum>>());
		m_laplaceList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CLaplace>>());
		m_highPassList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CHighPass>>());
		m_lowPassList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CLowPass>>());
		m_bilateralList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CBilateral>>());
		m_sobelList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CSobel>>());
	}

	std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CFilterImp::Load(ELDER::ImageInfo const& imageInfo)
	{
		ASSERT_LOG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> availableFilterMap;
		if (imageInfo.bitDepth == 8)
		{
			auto median3x3_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMedian8u1c>();
			median3x3_8u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> median3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMedian8u1c::Apply, median3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMedian3x3, std::move(median3x3_8u1c_pro));
			m_medianList.push_back(median3x3_8u1c);

			auto median5x5_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMedian8u1c>();
			median5x5_8u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> median5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMedian8u1c::Apply, median5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMedian5x5, std::move(median5x5_8u1c_pro));
			m_medianList.push_back(median5x5_8u1c);

			auto gaussian3x3_8u1c = (std::make_shared<ELDER::ALGORITHM::FILTER::CGaussian8u1c>());
			gaussian3x3_8u1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> gaussian3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CGaussian8u1c::Apply, gaussian3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kGaussian3x3, std::move(gaussian3x3_8u1c_pro));
			m_gaussianList.push_back(gaussian3x3_8u1c);

			auto gaussian5x5_8u1c = (std::make_shared<ELDER::ALGORITHM::FILTER::CGaussian8u1c>());
			gaussian5x5_8u1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> gaussian5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CGaussian8u1c::Apply, gaussian5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kGaussian5x5, std::move(gaussian5x5_8u1c_pro));
			m_gaussianList.push_back(gaussian5x5_8u1c);

			auto mean3x3_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMean8u1c>();
			mean3x3_8u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> mean3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMean8u1c::Apply, mean3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMean3x3, std::move(mean3x3_8u1c_pro));
			m_meanList.push_back(mean3x3_8u1c);

			auto mean5x5_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMean8u1c>();
			mean5x5_8u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> mean5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMean8u1c::Apply, mean5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMean5x5, std::move(mean5x5_8u1c_pro));
			m_meanList.push_back(mean5x5_8u1c);

			auto maximum3x3_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMaximum8u1c>();
			maximum3x3_8u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> maximum3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMaximum8u1c::Apply, maximum3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMaximum3x3, std::move(maximum3x3_8u1c_pro));
			m_maximumList.push_back(maximum3x3_8u1c);

			auto maximum5x5_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMaximum8u1c>();
			maximum5x5_8u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> maximum5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMaximum8u1c::Apply, maximum5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMaximum5x5, std::move(maximum5x5_8u1c_pro));
			m_maximumList.push_back(maximum5x5_8u1c);

			auto minimum3x3_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMinimum8u1c>();
			minimum3x3_8u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> minimum3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMinimum8u1c::Apply, minimum3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMinimum3x3, std::move(minimum3x3_8u1c_pro));
			m_minimumList.push_back(minimum3x3_8u1c);

			auto minimum5x5_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMinimum8u1c>();
			minimum5x5_8u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> minimum5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMinimum8u1c::Apply, minimum5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMinimum5x5, std::move(minimum5x5_8u1c_pro));
			m_minimumList.push_back(minimum5x5_8u1c);

			auto laplace3x3_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLaplace8u1c>();
			laplace3x3_8u1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> laplace3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLaplace8u1c::Apply, laplace3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLaplace3x3, std::move(laplace3x3_8u1c_pro));
			m_laplaceList.push_back(laplace3x3_8u1c);

			auto laplace5x5_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLaplace8u1c>();
			laplace5x5_8u1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> laplace5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLaplace8u1c::Apply, laplace5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLaplace5x5, std::move(laplace5x5_8u1c_pro));
			m_laplaceList.push_back(laplace5x5_8u1c);

			auto highPass3x3_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CHighPass8u1c>();
			highPass3x3_8u1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> highPass3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CHighPass8u1c::Apply, highPass3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kHighPass3x3, std::move(highPass3x3_8u1c_pro));
			m_highPassList.push_back(highPass3x3_8u1c);

			auto highPass5x5_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CHighPass8u1c>();
			highPass5x5_8u1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> highPass5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CHighPass8u1c::Apply, highPass5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kHighPass5x5, std::move(highPass5x5_8u1c_pro));
			m_highPassList.push_back(highPass5x5_8u1c);

			auto lowPass3x3_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLowPass8u1c>();
			lowPass3x3_8u1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> lowPass3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLowPass8u1c::Apply, lowPass3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLowPass3x3, std::move(lowPass3x3_8u1c_pro));
			m_lowPassList.push_back(lowPass3x3_8u1c);

			auto lowPass5x5_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLowPass8u1c>();
			lowPass5x5_8u1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> lowPass5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLowPass8u1c::Apply, lowPass5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLowPass5x5, std::move(lowPass5x5_8u1c_pro));
			m_lowPassList.push_back(lowPass5x5_8u1c);

			auto bilateral_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CBilateral8u1c>();
			bilateral_8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> bilateral_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CBilateral8u1c::Apply, bilateral_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::KBilateral, std::move(bilateral_8u1c_pro));
			m_bilateralList.push_back(bilateral_8u1c);

			auto sobel_8u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CSobel8u1c>();
			sobel_8u1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> sobel_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CSobel8u1c::Apply, sobel_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::KSobel3x3, std::move(sobel_8u1c_pro));
			m_sobelList.push_back(sobel_8u1c);
		}
		else if (imageInfo.bitDepth == 16)
		{
			auto median3x3_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMedian16u1c>();
			median3x3_16u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> median3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMedian16u1c::Apply, median3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMedian3x3, std::move(median3x3_16u1c_pro));
			m_medianList.push_back(median3x3_16u1c);

			auto median5x5_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMedian16u1c>();
			median5x5_16u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> median5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMedian16u1c::Apply, median5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMedian5x5, std::move(median5x5_16u1c_pro));
			m_medianList.push_back(median5x5_16u1c);

			auto gaussian3x3_16u1c = (std::make_shared<ELDER::ALGORITHM::FILTER::CGaussian16u1c>());
			gaussian3x3_16u1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> gaussian3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CGaussian16u1c::Apply, gaussian3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kGaussian3x3, std::move(gaussian3x3_16u1c_pro));
			m_gaussianList.push_back(gaussian3x3_16u1c);

			auto gaussian5x5_16u1c = (std::make_shared<ELDER::ALGORITHM::FILTER::CGaussian16u1c>());
			gaussian5x5_16u1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> gaussian5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CGaussian16u1c::Apply, gaussian5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kGaussian5x5, std::move(gaussian5x5_16u1c_pro));
			m_gaussianList.push_back(gaussian5x5_16u1c);

			auto mean3x3_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMean16u1c>();
			mean3x3_16u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> mean3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMean16u1c::Apply, mean3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMean3x3, std::move(mean3x3_16u1c_pro));
			m_meanList.push_back(mean3x3_16u1c);

			auto mean5x5_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMean16u1c>();
			mean5x5_16u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> mean5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMean16u1c::Apply, mean5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMean5x5, std::move(mean5x5_16u1c_pro));
			m_meanList.push_back(mean5x5_16u1c);

			auto maximum3x3_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMaximum16u1c>();
			maximum3x3_16u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> maximum3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMaximum16u1c::Apply, maximum3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMaximum3x3, std::move(maximum3x3_16u1c_pro));
			m_maximumList.push_back(maximum3x3_16u1c);

			auto maximum5x5_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMaximum16u1c>();
			maximum5x5_16u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> maximum5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMaximum16u1c::Apply, maximum5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMaximum5x5, std::move(maximum5x5_16u1c_pro));
			m_maximumList.push_back(maximum5x5_16u1c);

			auto minimum3x3_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMinimum16u1c>();
			minimum3x3_16u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> minimum3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMinimum16u1c::Apply, minimum3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMinimum3x3, std::move(minimum3x3_16u1c_pro));
			m_minimumList.push_back(minimum3x3_16u1c);

			auto minimum5x5_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMinimum16u1c>();
			minimum5x5_16u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> minimum5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMinimum16u1c::Apply, minimum5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMinimum5x5, std::move(minimum5x5_16u1c_pro));
			m_minimumList.push_back(minimum5x5_16u1c);

			auto laplace3x3_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLaplace16u1c>();
			laplace3x3_16u1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> laplace3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLaplace16u1c::Apply, laplace3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLaplace3x3, std::move(laplace3x3_16u1c_pro));
			m_laplaceList.push_back(laplace3x3_16u1c);

			auto laplace5x5_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLaplace16u1c>();
			laplace5x5_16u1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> laplace6x6_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLaplace16u1c::Apply, laplace5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLaplace5x5, std::move(laplace6x6_16u1c_pro));
			m_laplaceList.push_back(laplace5x5_16u1c);

			auto highPass3x3_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CHighPass16u1c>();
			highPass3x3_16u1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> highPass3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CHighPass16u1c::Apply, highPass3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kHighPass3x3, std::move(highPass3x3_16u1c_pro));
			m_highPassList.push_back(highPass3x3_16u1c);

			auto highPass5x5_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CHighPass16u1c>();
			highPass5x5_16u1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> highPass5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CHighPass16u1c::Apply, highPass5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kHighPass5x5, std::move(highPass5x5_16u1c_pro));
			m_highPassList.push_back(highPass5x5_16u1c);

			auto lowPass3x3_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLowPass16u1c>();
			lowPass3x3_16u1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> lowPass3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLowPass16u1c::Apply, lowPass3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLowPass3x3, std::move(lowPass3x3_16u1c_pro));
			m_lowPassList.push_back(lowPass3x3_16u1c);

			auto lowPass5x5_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLowPass16u1c>();
			lowPass5x5_16u1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> lowPass5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLowPass16u1c::Apply, lowPass5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLowPass5x5, std::move(lowPass5x5_16u1c_pro));
			m_lowPassList.push_back(lowPass5x5_16u1c);

			auto bilateral_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CBilateral16u1c>();
			bilateral_16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> bilateral_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CBilateral16u1c::Apply, bilateral_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::KBilateral, std::move(bilateral_16u1c_pro));
			m_bilateralList.push_back(bilateral_16u1c);

			auto sobel_16u1c = std::make_shared<ELDER::ALGORITHM::FILTER::CSobel16u1c>();
			sobel_16u1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> sobel_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CSobel16u1c::Apply, sobel_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::KSobel3x3, std::move(sobel_16u1c_pro));
			m_sobelList.push_back(sobel_16u1c);
		}
		else if (imageInfo.bitDepth == 32)
		{
			auto median3x3_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMedian32f1c>();
			median3x3_32f1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> median3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMedian32f1c::Apply, median3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMedian3x3, std::move(median3x3_32f1c_pro));
			m_medianList.push_back(median3x3_32f1c);

			auto median5x5_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMedian32f1c>();
			median5x5_32f1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> median5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMedian32f1c::Apply, median5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMedian5x5, std::move(median5x5_32f1c_pro));
			m_medianList.push_back(median5x5_32f1c);

			auto gaussian3x3_32f1c = (std::make_shared<ELDER::ALGORITHM::FILTER::CGaussian32f1c>());
			gaussian3x3_32f1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> gaussian3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CGaussian32f1c::Apply, gaussian3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kGaussian3x3, std::move(gaussian3x3_32f1c_pro));
			m_gaussianList.push_back(gaussian3x3_32f1c);

			auto gaussian5x5_32f1c = (std::make_shared<ELDER::ALGORITHM::FILTER::CGaussian32f1c>());
			gaussian5x5_32f1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> gaussian5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CGaussian32f1c::Apply, gaussian5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kGaussian5x5, std::move(gaussian5x5_32f1c_pro));
			m_gaussianList.push_back(gaussian5x5_32f1c);

			auto mean3x3_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMean32f1c>();
			mean3x3_32f1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> mean3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMean32f1c::Apply, mean3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMean3x3, std::move(mean3x3_32f1c_pro));
			m_meanList.push_back(mean3x3_32f1c);

			auto mean5x5_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMean32f1c>();
			mean5x5_32f1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> mean5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMean32f1c::Apply, mean5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMean5x5, std::move(mean5x5_32f1c_pro));
			m_meanList.push_back(mean5x5_32f1c);

			auto maximum3x3_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMaximum32f1c>();
			maximum3x3_32f1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> maximum3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMaximum32f1c::Apply, maximum3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMaximum3x3, std::move(maximum3x3_32f1c_pro));
			m_maximumList.push_back(maximum3x3_32f1c);

			auto maximum5x5_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMaximum32f1c>();
			maximum5x5_32f1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> maximum5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMaximum32f1c::Apply, maximum5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMaximum5x5, std::move(maximum5x5_32f1c_pro));
			m_maximumList.push_back(maximum5x5_32f1c);

			auto minimum3x3_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMinimum32f1c>();
			minimum3x3_32f1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> minimum3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMinimum32f1c::Apply, minimum3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMinimum3x3, std::move(minimum3x3_32f1c_pro));
			m_minimumList.push_back(minimum3x3_32f1c);

			auto minimum5x5_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CMinimum32f1c>();
			minimum5x5_32f1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> minimum5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CMinimum32f1c::Apply, minimum5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kMinimum5x5, std::move(minimum5x5_32f1c_pro));
			m_minimumList.push_back(minimum5x5_32f1c);

			auto laplace3x3_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLaplace32f1c>();
			laplace3x3_32f1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> laplace3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLaplace32f1c::Apply, laplace3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLaplace3x3, std::move(laplace3x3_32f1c_pro));
			m_laplaceList.push_back(laplace3x3_32f1c);

			auto laplace5x5_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLaplace32f1c>();
			laplace5x5_32f1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> laplace5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLaplace32f1c::Apply, laplace5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLaplace5x5, std::move(laplace5x5_32f1c_pro));
			m_laplaceList.push_back(laplace5x5_32f1c);

			auto highPass3x3_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CHighPass32f1c>();
			highPass3x3_32f1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> highPass3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CHighPass32f1c::Apply, highPass3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kHighPass3x3, std::move(highPass3x3_32f1c_pro));
			m_highPassList.push_back(highPass3x3_32f1c);

			auto highPass5x5_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CHighPass32f1c>();
			highPass5x5_32f1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> highPass5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CHighPass32f1c::Apply, highPass5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kHighPass5x5, std::move(highPass5x5_32f1c_pro));
			m_highPassList.push_back(highPass5x5_32f1c);

			auto lowPass3x3_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLowPass32f1c>();
			lowPass3x3_32f1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> lowPass3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLowPass32f1c::Apply, lowPass3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLowPass3x3, std::move(lowPass3x3_32f1c_pro));
			m_lowPassList.push_back(lowPass3x3_32f1c);

			auto lowPass5x5_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CLowPass32f1c>();
			lowPass5x5_32f1c->Initialize(imageInfo.size, 5);
			std::shared_ptr<ELDER::CImageProcessor> lowPass5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CLowPass32f1c::Apply, lowPass5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::kLowPass5x5, std::move(lowPass5x5_32f1c_pro));
			m_lowPassList.push_back(lowPass5x5_32f1c);

			auto bilateral_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CBilateral32f1c>();
			bilateral_32f1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> bilateral_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CBilateral32f1c::Apply, bilateral_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::KBilateral, std::move(bilateral_32f1c_pro));
			m_bilateralList.push_back(bilateral_32f1c);

			auto sobel_32f1c = std::make_shared<ELDER::ALGORITHM::FILTER::CSobel32f1c>();
			sobel_32f1c->Initialize(imageInfo.size, 3);
			std::shared_ptr<ELDER::CImageProcessor> sobel_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::FILTER::CSobel32f1c::Apply, sobel_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableFilterMap.emplace(ProcessorTypes::KSobel3x3, std::move(sobel_32f1c_pro));
			m_sobelList.push_back(sobel_32f1c);
		}
	
		return availableFilterMap;
	}

	void CFilterImp::SetGaussianSigma(float sigma)
	{
		for (const auto &gaussian : m_gaussianList)
		{
			gaussian->SetSigma(sigma);
		}
	}

	
}


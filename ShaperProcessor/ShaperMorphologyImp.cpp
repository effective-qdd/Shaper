#include "ShaperMorphologyImp.h"

namespace SHAPER
{
	CMorphologyImp::CMorphologyImp()
		: CSuperMorphology()
	{
	}

	CMorphologyImp::~CMorphologyImp()
	{
		m_dilateList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::MORPHOLOGY::CDilate>>());
		m_erodeList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::MORPHOLOGY::CErode>>());
		m_openList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::MORPHOLOGY::COpen>>());
		m_closeList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::MORPHOLOGY::CClose>>());
	}

	std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CMorphologyImp::Load(ELDER::ImageInfo const& imageInfo)
	{
		ASSERT_LOG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> availableMorphologyMap;
		if (imageInfo.bitDepth == 8)
		{
			auto dilate3x3_8u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CDilate8u1c>();
			dilate3x3_8u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> dilate3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CDilate8u1c::Apply, dilate3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kDilate3x3, std::move(dilate3x3_8u1c_pro));
			m_dilateList.push_back(dilate3x3_8u1c);

			auto dilate5x5_8u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CDilate8u1c>();
			dilate5x5_8u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> dilate5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CDilate8u1c::Apply, dilate5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kDilate5x5, std::move(dilate5x5_8u1c_pro));
			m_dilateList.push_back(dilate5x5_8u1c);

			auto erode3x3_8u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CErode8u1c>();
			erode3x3_8u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> erode3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CErode8u1c::Apply, erode3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kErode3x3, std::move(erode3x3_8u1c_pro));
			m_erodeList.push_back(erode3x3_8u1c);

			auto erode5x5_8u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CErode8u1c>();
			erode5x5_8u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> erode5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CErode8u1c::Apply, erode5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kErode5x5, std::move(erode5x5_8u1c_pro));
			m_erodeList.push_back(erode5x5_8u1c);

			auto open3x3_8u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::COpen8u1c>();
			open3x3_8u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> open3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::COpen8u1c::Apply, open3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kOpen3x3, std::move(open3x3_8u1c_pro));
			m_openList.push_back(open3x3_8u1c);

			auto open5x5_8u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::COpen8u1c>();
			open5x5_8u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> open5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::COpen8u1c::Apply, open5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kOpen5x5, std::move(open5x5_8u1c_pro));
			m_openList.push_back(open5x5_8u1c);

			auto close3x3_8u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CClose8u1c>();
			close3x3_8u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> close3x3_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CClose8u1c::Apply, close3x3_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kClose3x3, std::move(close3x3_8u1c_pro));
			m_closeList.push_back(close3x3_8u1c);

			auto close5x5_8u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CClose8u1c>();
			close5x5_8u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> close5x5_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CClose8u1c::Apply, close5x5_8u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kClose5x5, std::move(close5x5_8u1c_pro));
			m_closeList.push_back(close5x5_8u1c);
		}
		else if (imageInfo.bitDepth == 16)
		{
			auto dilate3x3_16u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CDilate16u1c>();
			dilate3x3_16u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> dilate3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CDilate16u1c::Apply, dilate3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kDilate3x3, std::move(dilate3x3_16u1c_pro));
			m_dilateList.push_back(dilate3x3_16u1c);

			auto dilate5x5_16u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CDilate16u1c>();
			dilate5x5_16u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> dilate5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CDilate16u1c::Apply, dilate5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kDilate5x5, std::move(dilate5x5_16u1c_pro));
			m_dilateList.push_back(dilate5x5_16u1c);

			auto erode3x3_16u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CErode16u1c>();
			erode3x3_16u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> erode3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CErode16u1c::Apply, erode3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kErode3x3, std::move(erode3x3_16u1c_pro));
			m_erodeList.push_back(erode3x3_16u1c);

			auto erode5x5_16u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CErode16u1c>();
			erode5x5_16u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> erode5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CErode16u1c::Apply, erode5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kErode5x5, std::move(erode5x5_16u1c_pro));
			m_erodeList.push_back(erode5x5_16u1c);

			auto open3x3_16u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::COpen16u1c>();
			open3x3_16u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> open3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::COpen16u1c::Apply, open3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kOpen3x3, std::move(open3x3_16u1c_pro));
			m_openList.push_back(open3x3_16u1c);

			auto open5x5_16u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::COpen16u1c>();
			open5x5_16u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> open5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::COpen16u1c::Apply, open5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kOpen5x5, std::move(open5x5_16u1c_pro));
			m_openList.push_back(open5x5_16u1c);

			auto close3x3_16u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CClose16u1c>();
			close3x3_16u1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> close3x3_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CClose16u1c::Apply, close3x3_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kClose3x3, std::move(close3x3_16u1c_pro));
			m_closeList.push_back(close3x3_16u1c);

			auto close5x5_16u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CClose16u1c>();
			close5x5_16u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> close5x5_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CClose16u1c::Apply, close5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kClose5x5, std::move(close5x5_16u1c_pro));
			m_closeList.push_back(close5x5_16u1c);
		}
		else if (imageInfo.bitDepth == 32)
		{
			auto dilate3x3_32f1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CDilate32f1c>();
			dilate3x3_32f1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> dilate3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CDilate32f1c::Apply, dilate3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kDilate3x3, std::move(dilate3x3_32f1c_pro));
			m_dilateList.push_back(dilate3x3_32f1c);

			auto dilate5x5_16u1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CDilate32f1c>();
			dilate5x5_16u1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> dilate5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CDilate32f1c::Apply, dilate5x5_16u1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kDilate5x5, std::move(dilate5x5_32f1c_pro));
			m_dilateList.push_back(dilate5x5_16u1c);

			auto erode3x3_32f1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CErode32f1c>();
			erode3x3_32f1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> erode3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CErode32f1c::Apply, erode3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kErode3x3, std::move(erode3x3_32f1c_pro));
			m_erodeList.push_back(erode3x3_32f1c);

			auto erode5x5_32f1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CErode32f1c>();
			erode5x5_32f1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> erode5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CErode32f1c::Apply, erode5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kErode5x5, std::move(erode5x5_32f1c_pro));
			m_erodeList.push_back(erode5x5_32f1c);

			auto open3x3_32f1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::COpen32f1c>();
			open3x3_32f1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> open3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::COpen32f1c::Apply, open3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kOpen3x3, std::move(open3x3_32f1c_pro));
			m_openList.push_back(open3x3_32f1c);

			auto open5x5_32f1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::COpen32f1c>();
			open5x5_32f1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> open5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::COpen32f1c::Apply, open5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kOpen5x5, std::move(open5x5_32f1c_pro));
			m_openList.push_back(open5x5_32f1c);

			auto close3x3_32f1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CClose32f1c>();
			close3x3_32f1c->Initialize(imageInfo.size, { 3, 3 });
			std::shared_ptr<ELDER::CImageProcessor> close3x3_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CClose32f1c::Apply, close3x3_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kClose3x3, std::move(close3x3_32f1c_pro));
			m_closeList.push_back(close3x3_32f1c);

			auto close5x5_32f1c = std::make_shared<ELDER::ALGORITHM::MORPHOLOGY::CClose32f1c>();
			close5x5_32f1c->Initialize(imageInfo.size, { 5, 5 });
			std::shared_ptr<ELDER::CImageProcessor> close5x5_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::MORPHOLOGY::CClose32f1c::Apply, close5x5_32f1c, std::placeholders::_1, std::placeholders::_2)));
			availableMorphologyMap.emplace(ProcessorTypes::kClose5x5, std::move(close5x5_32f1c_pro));
			m_closeList.push_back(close5x5_32f1c);
		}
		
		return availableMorphologyMap;
	}
}


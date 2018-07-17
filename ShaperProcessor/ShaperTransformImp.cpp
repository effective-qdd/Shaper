#include "ShaperTransformImp.h"

namespace SHAPER
{
	CTransformImp::CTransformImp()
		: CSuperTransform()
	{
	}

	CTransformImp::~CTransformImp()
	{
		m_rotationSpecialList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::TRANSFORM::CRotationSpecial>>());
		m_rotationArbitraryList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::TRANSFORM::CRotationArbitrary>>());
		m_flipList.swap(std::list<std::shared_ptr<ELDER::ALGORITHM::TRANSFORM::CFlip>>());
	}

	std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CTransformImp::Load(ELDER::ImageInfo const& imageInfo)
	{
		return std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>();
	}

	std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> CTransformImp::LoadInner(ELDER::ImageInfo const& imageInfo)
	{
		ASSERT_LOG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> availableTransformMap;
		if (imageInfo.bitDepth == 8)
		{
			auto rotationSpecial8u1c = std::make_shared<ELDER::ALGORITHM::TRANSFORM::CRotationSpecial8u1c>();
			rotationSpecial8u1c->Initialize(imageInfo.size, ELDER::ALGORITHM::TRANSFORM::CRotationSpecial::k90Degrees);
			std::shared_ptr<ELDER::CImageProcessor> rotationSpecial8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::TRANSFORM::CRotationSpecial8u1c::Apply, rotationSpecial8u1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableTransformMap.emplace(InnerProcessorTypes::kRotationSepcialAngle, std::move(rotationSpecial8u1cPro));
			m_rotationSpecialList.push_back(rotationSpecial8u1c);

			auto rotationArbitrary8u1c = std::make_shared<ELDER::ALGORITHM::TRANSFORM::CRotationArbitrary8u1c>();
			rotationArbitrary8u1c->Initialize(imageInfo.size, -90.0);
			std::shared_ptr<ELDER::CImageProcessor> rotationArbitrary8u1cPro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::TRANSFORM::CRotationArbitrary8u1c::Apply, rotationArbitrary8u1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableTransformMap.emplace(InnerProcessorTypes::kRotationArbitraryAngle, std::move(rotationArbitrary8u1cPro));
			m_rotationArbitraryList.push_back(rotationArbitrary8u1c);

			auto flip_8u1c = std::make_shared<ELDER::ALGORITHM::TRANSFORM::CFlip8u1c>();
			flip_8u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> flip_8u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::TRANSFORM::CFlip8u1c::Apply, flip_8u1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableTransformMap.emplace(InnerProcessorTypes::kFlip, std::move(flip_8u1c_pro));
			m_flipList.push_back(flip_8u1c);
		}
		else if (imageInfo.bitDepth == 16)
		{
			auto rotationSpecial_16u1c = std::make_shared<ELDER::ALGORITHM::TRANSFORM::CRotationSpecial16u1c>();
			rotationSpecial_16u1c->Initialize(imageInfo.size, ELDER::ALGORITHM::TRANSFORM::CRotationSpecial::k90Degrees);
			std::shared_ptr<ELDER::CImageProcessor> rotationSpecial_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::TRANSFORM::CRotationSpecial16u1c::Apply, rotationSpecial_16u1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableTransformMap.emplace(InnerProcessorTypes::kRotationSepcialAngle, std::move(rotationSpecial_16u1c_pro));
			m_rotationSpecialList.push_back(rotationSpecial_16u1c);

			auto rotationArbitrary_16u1c = std::make_shared<ELDER::ALGORITHM::TRANSFORM::CRotationArbitrary16u1c>();
			rotationArbitrary_16u1c->Initialize(imageInfo.size, -90.0);
			std::shared_ptr<ELDER::CImageProcessor> rotationArbitrary_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::TRANSFORM::CRotationArbitrary16u1c::Apply, rotationArbitrary_16u1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableTransformMap.emplace(InnerProcessorTypes::kRotationArbitraryAngle, std::move(rotationArbitrary_16u1c_pro));
			m_rotationArbitraryList.push_back(rotationArbitrary_16u1c);

			auto flip_16u1c = std::make_shared<ELDER::ALGORITHM::TRANSFORM::CFlip16u1c>();
			flip_16u1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> flip_16u1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::TRANSFORM::CFlip16u1c::Apply, flip_16u1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableTransformMap.emplace(InnerProcessorTypes::kFlip, std::move(flip_16u1c_pro));
			m_flipList.push_back(flip_16u1c);
		}
		else if (imageInfo.bitDepth == 32)
		{
			auto rotationSpecial_32f1c = std::make_shared<ELDER::ALGORITHM::TRANSFORM::CRotationSpecial32f1c>();
			rotationSpecial_32f1c->Initialize(imageInfo.size, ELDER::ALGORITHM::TRANSFORM::CRotationSpecial::k90Degrees);
			std::shared_ptr<ELDER::CImageProcessor> rotationSpecial_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::TRANSFORM::CRotationSpecial32f1c::Apply, rotationSpecial_32f1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableTransformMap.emplace(InnerProcessorTypes::kRotationSepcialAngle, std::move(rotationSpecial_32f1c_pro));
			m_rotationSpecialList.push_back(rotationSpecial_32f1c);

			auto rotationArbitrary_32f1c = std::make_shared<ELDER::ALGORITHM::TRANSFORM::CRotationArbitrary32f1c>();
			rotationArbitrary_32f1c->Initialize(imageInfo.size, -90.0);
			std::shared_ptr<ELDER::CImageProcessor> rotationArbitrary_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::TRANSFORM::CRotationArbitrary32f1c::Apply, rotationArbitrary_32f1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			m_rotationArbitraryList.push_back(rotationArbitrary_32f1c);
			availableTransformMap.emplace(InnerProcessorTypes::kRotationArbitraryAngle, std::move(rotationArbitrary_32f1c_pro));

			auto flip_32f1c = std::make_shared<ELDER::ALGORITHM::TRANSFORM::CFlip32f1c>();
			flip_32f1c->Initialize(imageInfo.size);
			std::shared_ptr<ELDER::CImageProcessor> flip_32f1c_pro = std::make_shared<ELDER::CImageProcessor>(std::move(std::bind(&ELDER::ALGORITHM::TRANSFORM::CFlip32f1c::Apply, flip_32f1c, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
			availableTransformMap.emplace(InnerProcessorTypes::kFlip, std::move(flip_32f1c_pro));
			m_flipList.push_back(flip_32f1c);
		}

		return availableTransformMap;
	}

	void CTransformImp::SetRotationSpecialAngle(SpecialAngles angle)
	{
		for (const auto &rotationSpecial : m_rotationSpecialList)
		{
			rotationSpecial->SetAngle(static_cast<ELDER::ALGORITHM::TRANSFORM::CRotationSpecial::SpecialAngle>(angle));
		}
	}

	void CTransformImp::SetRotationArbitraryAngle(double angle)
	{
		for (const auto &rotationArbitrary : m_rotationArbitraryList)
		{
			rotationArbitrary->SetAngle(angle);
		}
	}

	void CTransformImp::SetMode(FlipModes flipMode)
	{
		for (const auto &flip : m_flipList)
		{
			flip->SetMode(static_cast<ELDER::ALGORITHM::TRANSFORM::CFlip::FlipModes>(flipMode));
		}
	}
}

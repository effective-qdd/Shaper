#ifndef SHAPER_TRANSFORM_IMP_H
#define SHAPER_TRANSFORM_IMP_H

#include "../../Elder/ImageAlgorithm/ImageTransformRotation.hpp"
#include "../../Elder/ImageAlgorithm/ImageTransformFlip.hpp"

#include "SuperTransform.h"

namespace SHAPER
{
	class CTransformImp : public CSuperTransform
	{
	public:
		CTransformImp();

		~CTransformImp();

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> Load(ELDER::ImageInfo const&);

		std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> LoadInner(ELDER::ImageInfo const&);

		void SetRotationSpecialAngle(SpecialAngles);

		void SetRotationArbitraryAngle(double);

		void SetMode(FlipModes);

	private:
		std::list<std::shared_ptr<ELDER::ALGORITHM::TRANSFORM::CRotationSpecial>> m_rotationSpecialList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::TRANSFORM::CRotationArbitrary>> m_rotationArbitraryList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::TRANSFORM::CFlip>> m_flipList;
	};
}

#endif
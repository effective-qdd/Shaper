#ifndef SHAPER_CORRECTION_IMP_H
#define SHAPER_CORRECTION_IMP_H

#include "../../Elder/ImageAlgorithm/ImageCorrectionDarkRef.hpp"
#include "../../Elder/ImageAlgorithm/ImageCorrectionDarkGainRef.hpp"
#include "../../Elder/ImageAlgorithm/ImageCorrectionCrossCorrelationNormal.hpp"

#include "SuperCorrection.h"

namespace SHAPER
{
	class CCorrectionImp : public CSuperCorrection
	{
	public:
		CCorrectionImp();

		~CCorrectionImp();

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> Load(ELDER::ImageInfo const&);

		//std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> LoadInner(ELDER::ImageInfo const&);

		void SetDarkReference(float const*const);

		void SetDarkGainReferences(float const*const, float const*const);

		void SetTemplateReference(void const*const);

	private:
		std::list<std::shared_ptr<ELDER::ALGORITHM::CORRECTION::CDarkRef>> m_darkRefList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::CORRECTION::CDarkGainRef>> m_darkGainRefList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::CORRECTION::CCorssCorrelationNormal>> m_crossCorrNormList;
	};
}

#endif
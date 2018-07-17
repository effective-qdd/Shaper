#ifndef SHAPER_MORPHOLOGY_IMP_H
#define SHAPER_MORPHOLOGY_IMP_H

#include "../../Elder/ImageAlgorithm/ImageMorphologyDilate.hpp"
#include "../../Elder/ImageAlgorithm/ImageMorphologyErode.hpp"
#include "../../Elder/ImageAlgorithm/ImageMorphologyOpen.hpp"
#include "../../Elder/ImageAlgorithm/ImageMorphologyClose.hpp"

#include "SuperMorphology.h"

namespace SHAPER
{
	class CMorphologyImp : public CSuperMorphology
	{
	public:
		CMorphologyImp();

		~CMorphologyImp();

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> Load(ELDER::ImageInfo const&);

	private:
		std::list<std::shared_ptr<ELDER::ALGORITHM::MORPHOLOGY::CDilate>> m_dilateList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::MORPHOLOGY::CErode>> m_erodeList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::MORPHOLOGY::COpen>> m_openList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::MORPHOLOGY::CClose>> m_closeList;
	};
}

#endif
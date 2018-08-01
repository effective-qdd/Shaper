#ifndef SHAPER_FILTER_IMP_H
#define SHAPER_FILTER_IMP_H

#include "../../Elder/ImageAlgorithm/ImageFilterMedian.hpp"
#include "../../Elder/ImageAlgorithm/ImageFilterGaussian.hpp"
#include "../../Elder/ImageAlgorithm/ImageFilterMean.hpp"
#include "../../Elder/ImageAlgorithm/ImageFilterMaximum.hpp"
#include "../../Elder/ImageAlgorithm/ImageFilterMinimum.hpp"
#include "../../Elder/ImageAlgorithm/ImageFilterLaplace.hpp"
#include "../../Elder/ImageAlgorithm/ImageFilterHighPass.hpp"
#include "../../Elder/ImageAlgorithm/ImageFilterLowPass.hpp"
#include "../../Elder/ImageAlgorithm/ImageFilterBilateral.hpp"
#include "../../Elder/ImageAlgorithm/ImageFilterSobel.hpp"


#include "SuperFilter.h"

namespace SHAPER
{
	class CFilterImp : public CSuperFilter
	{
	public:
		CFilterImp();

		~CFilterImp();

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> Load(ELDER::ImageInfo const&);

		void SetGaussianSigma(float);

		void SetBilateralRadius(int);

		void SetBilateralSigmaColor(double);

		void SetBilateralSigmaSpace(double);

	private:
		std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CMedian>> m_medianList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CGaussian>> m_gaussianList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CMean>> m_meanList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CMaximum>> m_maximumList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CMinimum>> m_minimumList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CLaplace>> m_laplaceList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CHighPass>> m_highPassList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CLowPass>> m_lowPassList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CBilateral>> m_bilateralList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::FILTER::CSobel>> m_sobelList;
	};
}

#endif
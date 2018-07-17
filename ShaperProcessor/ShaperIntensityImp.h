#ifndef SHAPER_INTENSITY_IMP_H
#define SHAPER_INTENSITY_IMP_H

#include "../../Elder/ImageAlgorithm/ImageIntensityHistogramEqual.hpp"
#include "../../Elder/ImageAlgorithm/ImageIntensityHistogramLocal.hpp"
#include "../../Elder/ImageAlgorithm/ImageIntensityNegative.hpp"
#include "../../Elder/ImageAlgorithm/ImageIntensityLog.hpp"
#include "../../Elder/ImageAlgorithm/ImageIntensityGamma.hpp"
#include "../../Elder/ImageAlgorithm/ImageIntensityLinear.hpp"
#include "../../Elder/ImageAlgorithm/ImageIntensityPiecewiseLinear.hpp"
#include "../../Elder/ImageAlgorithm/ImageIntensityContrastBrightness.hpp"
#include "../../Elder/ImageAlgorithm/ImageIntensityThreshold.hpp"
#include "../../Elder/ImageAlgorithm/ImageIntensityAccumulation.hpp"
#include "../../Elder/ImageAlgorithm/ImageIntensityAverage.hpp"

#include "SuperIntensity.h"

namespace SHAPER
{
	class CIntensityImp : public CSuperIntensity
	{
	public:
		CIntensityImp();

		~CIntensityImp();

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> Load(ELDER::ImageInfo const&);

		std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> LoadInner(ELDER::ImageInfo const&);

		void SetGammaValue(float);

		void SetLogValue(double);

		void SetLinearPoint(int, int, int, int);

		void SetLinearPoint(int, int);

		void SetPiecewiseLinearPoint(int, int, int, int);

		void SetContrastBrightness(double, double);

		void SetThreshold(int, int, int, int);

	private:
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CHistogramEqualization>> m_histEqList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CHistogramLocal>> m_histLocalList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CNegative>> m_negativeList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CLog>> m_logList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CGamma>> m_gammaList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CLinear>> m_linearList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CPiecewiseLinear>> m_piecewiseLinearList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CContrastBrightness>> m_contrastBrightnessList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CThreshold>> m_thresholdList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CAccumulation>> m_accumulationList;
		std::list<std::shared_ptr<ELDER::ALGORITHM::INTENSITY::CAverage>> m_averageList;
	};
}

#endif
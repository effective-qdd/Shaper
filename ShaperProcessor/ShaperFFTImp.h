#ifndef SHAPER_FFT_IMP_H
#define SHAPER_FFT_IMP_H

#include "../../Elder/ImageAlgorithm/ImageFFT.hpp"

#include "SuperFFT.h"

namespace SHAPER
{
	class CFFTImp : public CSuperFFT
	{
	public:
		CFFTImp();

		~CFFTImp();

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> Load(ELDER::ImageInfo const&);

		std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> LoadInner(ELDER::ImageInfo const&);

	private:
		std::mutex m_activeMutex;
		std::list<std::shared_ptr<ELDER::ALGORITHM::CFFT>> m_fftList;
	};
}

#endif
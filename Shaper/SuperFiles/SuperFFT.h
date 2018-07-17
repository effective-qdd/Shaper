#ifndef SUPER_FFT_H
#define SUPER_FFT_H

#include "FFT.h"
#include "FFTCore.h"

namespace SHAPER
{
	class CSuperFFT : public CFFT, public CFFTCore
	{
	public:
		CSuperFFT() {}

		virtual ~CSuperFFT() {}
	};
}

#endif
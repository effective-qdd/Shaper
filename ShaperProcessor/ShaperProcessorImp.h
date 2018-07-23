#ifndef SHAPER_PROCESSOR_IMP_H
#define SHAPER_PROCESSOR_IMP_H

#include "SuperProcessor.h"

namespace SHAPER
{
	class CCorrectionImp;
	class CFilterImp;
	class CIntensityImp;
	class CMorphologyImp;
	class CTransformImp;
	class CFFTImp;
	class CProcessorImp final : public CSuperProcessor
	{
	public:
		CProcessorImp(bool, int);

		~CProcessorImp();

		void Initialize(SID, ELDER::ImageInfo const&);

		void UnInitialize();

		std::list<std::shared_ptr<ELDER::CImageProcessor>> Active(SID, std::list<ProcessorTypes> const&);

		std::shared_ptr<ELDER::CImageProcessor> const& Active(SID, ProcessorTypes);

		std::list<std::shared_ptr<ELDER::CImageProcessor>> Active(SID, std::list<InnerProcessorTypes> const&);

		std::shared_ptr<ELDER::CImageProcessor> const& Active(SID, InnerProcessorTypes);

		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> PopMultiCore(SID, std::list<ProcessorTypes> const&);

		void PushMultiCore(SID, std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>&&);

		CFilter* Filter() const;

		CIntensity* Intensity() const;

		CTransform* Transform() const;

		CCorrection* Correction() const;

	private:
		std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> LoadProcessors(ELDER::ImageInfo const&);

		std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> LoadInnerProcessors(ELDER::ImageInfo const&);

	private:
		bool m_isMultiCoreEnable;
		int m_multiCoreNum;
		std::map<ELDER::ImageInfo, SID> m_sidMap;
		std::map<SID, std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>> m_availableProcessorMap;
		std::map<SID, std::deque<std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>>> m_availableProcessorMultiCoreDeque;
		std::map<SID, std::map<InnerProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>> m_availableInnerProcessorMap;
		std::shared_ptr<CCorrectionImp> m_correctionImp;
		std::shared_ptr<CFilterImp> m_filterImp;
		std::shared_ptr<CIntensityImp> m_intensityImp;
		std::shared_ptr<CMorphologyImp> m_morphologyImp;
		std::shared_ptr<CTransformImp> m_transformImp;
		std::shared_ptr<CFFTImp> m_fftImp;
	};
}

#endif
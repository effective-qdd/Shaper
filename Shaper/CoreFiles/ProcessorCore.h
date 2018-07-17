#ifndef PROCESSOR_CORE_H
#define PROCESSOR_CORE_H

#include "Defines.h"
#include <list>

#include "../../Elder/ImageProcessor/ImageProcessor.hpp"
#include "../../Elder/NonCopyable/NonCopyable.hpp"

namespace SHAPER
{
	class CFilterCore;
	class CRotationCore;		
	class CFlipCore;
	class CProcessorCore : public ELDER::CNonCopyable
	{
	public:
		CProcessorCore() {}

		virtual ~CProcessorCore() {}

		virtual std::list<std::shared_ptr<ELDER::CImageProcessor>> Active(SID, std::list<ProcessorTypes> const&) noexcept(false) = 0;

		virtual std::shared_ptr<ELDER::CImageProcessor> const& Active(SID, ProcessorTypes) noexcept(false) = 0;

		virtual std::list<std::shared_ptr<ELDER::CImageProcessor>> Active(SID, std::list<InnerProcessorTypes> const&) noexcept(false) = 0;

		virtual std::shared_ptr<ELDER::CImageProcessor> const& Active(SID, InnerProcessorTypes) noexcept(false) = 0;

		virtual std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>> PopMultiCore(SID, std::list<ProcessorTypes> const&) noexcept(false) = 0;

		virtual void PushMultiCore(SID, std::map<ProcessorTypes, std::shared_ptr<ELDER::CImageProcessor>>&&) noexcept(false) = 0;
	};
}

#endif
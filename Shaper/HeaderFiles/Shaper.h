#ifndef SHAPER_H
#define SHAPER_H

#include "File.h"
#include "Buffer.h"
#include "Process.h"
#include "Processor.h"
#include "Operator.h"
#include "Display.h"

namespace SHAPER
{
	class CShaper
	{
	public:
		CShaper() = default;

		virtual ~CShaper() = default;

		/// Initialize shaper.
		/// param1 : Enable multi CPU core processing. Default is disable.
		/// param2 : Set CPU cores which are used for processing. The default num is max CPU cores.
		/// TODO : json config file
		virtual void Initialize(bool multiCoreEnable = false,
								int multiCoreNum = std::thread::hardware_concurrency()) noexcept(false) = 0;

		/// Enable multi CPU core processing. Default is disable.
		//virtual void EnableMultiCore(bool) noexcept(false) = 0;

		/// Set CPU cores which are used for processing. The default num is max CPU cores.
		/// Is enabled, the core num is set to the max CPU cores automatically.
		//virtual void SetMultiCoreNum(int) noexcept(false) = 0;

		/// Get the Buffer ptr of shaper.
		virtual CBuffer* Buffer() const noexcept(false) = 0;

		/// Get the File ptr of shaper.
		virtual CFile* File() const noexcept(false) = 0;

		/// Get the Process ptr of shaper.
		virtual CProcess* Process() const noexcept(false) = 0;

		/// Get the Processor ptr of shaper.
		virtual CProcessor* Processor() const noexcept(false) = 0;

		/// Get the Operator ptr of shaper.
		virtual COperator* Operator() const noexcept(false) = 0;

		/// Get the Display ptr of shaper.
		virtual CDisplay* Display() const noexcept(false) = 0;
	};

	/// Get the instance of shaper.
	std::shared_ptr<CShaper> ShaperInstance();
}

#endif
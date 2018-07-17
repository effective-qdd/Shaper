#ifndef SHAPER_IMP_H
#define SHAPER_IMP_H

#include "Shaper.h"

namespace SHAPER
{
	class CSuperCore;
	class CSuperBuffer;
	class CSuperFile;
	class CSuperProcess;
	class CSuperProcessor;
	class CSuperOperator;
	class CSuperDisplay;

	class CShaperImp final : public CShaper
	{
	public:
		CShaperImp();

		~CShaperImp();

		void Initialize(bool, int);

		CBuffer* Buffer() const;

		CFile* File() const;

		CProcess* Process() const;

		CProcessor* Processor() const;

		COperator* Operator() const;

		CDisplay* Display() const;

	private:
		CBuffer* m_buffer;
		std::shared_ptr<CSuperBuffer> m_superBuffer;
		CFile* m_file;
		std::shared_ptr<CSuperFile> m_superFile;
		CProcess* m_process;
		std::shared_ptr<CSuperProcess> m_superProcess;
		CProcessor* m_processor;
		std::shared_ptr<CSuperProcessor> m_superProcessor;
		COperator* m_operator;
		std::shared_ptr<CSuperOperator> m_superOperator;
		CDisplay* m_display;
		std::shared_ptr<CSuperDisplay> m_superDisplay;

		bool m_isInitialized;
	};
}

#endif
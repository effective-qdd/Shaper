#include "ShaperImp.h"

#include <windows.h>

#include "../../Elder/Exception/Exception.hpp"

#include "BufferInit.h"
#include "BufferCore.h"
#include "SuperBuffer.h"
#include "SuperFile.h"
#include "ProcessCore.h"
#include "SuperProcess.h"
#include "ProcessorInit.h"
#include "ProcessorCore.h"
#include "SuperProcessor.h"
#include "SuperOperator.h"
#include "DisplayInit.h"
#include "SuperDisplay.h"

namespace SHAPER
{
	typedef std::shared_ptr<CSuperProcessor>(*ProcessorInstanceFunc)(bool, int);
	typedef std::shared_ptr<CSuperBuffer>(*BufferInstanceFunc)(CProcessorInit*);
	typedef std::shared_ptr<CSuperFile>(*FileInstanceFunc)(CBufferInit*, CBufferCore *);
	typedef std::shared_ptr<CSuperProcess>(*ProcessInstanceFunc)(CBufferCore *, CProcessorCore*);
	typedef std::shared_ptr<CSuperOperator>(*OperatorInstanceFunc)(CBufferCore *);
	typedef std::shared_ptr<CSuperDisplay>(*DisplayInstanceFunc)(CBufferCore *);
		
	CShaperImp::CShaperImp()
		: m_buffer(nullptr)
		, m_superBuffer(nullptr)
		, m_file(nullptr)
		, m_superFile(nullptr)
		, m_process(nullptr)
		, m_superProcess(nullptr)
		, m_processor(nullptr)
		, m_superProcessor(nullptr)
		, m_display(nullptr)
		, m_superDisplay(nullptr)
		, m_isInitialized(false)
	{
	}

	CShaperImp::~CShaperImp()
	{
		m_isInitialized = false;
	}

	void CShaperImp::Initialize(bool enable, int num)
	{
		if (m_isInitialized) return;

		// Load Shaper Processor dll before Shaper Buffer dll.
		HINSTANCE shaperProcessorLib = LoadLibrary("ShaperProcessor.dll");
		ENSURE_THROW_MSG(shaperProcessorLib != nullptr, "Load Shaper Processor dll failed.");
		auto processorInstFunc = (ProcessorInstanceFunc)GetProcAddress(shaperProcessorLib, "ProcessorInstance");
		m_superProcessor = processorInstFunc(enable, num);
		ENSURE_THROW_MSG(m_superProcessor != nullptr, "Get Shaper Processor ptr failed.");
		m_processor = dynamic_cast<CProcessor*>(m_superProcessor.get());
		ENSURE_THROW_MSG(m_processor != nullptr, "Get CProcessor ptr failed.");
		auto processorInit = dynamic_cast<CProcessorInit*>(m_superProcessor.get());
		ENSURE_THROW_MSG(processorInit != nullptr, "Get CProcessorInit ptr failed.");
		auto processorCore = dynamic_cast<CProcessorCore*>(m_superProcessor.get());			
		ENSURE_THROW_MSG(processorCore != nullptr, "Get CProcessorCore ptr failed.");
			
		// Load Shaper Buffer dll after Shaper Processor dll and before Shaper File dll.
		HINSTANCE shaperBufferLib = LoadLibrary("ShaperBuffer.dll");
		ENSURE_THROW_MSG(shaperBufferLib != nullptr, "Load Shaper Buffer dll failed.");
		auto bufferInstFunc = (BufferInstanceFunc)GetProcAddress(shaperBufferLib, "BufferInstance");
		m_superBuffer = bufferInstFunc(processorInit);
		ENSURE_THROW_MSG(m_superBuffer != nullptr, "Get Shaper Buffer ptr failed.");
		m_buffer = dynamic_cast<CBuffer*>(m_superBuffer.get());
		ENSURE_THROW_MSG(m_buffer != nullptr, "Get CBuffer ptr failed.");
		auto bufferInit = dynamic_cast<CBufferInit*>(m_superBuffer.get());
		ENSURE_THROW_MSG(bufferInit != nullptr, "Get CBufferInit ptr failed.");
		auto bufferCore = dynamic_cast<CBufferCore*>(m_superBuffer.get());
		ENSURE_THROW_MSG(bufferCore != nullptr, "Get CBufferCore ptr failed.");

		//  Load Shaper File dll after Shaper Buffer dll.
		HINSTANCE shaperFileLib = LoadLibrary("ShaperFile.dll");
		ENSURE_THROW_MSG(shaperFileLib != nullptr, "Load Shaper File dll failed.");
		auto fileInstFunc = (FileInstanceFunc)GetProcAddress(shaperFileLib, "FileInstance");
		m_superFile = fileInstFunc(bufferInit, bufferCore);
		ENSURE_THROW_MSG(m_superFile != nullptr, "Get Shaper File ptr failed.");
		m_file = dynamic_cast<CFile*>(m_superFile.get());
		ENSURE_THROW_MSG(m_file != nullptr, "Get CFile ptr failed.");

		// Load Shaper Process dll after Shaper Buffer dll and Shaper Processor dll.
		HINSTANCE shaperProcessLib = LoadLibrary("ShaperProcess.dll");
		ENSURE_THROW_MSG(shaperProcessLib != nullptr, "Load Shaper Process dll failed.");
		auto processInstFunc = (ProcessInstanceFunc)GetProcAddress(shaperProcessLib, "ProcessInstance");
		m_superProcess = processInstFunc(bufferCore, processorCore);
		ENSURE_THROW_MSG(m_superProcess != nullptr, "Get Shaper Process ptr failed.");
		m_process = dynamic_cast<CProcess*>(m_superProcess.get());
		ENSURE_THROW_MSG(m_process != nullptr, "Get CProcess ptr failed.");
		auto processCore = dynamic_cast<CProcessCore*>(m_superProcess.get());
		ENSURE_THROW_MSG(processCore != nullptr, "Get CProcessCore ptr failed.");

		// Load Shaper Operator dll after Shaper Buffer dll and Shaper Operator dll.
		HINSTANCE shaperOperatorLib = LoadLibrary("ShaperOperator.dll");
		ENSURE_THROW_MSG(shaperOperatorLib != nullptr, "Load Shaper Operator dll failed.");
		auto operatorInstFunc = (OperatorInstanceFunc)GetProcAddress(shaperOperatorLib, "OperatorInstance");
		m_superOperator = operatorInstFunc(bufferCore);
		ENSURE_THROW_MSG(m_superOperator != nullptr, "Get Shaper Operator ptr failed.");
		m_operator = dynamic_cast<COperator*>(m_superOperator.get());
		ENSURE_THROW_MSG(m_operator != nullptr, "Get Operator ptr failed.");

		// Load Shaper Display dll after Shaper Buffer dll.
		HINSTANCE shaperDisplayLib = LoadLibrary("ShaperDisplay.dll");
		ENSURE_THROW_MSG(shaperDisplayLib != nullptr, "Load Shaper Display dll failed.");
		auto displayInstFunc = (DisplayInstanceFunc)GetProcAddress(shaperDisplayLib, "DisplayInstance");
		m_superDisplay = displayInstFunc(bufferCore);
		ENSURE_THROW_MSG(m_superDisplay != nullptr, "Get Shaper Display ptr failed.");
		m_display = dynamic_cast<CDisplay*>(m_superDisplay.get());
		ENSURE_THROW_MSG(m_display != nullptr, "Get CDisplay ptr failed.");

		m_isInitialized = true;
	}

	CBuffer* CShaperImp::Buffer() const
	{
		ENSURE_THROW_MSG(m_buffer != nullptr, "Get CBuffer ptr failed. Call Initialize() first.");
		return m_buffer;
	}

	CFile* CShaperImp::File() const
	{
		ENSURE_THROW_MSG(m_file != nullptr, "Get CFile ptr failed. Call Initialize() first.");
		return m_file;
	}

	CProcess* CShaperImp::Process() const
	{
		ENSURE_THROW_MSG(m_process != nullptr, "Get CProcess ptr failed. Call Initialize() first.");
		return m_process;
	}

	CProcessor* CShaperImp::Processor() const
	{
		ENSURE_THROW_MSG(m_processor != nullptr, "Get CProcessor ptr failed. Call Initialize() first.");
		return m_processor;
	}

	COperator* CShaperImp::Operator() const
	{
		ENSURE_THROW_MSG(m_operator != nullptr, "Get COperator ptr failed. Call Initialize() first.");
		return m_operator;
	}

	CDisplay* CShaperImp::Display() const
	{
		ENSURE_THROW_MSG(m_display != nullptr, "Get CDisplay ptr failed. Call Initialize() first.");
		return m_display;
	}
}

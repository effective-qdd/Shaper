#include "ShaperSeqStream.h"

#include "BufferInit.h"
#include "BufferCore.h"

namespace SHAPER
{
	CSeqStream::CSeqStream(CBufferInit *bufferInit, CBufferCore *bufferCore)
		: CSuperStream()
		, m_bufferInit(bufferInit)
		, m_bufferCore(bufferCore)
		, m_seqReader(nullptr)
	{
		m_seqReaderMap.emplace(ELDER::DataFormats::k8BitGrayScale, std::move(std::make_shared<ELDER::FILE::CSeqReader8u1c>()));
		m_seqReaderMap.emplace(ELDER::DataFormats::k16BitGrayScale, std::move(std::make_shared<ELDER::FILE::CSeqReader16u1c>()));
	}

	CSeqStream::~CSeqStream()
	{
	}

	IID CSeqStream::First() const
	{
		ENSURE_THROW_MSG(m_seqReader != nullptr, "Seq reader is null");
		ASSERT_LOG(m_bufferCore != nullptr, "m_bufferCore is null");
		auto anyImage = m_bufferCore->AcquireRaw(m_sid);
		m_seqReader->Read(anyImage, 0);
			
		return m_bufferCore->MakeID(std::move(anyImage), m_sid);
	}

	IID CSeqStream::Last() const
	{
		return Index(m_fileInfo.imageCounts - 1);
	}

	IID CSeqStream::Index(unsigned long index) const
	{
		ENSURE_THROW_MSG(m_fileInfo.imageCounts > index, "Image index out of range");
		ENSURE_THROW_MSG(m_seqReader != nullptr, "Seq reader is null");
		ASSERT_LOG(m_bufferCore != nullptr, "m_bufferCore is null");
		auto anyImage = m_bufferCore->AcquireRaw(m_sid);
		m_seqReader->Read(anyImage, index);

		return m_bufferCore->MakeID(std::move(anyImage), m_sid);
	}

	void CSeqStream::Open(std::string const& filePath)
	{
		ENSURE_THROW_MSG(!filePath.empty(), "File path is empty");
		auto dataFormat = ELDER::FILE::CSeqReader::PreOpen(filePath);
		ENSURE_THROW_MSG(m_seqReaderMap.find(dataFormat) != m_seqReaderMap.end(), "Can not find such seq reader");
		m_seqReader = m_seqReaderMap[dataFormat];
		m_fileInfo = m_seqReader->Open(filePath);
		m_sid = m_bufferInit->Initialize(m_fileInfo.imageInfo);
	}

	ELDER::FileInfo CSeqStream::Info()
	{
		return m_fileInfo;
	}
		
}
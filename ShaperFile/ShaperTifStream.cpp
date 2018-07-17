#include "ShaperTifStream.h"

#include "BufferInit.h"
#include "BufferCore.h"

namespace SHAPER
{
	CTifStream::CTifStream(CBufferInit* bufferInit, CBufferCore* bufferCore)
		: CSuperStream()
		, m_bufferInit(bufferInit)
		, m_bufferCore(bufferCore)
		, m_tifReader(nullptr)
	{
		m_tifReaderMap.emplace(ELDER::DataFormats::k8BitGrayScale, std::move(std::make_shared<ELDER::FILE::CTifReader8u1c>()));
		m_tifReaderMap.emplace(ELDER::DataFormats::k16BitGrayScale, std::move(std::make_shared<ELDER::FILE::CTifReader16u1c>()));
		m_tifReaderMap.emplace(ELDER::DataFormats::k32BitGrayScale, std::move(std::make_shared<ELDER::FILE::CTiffReader32f1c>()));
	}

	CTifStream::~CTifStream()
	{
	}

	IID CTifStream::First() const
	{
		ENSURE_THROW_MSG(m_tifReader != nullptr, "Tif reader is null");
		ASSERT_LOG(m_bufferCore != nullptr, "m_bufferCore is null");
		auto anyImage = m_bufferCore->AcquireRaw(m_sid);
		m_tifReader->Read(anyImage);

		return m_bufferCore->MakeID(std::move(anyImage), m_sid);
	}

	IID CTifStream::Last() const
	{
		return First();
	}

	IID CTifStream::Index(unsigned long index) const
	{
		ENSURE_THROW_MSG(m_fileInfo.imageCounts > index, "Image index out of range");

		return First();
	}

	void CTifStream::Open(std::string const& filePath)
	{
		ENSURE_THROW_MSG(!filePath.empty(), "File path is empty");
		auto dataFormat = ELDER::FILE::CTifReader::PreOpen(filePath);
		ENSURE_THROW_MSG(m_tifReaderMap.find(dataFormat) != m_tifReaderMap.end(), "Can not find such tif reader");
		m_tifReader = m_tifReaderMap[dataFormat];
		m_fileInfo = m_tifReader->Open(filePath);
		m_sid = m_bufferInit->Initialize(m_fileInfo.imageInfo);
	}

	ELDER::FileInfo CTifStream::Info()
	{
		return m_fileInfo;
	}
}

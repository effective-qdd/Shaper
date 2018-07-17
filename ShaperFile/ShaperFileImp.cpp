#include "ShaperFileImp.h"

#include <algorithm>

#include "../../Elder/Singleton/Singleton.hpp"
#include "../../Elder/File/File.hpp"

#include "ShaperTifStream.h"
#include "ShaperSeqStream.h"
#include "BufferCore.h"

namespace SHAPER
{
	CFileImp::CFileImp(CBufferInit* bufferInit, CBufferCore* bufferCore)
		: CSuperFile()
		, m_bufferInit(bufferInit)
		, m_bufferCore(bufferCore)
	{
		ASSERT_LOG(m_bufferInit != nullptr, "m_bufferInit is null");
		ASSERT_LOG(m_bufferCore != nullptr, "m_bufferCore is null");
		m_supportedFileTypeList.push_back(FileTypes::kTif);
		m_supportedFileTypeList.push_back(FileTypes::kSeq);

		m_fileTypesMap.emplace("tif", FileTypes::kTif);
		m_fileTypesMap.emplace("seq", FileTypes::kSeq);
	}


	CFileImp::~CFileImp()
	{
		m_superStreamMap.swap(std::map<FID, std::shared_ptr<CSuperStream>>());
		m_fileTypesMap.swap(std::map<std::string, FileTypes>());
	}

	FID CFileImp::Open(std::string const& filePath)
	{
		ENSURE_THROW_MSG(!filePath.empty(), "File path is empty");
		auto fileType = FileType(filePath);
		ENSURE_THROW_MSG(std::find(m_supportedFileTypeList.begin(), m_supportedFileTypeList.end(), fileType) != m_supportedFileTypeList.end(), "Can not find such file type");
		auto superStream = SuperStream(fileType);
		ASSERT_LOG(superStream != nullptr, "superStream is null");
		superStream->Open(filePath);
		FID fid = static_cast<FID>(m_superStreamMap.size());
		m_superStreamMap.emplace(fid, superStream);

		return fid;
	}

	FID CFileImp::Open(std::string const& filePath, FileTypes fileType)
	{
		ENSURE_THROW_MSG(!filePath.empty(), "File path is empty");
		ENSURE_THROW_MSG(std::find(m_supportedFileTypeList.begin(), m_supportedFileTypeList.end(), fileType) != m_supportedFileTypeList.end(), "Can not find such file type");
		auto superStream = SuperStream(fileType);
		ASSERT_LOG(superStream != nullptr, "superStream is null");
		superStream->Open(filePath);
		FID fid = static_cast<FID>(m_superStreamMap.size());
		m_superStreamMap.emplace(fid, superStream);

		return fid;
	}

	ELDER::FileInfo CFileImp::Info(FID fid)
	{
		ENSURE_THROW_MSG(m_superStreamMap.find(fid) != m_superStreamMap.end(), "Can not find the fid");

		return m_superStreamMap[fid]->Info();
	}

	CStream* CFileImp::Stream(FID fid)
	{
		ENSURE_THROW_MSG(m_superStreamMap.find(fid) != m_superStreamMap.end(), "Can not find the fid");

		return dynamic_cast<CStream *>(m_superStreamMap[fid].get());
	}

	void CFileImp::Close(FID fid)
	{
		ENSURE_THROW_MSG(m_superStreamMap.find(fid) != m_superStreamMap.end(), "Can not find the fid");
		m_superStreamMap.erase(fid);
	}

	void CFileImp::Save(IID iid, std::string const& filePath, FileTypes fileType)
	{
		ENSURE_THROW_MSG(!filePath.empty(), "File path is empty");
		ENSURE_THROW_MSG(std::find(m_supportedFileTypeList.begin(), m_supportedFileTypeList.end(), fileType) != m_supportedFileTypeList.end(), "Can not find such file type");
		ASSERT_LOG(m_bufferCore != nullptr, "m_bufferCore is null");
		auto buf = m_bufferCore->Resume(iid);

		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			if (fileType == FileTypes::kTif)
				ELDER::FILE::CFile::Write<ELDER::FILE::CTifWriter8u1c>(filePath, buf);
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			if (fileType == FileTypes::kTif)
				ELDER::FILE::CFile::Write<ELDER::FILE::CTifWriter16u1c>(filePath, buf);
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			if (fileType == FileTypes::kTif)
				ELDER::FILE::CFile::Write<ELDER::FILE::CTifWriter32f1c>(filePath, buf);
		}
		else
		{
			THROW_MSG("Unsupported buf type");
		}
	}

	std::shared_ptr<CSuperStream> CFileImp::SuperStream(FileTypes fileType)
	{
		ENSURE_THROW_MSG(std::find(m_supportedFileTypeList.begin(), m_supportedFileTypeList.end(), fileType) != m_supportedFileTypeList.end(), "Can not find such file type");
		ASSERT_LOG(m_bufferInit != nullptr, "m_bufferInit is null");
		ASSERT_LOG(m_bufferCore != nullptr, "m_bufferCore is null");
		if (fileType == FileTypes::kTif)
		{
			return std::make_shared<CTifStream>(m_bufferInit, m_bufferCore);
		}
		else if (fileType == FileTypes::kSeq)
		{
			return std::make_shared<CSeqStream>(m_bufferInit, m_bufferCore);
		}
		return nullptr;
	}

	FileTypes CFileImp::FileType(std::string const& filePath)
	{
		ENSURE_THROW_MSG(!filePath.empty(), "File path is empty");
		const auto fileExt = filePath.substr(filePath.find_last_of('.') + 1);

		return m_fileTypesMap.at(fileExt);
	}
}
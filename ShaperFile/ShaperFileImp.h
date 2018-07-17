#ifndef SHAPER_CORE_IMP_H
#define SHAPER_CORE_IMP_H

#include <map>
#include <list>

#include "../../Elder/Enumeration/Enums.h"
#include "../../Elder/File/Picture.hpp"

#include "SuperFile.h"

namespace SHAPER
{
	class CBufferInit;
	class CBufferCore;
	class CSuperStream;
	class CFileImp final : public CSuperFile
	{
	public:
		CFileImp(CBufferInit*, CBufferCore*);	

		~CFileImp();

		FID Open(std::string const&);

		FID Open(std::string const&, FileTypes);

		ELDER::FileInfo Info(FID);

		CStream* Stream(FID);

		void Close(FID);

		void Save(IID, std::string const&, FileTypes);

	private:
		std::shared_ptr<CSuperStream> SuperStream(FileTypes);

		FileTypes FileType(std::string const&);

	private:
		CBufferInit* m_bufferInit;
		CBufferCore* m_bufferCore;
		std::list<FileTypes> m_supportedFileTypeList;
		std::map<FID, std::shared_ptr<CSuperStream>> m_superStreamMap;
		std::map<std::string, FileTypes> m_fileTypesMap;
	};
}

#endif
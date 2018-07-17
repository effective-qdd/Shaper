#ifndef SHAPER_TIF_STREAM_H
#define SHAPER_TIF_STREAM_H

#include <unordered_map>

#include "../../Elder/File/Tif.hpp"

#include "SuperStream.h"

namespace SHAPER
{
	class CBufferInit;
	class CBufferCore;
	class CTifStream final : public CSuperStream
	{
	public:
		CTifStream(CBufferInit*, CBufferCore*);

		~CTifStream();

		IID First() const;

		IID Last() const;

		IID Index(unsigned long) const;

		void Open(std::string const& filePath);

		ELDER::FileInfo Info();

	private:
		CBufferInit* m_bufferInit;
		CBufferCore* m_bufferCore;
		std::shared_ptr<ELDER::FILE::CTifReader> m_tifReader;
		std::unordered_map<ELDER::DataFormats, std::shared_ptr<ELDER::FILE::CTifReader>> m_tifReaderMap;
		SID m_sid;
		ELDER::FileInfo m_fileInfo;
	};
}

#endif
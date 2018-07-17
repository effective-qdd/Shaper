#ifndef SHAPER_SEQ_STREAM_H
#define SHAPER_SEQ_STREAM_H

#include <unordered_map>

#include "../../Elder/File/Seq.hpp"

#include "SuperStream.h"

namespace SHAPER
{
	class CBufferInit;
	class CBufferCore;
	class CSeqStream final : public CSuperStream
	{
	public:
		CSeqStream(CBufferInit*, CBufferCore*);

		~CSeqStream();

		IID First() const;

		IID Last() const;

		IID Index(unsigned long) const;

		void Open(const std::string &filePath);

		ELDER::FileInfo Info();

	private:
		CBufferInit* m_bufferInit;
		CBufferCore* m_bufferCore;
		std::shared_ptr<ELDER::FILE::CSeqReader> m_seqReader;
		std::unordered_map<ELDER::DataFormats, std::shared_ptr<ELDER::FILE::CSeqReader>> m_seqReaderMap;
		SID m_sid;
		ELDER::FileInfo m_fileInfo;
	};
}

#endif
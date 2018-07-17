#ifndef SHAPER_FILE_STREAM_H
#define SHAPER_FILE_STREAM_H

#include "Defines.h"

namespace BJI
{
	namespace SHAPER
	{
		class CFileStream
		{
		public:
			CFileStream() = default;

			virtual ~CFileStream() = default;

			virtual void Open(const std::string &filePath) = 0;

			virtual FID FirstFrame() = 0;
		};
	}
}

#endif
#ifndef SHAPER_FILE_H
#define SHAPER_FILE_H

#include <memory>

namespace SHAPER
{
	class CSuperFile;
	class CBufferInit;
	class CBufferCore;
	std::shared_ptr<CSuperFile> FileInstance(CBufferInit*, CBufferCore*);
}

#endif


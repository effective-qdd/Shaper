#ifndef FILE_H
#define FILE_H

#include "Defines.h"
#include "Stream.h"
#include <string>

namespace SHAPER
{
	class CFile
	{
	public:
		CFile() {}

		virtual ~CFile() {}

		/// Open file with the specify file path. 
		/// Check the file type with .ext
		/// Support: tif
		virtual FID Open(std::string const&) noexcept(false) = 0;

		/// Open file with  the specify file path and file type.
		virtual FID Open(std::string const&, FileTypes) noexcept(false) = 0;

		/// Get the file info.
		virtual ELDER::FileInfo Info(FID) noexcept(false) = 0;

		/// Get the stream of file.
		virtual CStream* Stream(FID) noexcept(false) = 0;

		/// Close file.
		virtual void Close(FID) noexcept(false) = 0;

		/// Save file.
		virtual void Save(IID, std::string const&, FileTypes) noexcept(false) = 0;
	};
}

#endif
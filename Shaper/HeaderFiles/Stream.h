#ifndef STREAM_H
#define STREAM_H

#include "Defines.h"

namespace SHAPER
{		
	class CStream
	{
	public:
		CStream() = default;

		virtual ~CStream() = default;

		/// Get image info
		//virtual ImageInfo Info() noexcept(false) = 0;

		/// Get first image, index = 0;
		virtual IID First() const noexcept(false) = 0;

		/// Get next image, 
		//virtual IID Next() noexcept(false) = 0;

		/// Get previous image,
		//virtual IID Prev() noexcept(false) = 0;

		/// Get last image, 
		virtual IID Last() const noexcept(false) = 0;

		/// Get index image, index = 0;
		virtual IID Index(unsigned long) const noexcept(false) = 0;

	};
}	

#endif
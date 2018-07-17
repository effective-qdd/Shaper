#ifndef BUFFER_H
#define BUFFER_H

#include "Defines.h"

namespace SHAPER
{
	class CBuffer
	{
	public:
		CBuffer() {}

		virtual ~CBuffer() {}

		/// Acquire IID with specify image info
		virtual IID Acquire(ELDER::ImageInfo const&) noexcept(false) = 0;

		/// Acquire IID with specify image data and info. Optimized by IPP
		virtual IID Acquire(void const*, ELDER::ImageInfo const&) noexcept(false) = 0;

		/// Acquire IID with specify image data and info. Optimized by TBB
		virtual IID AcquireByTBB(void const*, ELDER::ImageInfo const&) noexcept(false) = 0;

		/// Acquire float32 IID with specify image info.
		virtual IID Acquire32f(void const*, ELDER::ImageInfo const&) noexcept(false) = 0;

		/// Acquire float32 IID with specify image info. Optimized by OpenMP
		virtual IID Acquire32fByOMP(void const*, ELDER::ImageInfo const&) noexcept(false) = 0;

		/// Get 8bit gray scale data.
		virtual unsigned char const* Revert8u(IID) noexcept(false) = 0;

		/// Get image info of iid.
		virtual ELDER::ImageInfo Info(IID) noexcept(false) = 0;

		/// Release an iid.
		virtual void Release(IID) noexcept(false) = 0;

		/// Release iids.
		virtual void Release(std::list<IID> const&) noexcept(false) = 0;

		/// Release all iids
		virtual void Clear() noexcept(false) = 0;
	};
}

#endif

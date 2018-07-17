#ifndef SHAPER_BUFFER_IMP_H
#define SHAPER_BUFFER_IMP_H

#include <map>
#include <mutex>

#include "../../Elder/Image/ImagePool.hpp"

#include "SuperBuffer.h"

namespace SHAPER
{
	class CProcessorInit;
	class CBufferImp final : public CSuperBuffer
	{
	public:
		CBufferImp(CProcessorInit*);

		~CBufferImp();

		//////////////////////////////////////////////////////////////////////////
		IID Acquire(ELDER::ImageInfo const&);

		IID Acquire(void const*, ELDER::ImageInfo const&);

		IID AcquireByTBB(void const*, ELDER::ImageInfo const&);

		IID Acquire32f(void const*, ELDER::ImageInfo const&);

		IID Acquire32fByOMP(void const*, ELDER::ImageInfo const&);

		unsigned char const* Revert8u(IID);

		ELDER::ImageInfo Info(IID);

		void Release(IID);

		void Release(std::list<IID> const&);

		void Clear();

		//////////////////////////////////////////////////////////////////////////
		SID Initialize(const ELDER::ImageInfo &);

		bool IsInitialized() noexcept(true);

		void UnInitialize();

		//const ELDER::ImageInfo &Info();

		std::any AcquireRaw(SID);

		std::any AcquireProcess(SID);

		SID BufferID(ELDER::ImageInfo const&);

		IID MakeID(std::any&&, SID);

		std::any const& Resume(IID);

	private:
		IID CreateIID(SID);

	private:
		CProcessorInit * m_processorInit;
		std::map<ELDER::ImageInfo, SID> m_bidMap;
		std::map<SID, ELDER::ImageInfo> m_imageInfoMap;
		std::map<SID, std::shared_ptr<ELDER::IImagePool>> m_rawImagePoolMap;
		std::map<SID, std::shared_ptr<ELDER::IImagePool>> m_processImagePoolMap;
		std::map<SID, unsigned char*> m_revertBufMap;
		std::map<SID, std::shared_ptr<ELDER::CImage8u1cIPPI>> m_revertTmpBuf8uMap;
		std::map<SID, std::shared_ptr<ELDER::CImage32f1cIPPI>> m_revertTmpBuf32fMap;
		std::map<IID, std::any> m_acquiredBufferMap;
		std::map<SID, SID> m_iidIncMap;
		bool m_isInitialized;
		std::mutex m_mutex;
		SID m_sid;
	};
}

#endif

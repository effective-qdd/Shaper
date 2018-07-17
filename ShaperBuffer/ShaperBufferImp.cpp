#include "ShaperBufferImp.h"

#include <tbb/tbb.h>

#include "../../Elder/ImageOperator/ImageCopy.hpp"
#include "../../Elder/ImageOperator/ImageConvert.hpp"
#include "../../Elder/ImageOperator/ImageMax.hpp"
#include "../../Elder/ImageOperator/ImageMin.hpp"
#include "../../Elder/ImageOperator/ImageSubConstant.hpp"
#include "../../Elder/ImageOperator/ImageMulConstant.hpp"
#include "../../Elder/ImageOperator/ImageConvert.hpp"
#include "../../Elder/ImageOperator/ImageSetConstant.hpp"

#include "ProcessorInit.h"


namespace SHAPER
{
	const int RawBufferSize = 10;
	const int ProcessBufferSize = 10;

	CBufferImp::CBufferImp(CProcessorInit* proceeeosInit)
		: CSuperBuffer() 
		, m_processorInit(proceeeosInit)
		, m_isInitialized(false)
		, m_sid(0)
	{
	}

	CBufferImp::~CBufferImp()
	{
		m_acquiredBufferMap.swap(std::map<IID, std::any>());
		m_rawImagePoolMap.swap(std::map<SID, std::shared_ptr<ELDER::IImagePool>>());
		for (auto &buf : m_revertBufMap)
		{
			if (buf.second != nullptr)
			{
				ippsFree(buf.second);
				buf.second = nullptr;
			}
		}
		m_revertBufMap.swap(std::map<SID, unsigned char*>());
		m_revertTmpBuf8uMap.swap(std::map<SID, std::shared_ptr<ELDER::CImage8u1cIPPI>>());
		m_revertTmpBuf32fMap.swap(std::map<SID, std::shared_ptr<ELDER::CImage32f1cIPPI>>());
		m_processImagePoolMap.swap(std::map<SID, std::shared_ptr<ELDER::IImagePool>>());
	}

	IID CBufferImp::Acquire(const ELDER::ImageInfo &imageInfo)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		auto bid = Initialize(imageInfo);
		auto rawImagePool = m_rawImagePoolMap[bid];
		auto iid = CreateIID(bid);
		m_acquiredBufferMap.emplace(iid, std::move(rawImagePool->Acquire()));

		return iid;
	}

// 	IID CBufferImp::Acquire(void const* buf, ELDER::ImageInfo const& imageInfo)
// 	{
// 		std::lock_guard<std::mutex> lock(m_mutex);
// 		ENSURE_THROW_MSG(buf != nullptr, "Ptr is null");
// 		ENSURE_THROW_MSG(imageInfo.bitDepth == 8 ||
// 			imageInfo.bitDepth == 16 ||
// 			imageInfo.bitDepth == 32,
// 			"Unsupported bit depth");
// 
// 		auto bid = Initialize(imageInfo);
// 		auto rawImagePool = m_rawImagePoolMap[bid];
// 		std::any anyImage;
// 		if (imageInfo.bitDepth == 8)
// 		{
// 			anyImage = rawImagePool->Acquire();
// 			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(anyImage);
// 			ASSERT_LOG(rawImage != nullptr, "Raw image ptr is null");
// 			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
// 			auto bufSize = rawImage->TotalBytes();
// 			memcpy_s(rawImage->Data(), bufSize, buf, bufSize);
// 		}
// 		else if (imageInfo.bitDepth == 16)
// 		{
// 			anyImage = rawImagePool->Acquire();
// 			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(anyImage);
// 			ASSERT_LOG(rawImage != nullptr, "Ptr is null");
// 			auto bufSize = rawImage->TotalBytes();
// 			memcpy_s(rawImage->Data(), bufSize, buf, bufSize);
// 		}
// 		else if (imageInfo.bitDepth == 32)
// 		{
// 			anyImage = rawImagePool->Acquire();
// 			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
// 			ASSERT_LOG(rawImage != nullptr, "Ptr is null");
// 			auto bufSize = rawImage->TotalBytes();
// 			memcpy_s(rawImage->Data(), bufSize, buf, bufSize);
// 		}
// 
// 		auto iid = CreateIID(bid);
// 		m_acquiredBufferMap.emplace(iid, std::move(anyImage));
// 
// 		return iid;
// 	}

	IID CBufferImp::Acquire(void const* buf, ELDER::ImageInfo const& imageInfo)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(buf != nullptr, "Ptr is null");
		ENSURE_THROW_MSG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		auto bid = Initialize(imageInfo);
		auto rawImagePool = m_rawImagePoolMap[bid];
		std::any anyImage;
		if (imageInfo.bitDepth == 8)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Raw image ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto bufStep = static_cast<int>(imageInfo.size.width * sizeof(Ipp8u));
			auto rawStep = rawImage->WidthBytes();
			auto status = ippiCopy_8u_C1R
			(
				reinterpret_cast<Ipp8u*>(const_cast<void*>(buf)),
				bufStep,
				rawImage->Data(),
				rawStep,
				{ rawImage->Width(), rawImage->Height() }
			);
		}
		else if (imageInfo.bitDepth == 16)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Raw image ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto bufStep = static_cast<int>(imageInfo.size.width * sizeof(Ipp16u));
			auto rawStep = rawImage->WidthBytes();
			auto status = ippiCopy_16u_C1R
			(
				reinterpret_cast<Ipp16u*>(const_cast<void*>(buf)),
				bufStep,
				rawImage->Data(),
				rawStep,
				{ rawImage->Width(), rawImage->Height() }
			);
		}
		else if (imageInfo.bitDepth == 32)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Raw image ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto bufStep = static_cast<int>(imageInfo.size.width * sizeof(Ipp32f));
			auto rawStep = rawImage->WidthBytes();
			auto status = ippiCopy_32f_C1R
			(
				reinterpret_cast<Ipp32f*>(const_cast<void*>(buf)),
				bufStep,
				rawImage->Data(),
				rawStep,
				{ rawImage->Width(), rawImage->Height() }
			);
		}

		auto iid = CreateIID(bid);
		m_acquiredBufferMap.emplace(iid, std::move(anyImage));

		return iid;
	}

	IID CBufferImp::AcquireByTBB(void const* buf, ELDER::ImageInfo const& imageInfo)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(buf != nullptr, "Buf is null");
		ENSURE_THROW_MSG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		auto bid = Initialize(imageInfo);
		auto rawImagePool = m_rawImagePoolMap[bid];
		size_t grainSize = imageInfo.size.width / std::thread::hardware_concurrency();
		std::any anyImage;
		if (imageInfo.bitDepth == 8)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Raw image ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto inputBuf = reinterpret_cast<unsigned char*>(const_cast<void*>(buf));
			auto srcBufStep = static_cast<int>(imageInfo.size.width * sizeof(Ipp8u));
			auto dstBufStep = rawImage->WidthBytes();
			tbb::parallel_for
			(
				tbb::blocked_range<size_t>(0, imageInfo.size.height, grainSize),
				[=](tbb::blocked_range<size_t> const& r)
				{
					auto srcOffset = r.begin() * imageInfo.size.width;
					auto dstOffset = r.begin() * (rawImage->WidthBytes() / sizeof(Ipp8u));
					auto ySize = static_cast<int>(r.end() - r.begin());
					auto srcBuf = inputBuf + srcOffset;
					auto dstBuf = rawImage->Data() + dstOffset;
					auto status = ippiCopy_8u_C1R
					(
						srcBuf,
						srcBufStep,
						dstBuf,
						dstBufStep,
						{ imageInfo.size.width , ySize }
					);					
				}
			);
		}
		else if (imageInfo.bitDepth == 16)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Raw image ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto inputBuf = reinterpret_cast<unsigned short*>(const_cast<void*>(buf));
			auto srcBufStep = static_cast<int>(imageInfo.size.width * sizeof(Ipp16u));
			auto dstBufStep = rawImage->WidthBytes();
			tbb::parallel_for
			(
				tbb::blocked_range<size_t>(0, imageInfo.size.height, grainSize),
				[=](tbb::blocked_range<size_t> const& r)
				{
					auto srcOffset = r.begin() * imageInfo.size.width;
					auto dstOffset = r.begin() * (rawImage->WidthBytes() / sizeof(Ipp16u));
					auto ySize = (int)(r.end() - r.begin());
					auto srcBuf = inputBuf + srcOffset;
					auto dstBuf = rawImage->Data() + dstOffset;
					//memcpy_s(dstBuf, rawImage->WidthBytes() * ySize, srcBuf, rawImage->WidthBytes() * ySize);
					auto status = ippiCopy_16u_C1R
					(
						srcBuf,
						srcBufStep,
						dstBuf,
						dstBufStep,
						{ imageInfo.size.width , ySize }
					);
				}
			);
		}
		else if (imageInfo.bitDepth == 32)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Raw image ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto inputBuf = reinterpret_cast<float*>(const_cast<void*>(buf));
			auto srcBufStep = static_cast<int>(imageInfo.size.width * sizeof(Ipp32f));
			auto dstBufStep = rawImage->WidthBytes();
			tbb::parallel_for
			(
				tbb::blocked_range<size_t>(0, imageInfo.size.height, grainSize),
				[=](tbb::blocked_range<size_t> const& r)
				{
					auto srcOffset = r.begin() * imageInfo.size.width;
					auto dstOffset = r.begin() * (rawImage->WidthBytes() / sizeof(Ipp32f));
					auto ySize = (int)(r.end() - r.begin());
					auto srcBuf = inputBuf + srcOffset;
					auto dstBuf = rawImage->Data() + dstOffset;
					auto status = ippiCopy_32f_C1R
					(
						srcBuf,
						srcBufStep,
						dstBuf,
						dstBufStep,
						{ imageInfo.size.width , ySize }
					);
				}
			);
		}

		auto iid = CreateIID(bid);
		m_acquiredBufferMap.emplace(iid, std::move(anyImage));

		return iid;
	}

	IID CBufferImp::Acquire32f(void const* buf, ELDER::ImageInfo const& imageInfo)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(buf != nullptr, "Ptr is null");
		ENSURE_THROW_MSG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		auto imageInfo32f = imageInfo;
		imageInfo32f.bitDepth = 32;
		auto bid = Initialize(imageInfo32f);
		auto rawImagePool = m_rawImagePoolMap[bid];
		std::any anyImage;
		if (imageInfo.bitDepth == 8)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Raw image ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto bufStep = static_cast<int>(imageInfo.size.width * sizeof(Ipp8u));
			auto rawStep = rawImage->WidthBytes();
			IppStatus status = ippiConvert_8u32f_C1R
			(
				reinterpret_cast<Ipp8u*>(const_cast<void*>(buf)),
				bufStep,
				rawImage->Data(),
				rawStep,
				{ rawImage->Width(), rawImage->Height() }
			);
			ENSURE_THROW_MSG(status == ippStsNoErr, "ippiConvert_8u32f_C1R failed!");
		}
		else if (imageInfo.bitDepth == 16)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto bufStep = static_cast<int>(imageInfo.size.width * sizeof(Ipp16u));
			auto rawStep = rawImage->WidthBytes();
			LOG_INFO("Before ippiConvert_16u32f_C1R");
			IppStatus status = ippiConvert_16u32f_C1R
			(
				reinterpret_cast<Ipp16u*>(const_cast<void*>(buf)),
				bufStep,
				rawImage->Data(),
				rawStep,
				{ rawImage->Width(), rawImage->Height() }
			);
			LOG_INFO("After ippiConvert_16u32f_C1R");
			ENSURE_THROW_MSG(status == ippStsNoErr, "ippiConvert_16u32f_C1R failed!");
		}
		else if (imageInfo.bitDepth == 32)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Raw image ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto bufStep = static_cast<int>(imageInfo.size.width * sizeof(Ipp32f));
			auto rawStep = rawImage->WidthBytes();
			auto status = ippiCopy_32f_C1R
			(
				reinterpret_cast<Ipp32f*>(const_cast<void*>(buf)),
				bufStep,
				rawImage->Data(),
				rawStep,
				{ rawImage->Width(), rawImage->Height() }
			);
		}

		auto iid = CreateIID(bid);
		m_acquiredBufferMap.emplace(iid, std::move(anyImage));

		return iid;
	}

	IID CBufferImp::Acquire32fByOMP(void const* buf, ELDER::ImageInfo const& imageInfo)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(buf != nullptr, "Ptr is null");
		ENSURE_THROW_MSG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");

		auto imageInfo32f = imageInfo;
		imageInfo32f.bitDepth = 32;
		auto bid = Initialize(imageInfo32f);
		auto rawImagePool = m_rawImagePoolMap[bid];
		std::any anyImage;
		if (imageInfo.bitDepth == 8)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Raw image ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto bufPtr = reinterpret_cast<Ipp8u*>(const_cast<void*>(buf));
			auto rawPtr = rawImage->Data();
			auto rawStep = static_cast<int>(rawImage->WidthBytes() / sizeof(float));
#pragma omp parallel for
			for (int h = 0; h < imageInfo32f.size.height; ++h)
			{
				for (int w = 0; w < imageInfo32f.size.width; ++w)
				{
					rawPtr[0 + rawStep * h] = static_cast<float>(bufPtr[0 + imageInfo32f.size.width * h]);
				}
			}
		}
		else if (imageInfo.bitDepth == 16)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto bufPtr = reinterpret_cast<Ipp16u*>(const_cast<void*>(buf));
			auto rawPtr = rawImage->Data();
			auto rawStep = static_cast<int>(rawImage->WidthBytes() / sizeof(float));
#pragma omp parallel for				
			for (int h = 0; h < imageInfo32f.size.height; ++h)
			{
				for (int w = 0; w < imageInfo32f.size.width; ++w)
				{
					rawPtr[w + rawStep * h] = static_cast<float>(bufPtr[w + imageInfo32f.size.width * h]);
				}
			}
		}
		else if (imageInfo.bitDepth == 32)
		{
			anyImage = rawImagePool->Acquire();
			auto rawImage = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
			ASSERT_LOG(rawImage != nullptr, "Ptr is null");
			ASSERT_LOG(rawImage->Width() == imageInfo.size.width, "Width mismatch");
			ASSERT_LOG(rawImage->Height() == imageInfo.size.height, "Height mismatch");
			auto bufPtr = reinterpret_cast<Ipp32f*>(const_cast<void*>(buf));
			auto rawPtr = rawImage->Data();
			auto rawStep = static_cast<int>(rawImage->WidthBytes() / sizeof(float));
#pragma omp parallel for	
			for (int h = 0; h < imageInfo32f.size.height; ++h)
			{
				for (int w = 0; w < imageInfo32f.size.width; ++w)
				{
					rawPtr[w + rawStep * h] = bufPtr[w + imageInfo32f.size.width * h];
				}
			}
		}

		auto iid = CreateIID(bid);
		m_acquiredBufferMap.emplace(iid, std::move(anyImage));

		return iid;
	}

	unsigned char const* CBufferImp::Revert8u(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(m_acquiredBufferMap.find(iid) != m_acquiredBufferMap.end(), "Can not find such iid");
		auto anyImage = m_acquiredBufferMap.at(iid);

		std::shared_ptr<ELDER::CImage8u1cIPPI> image8u1c = nullptr;
		if (anyImage.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			image8u1c = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(anyImage);
		}
		else 
		{
			SID bid = iid >> 16;
			ENSURE_THROW_MSG(m_revertTmpBuf8uMap.find(bid) != m_revertTmpBuf8uMap.end(), "Can not find such bid");
			ENSURE_THROW_MSG(m_revertTmpBuf32fMap.find(bid) != m_revertTmpBuf32fMap.end(), "Can not find such bid");
			image8u1c = m_revertTmpBuf8uMap[bid];
			auto tmpImage32f1c = m_revertTmpBuf32fMap[bid];
			auto value = 0.0f;
			auto max = 0.0f;
			auto min = 0.0f;
			
			if (anyImage.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
			{
				auto image16u1c = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(anyImage);
				
				ELDER::OPERATOR::CImageConvert<ELDER::OPERATOR::CConvert16u1cTo32f1c>::Convert(image16u1c, tmpImage32f1c);
				max = ELDER::OPERATOR::CImageMax<ELDER::OPERATOR::CMax32f1c>::Max(tmpImage32f1c);
				min = ELDER::OPERATOR::CImageMin<ELDER::OPERATOR::CMin32f1c>::Min(tmpImage32f1c);
				value = max - min;
				ELDER::OPERATOR::CImageSubConstant<ELDER::OPERATOR::CSubConstant32f1c>::SubConstant(tmpImage32f1c, min);
			}
			else if (anyImage.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
			{
				auto image32f1c = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
				max = ELDER::OPERATOR::CImageMax<ELDER::OPERATOR::CMax32f1c>::Max(image32f1c);
				min = ELDER::OPERATOR::CImageMin<ELDER::OPERATOR::CMin32f1c>::Min(image32f1c);
				value = max - min;
				ELDER::OPERATOR::CImageSubConstant<ELDER::OPERATOR::CSubConstant32f1c>::SubConstant(tmpImage32f1c, image32f1c, min);
			}
			if (abs(value) > 0.00001)
			{
				auto scale = 255.0f / value;
				ELDER::OPERATOR::CImageMulConstant<ELDER::OPERATOR::CMulConstant32f1c>::MulConstant(tmpImage32f1c, scale);
				ELDER::OPERATOR::CImageConvert<ELDER::OPERATOR::CConvert32f1cTo8u1c>::Convert(tmpImage32f1c, image8u1c);
			}
			else
			{
				ELDER::OPERATOR::CImageSetConstant<ELDER::OPERATOR::CSetConstant8u1c>::SetConstant(image8u1c, static_cast<ELDER::IDT8U>(max) % 255);
			}
		}

		unsigned char* data = nullptr;
		if (image8u1c->Width() == image8u1c->WidthBytes())
		{
			data = image8u1c->Data();
		}
		else
		{
			SID bid = iid >> 16;
			ENSURE_THROW_MSG(m_revertBufMap.find(bid) != m_revertBufMap.end(), "Can not find such bid");
			auto revertData = m_revertBufMap[bid];
			if (revertData != nullptr)
			{
				auto status = ippiCopy_8u_C1R
				(
					image8u1c->Data(),
					image8u1c->WidthBytes(),
					revertData,
					image8u1c->Width(),
					{ image8u1c->Width(), image8u1c->Height() }
				);
				ENSURE_THROW_MSG(status == ippStsNoErr, "ippiCopy_8u_C1R failed!");
				data = revertData;
			}
		}

		return data;
	}

	ELDER::ImageInfo CBufferImp::Info(IID iid)
	{
		SID bid = iid >> 16;
		ENSURE_THROW_MSG(m_imageInfoMap.find(bid) != m_imageInfoMap.end(), "Can not find such bid");
		
		return m_imageInfoMap[bid];
	}

// 	const unsigned char *CBufferImp::Revert(IID iid)
// 	{
// 		std::lock_guard<std::mutex> lock(m_mutex);
// 		ENSURE_THROW_MSG(m_acquiredBufferMap.find(iid) != m_acquiredBufferMap.end(), "Can not find such iid");
// 		auto anyImage = m_acquiredBufferMap.at(iid);
// 
// 		unsigned char *data = nullptr;
// 		if (anyImage.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
// 		{
// 			auto image = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(anyImage);
// 			if (image->Width() == image->WidthBytes())
// 			{
// 				data = image->Data();
// 			}
// 			else
// 			{
// 				auto bid = iid >> 16;
// 				ENSURE_THROW_MSG(m_revertBufMap.find(bid) != m_revertBufMap.end(), "Can not find such bid");
// 				auto revertData = m_revertBufMap[bid];
// 				if (revertData != nullptr)
// 				{
// 					auto status = ippiCopy_8u_C1R
// 					(
// 						image->Data(),
// 						image->WidthBytes(),
// 						revertData,
// 						image->Width(),
// 						{ image->Width(), image->Height() }
// 					);
// 					ENSURE_THROW_MSG(status == ippStsNoErr, "ippiCopy_8u_C1R failed!");
// 					data = revertData;
// 				}
// 			} 
// 		}
// 		else if (anyImage.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
// 		{
// 			auto image = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(anyImage);
// 			if (image->Width() * sizeof(unsigned short) == image->WidthBytes())
// 			{
// 				data = (unsigned char*)(image->Data());
// 			}
// 			else
// 			{
// 				auto bid = iid >> 16;
// 				ENSURE_THROW_MSG(m_revertBufMap.find(bid) != m_revertBufMap.end(), "Can not find such bid");
// 				auto revertData = m_revertBufMap[bid];
// 				if (revertData != nullptr)
// 				{
// 					auto status = ippiCopy_8u_C1R
// 					(
// 						(unsigned char*)(image->Data()),
// 						image->WidthBytes(),
// 						revertData,
// 						image->Width(),
// 						{ image->Width(), image->Height() }
// 					);
// 					ENSURE_THROW_MSG(status == ippStsNoErr, "ippiCopy_8u_C1R failed!");
// 					data = revertData;
// 				}
// 			}
// 		}
// 		else if (anyImage.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
// 		{
// 			auto image = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(anyImage);
// 			if (image->Width() * sizeof(float) == image->WidthBytes())
// 			{
// 				data = (unsigned char*)(image->Data());
// 			}
// 			else
// 			{
// 				auto bid = iid >> 16;
// 				ENSURE_THROW_MSG(m_revertBufMap.find(bid) != m_revertBufMap.end(), "Can not find such bid");
// 				auto revertData = m_revertBufMap[bid];
// 				if (revertData != nullptr)
// 				{
// 					auto status = ippiCopy_8u_C1R
// 					(
// 						(unsigned char*)(image->Data()),
// 						image->WidthBytes(),
// 						revertData,
// 						image->Width(),
// 						{ image->Width(), image->Height() }
// 					);
// 					ENSURE_THROW_MSG(status == ippStsNoErr, "ippiCopy_8u_C1R failed!");
// 					data = revertData;
// 				}
// 			}
// 		}
// 		
// 		return data;
// 	}

	void CBufferImp::Release(IID iid)
	{
		//if(m_acquiredBufferMap.find(iid) != m_acquiredBufferMap.end())
		//LOG_INFO_VALUE("Before erase m_acquiredBufferMap size = ", m_acquiredBufferMap.size());
		std::lock_guard<std::mutex> lock(m_mutex);
		m_acquiredBufferMap.erase(iid);
		//LOG_INFO_VALUE("After erase m_acquiredBufferMap size = ", m_acquiredBufferMap.size());
	}

	void CBufferImp::Release(std::list<IID> const& iidVec)
	{
		for (const auto &iid : iidVec)
		{
			Release(iid);
		}
	}

	void CBufferImp::Clear()
	{
		m_acquiredBufferMap.clear();
		m_acquiredBufferMap.swap(std::map<IID, std::any>());
	}

	SID CBufferImp::Initialize(ELDER::ImageInfo const& imageInfo)
	{
		ASSERT_LOG(imageInfo.bitDepth == 8 ||
			imageInfo.bitDepth == 16 ||
			imageInfo.bitDepth == 32,
			"Unsupported bit depth");
		if (m_bidMap.find(imageInfo) != m_bidMap.end())
		{
			return m_bidMap[imageInfo];
		}
		else
		{
			std::shared_ptr<ELDER::IImagePool> rawImagePool = nullptr;
			std::shared_ptr<ELDER::IImagePool> processImagePool = nullptr;
			std::shared_ptr<ELDER::CImage8u1cIPPI> image8u1c = nullptr;
			std::shared_ptr<ELDER::CImage32f1cIPPI> image32f1c = nullptr;
			if (imageInfo.bitDepth == 8)
			{
				rawImagePool = std::make_shared<ELDER::CImagePool8u1c>();
				rawImagePool->Initialize(RawBufferSize, imageInfo.size);
				processImagePool = std::make_shared<ELDER::CImagePool8u1c>();
				processImagePool->Initialize(ProcessBufferSize, imageInfo.size);
				
			}
			else if (imageInfo.bitDepth == 16)
			{
				rawImagePool = std::make_shared<ELDER::CImagePool16u1c>();
				rawImagePool->Initialize(RawBufferSize, imageInfo.size);
				processImagePool = std::make_shared<ELDER::CImagePool16u1c>();
				processImagePool->Initialize(ProcessBufferSize, imageInfo.size);
				image8u1c = std::make_shared<ELDER::CImage8u1cIPPI>();
				image8u1c->Initialize(imageInfo.size.width, imageInfo.size.height);
				image32f1c = std::make_shared<ELDER::CImage32f1cIPPI>();
				image32f1c->Initialize(imageInfo.size.width, imageInfo.size.height);				
			}
			else if (imageInfo.bitDepth == 32)
			{
				rawImagePool = std::make_shared<ELDER::CImagePool32f1c>();
				rawImagePool->Initialize(RawBufferSize, imageInfo.size);
				processImagePool = std::make_shared<ELDER::CImagePool32f1c>();
				processImagePool->Initialize(ProcessBufferSize, imageInfo.size);
				image8u1c = std::make_shared<ELDER::CImage8u1cIPPI>();
				image8u1c->Initialize(imageInfo.size.width, imageInfo.size.height);
				image32f1c = std::make_shared<ELDER::CImage32f1cIPPI>();
				image32f1c->Initialize(imageInfo.size.width, imageInfo.size.height);
			}
			++m_sid;

			m_processorInit->Initialize(m_sid, imageInfo);
			m_bidMap.emplace(imageInfo, m_sid);
			m_imageInfoMap.emplace(m_sid, imageInfo);
			m_rawImagePoolMap.emplace(m_sid, rawImagePool);
			m_processImagePoolMap.emplace(m_sid, processImagePool);
			m_iidIncMap.emplace(m_sid, 0);
			if (imageInfo.size.width % 64 != 0)
			{
				auto revertBuf = ippsMalloc_8u(imageInfo.size.width * imageInfo.size.height);
				m_revertBufMap.emplace(m_sid, revertBuf);
			}
			m_revertTmpBuf8uMap.emplace(m_sid, image8u1c);
			m_revertTmpBuf32fMap.emplace(m_sid, image32f1c);

			return m_sid;
		}
	}

	bool CBufferImp::IsInitialized() noexcept(true)
	{
		return m_isInitialized;
	}

	void CBufferImp::UnInitialize()
	{

	}

	std::any CBufferImp::AcquireRaw(SID sid)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ASSERT_LOG(m_rawImagePoolMap.find(sid) != m_rawImagePoolMap.end(), "Can not find such bid in raw buffer pool");
		return m_rawImagePoolMap[sid]->Acquire();
	}

	std::any CBufferImp::AcquireProcess(SID sid)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ASSERT_LOG(m_processImagePoolMap.find(sid) != m_processImagePoolMap.end(), "Can not find such bid in process buffer pool");
		return m_processImagePoolMap[sid]->Acquire();
	}

	SID CBufferImp::BufferID(ELDER::ImageInfo const& imageInfo)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return  Initialize(imageInfo);
	}

	IID CBufferImp::MakeID(std::any&& any, SID sid)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto iid = CreateIID(sid);
		auto anyImage = std::move(any);
		m_acquiredBufferMap.emplace(iid, std::move(anyImage));
		
		return iid;
	}

	std::any const& CBufferImp::Resume(IID iid)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ENSURE_THROW_MSG(m_acquiredBufferMap.find(iid) != m_acquiredBufferMap.end(), "Can not find such iid");
		
		return m_acquiredBufferMap.at(iid);
	}

	IID CBufferImp::CreateIID(SID sid)
	{
		auto iidInc = ++m_iidIncMap[sid];
		
		return static_cast<IID>((sid << 16) + iidInc);
	}
}

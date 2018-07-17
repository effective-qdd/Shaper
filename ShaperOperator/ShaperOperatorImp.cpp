#include "ShaperOperatorImp.h"

#include "../../Elder/Logger/Logger.hpp"
#include "../../Elder/ImageOperator/ImageCopy.hpp"
#include "../../Elder/ImageOperator/ImageCopyCrop.hpp"
#include "../../Elder/ImageOperator/ImageCopyROI.hpp"
#include "../../Elder/ImageOperator/ImageConvert.hpp"
#include "../../Elder/ImageOperator/ImageConvertCrop.hpp"
#include "../../Elder/ImageOperator/ImageConvertROI.hpp"
#include "../../Elder/ImageOperator/ImageAdd.hpp"
#include "../../Elder/ImageOperator/ImageSub.hpp"
#include "../../Elder/ImageOperator/ImageMul.hpp"
#include "../../Elder/ImageOperator/ImageDiv.hpp"
#include "../../Elder/ImageOperator/ImageAddConstant.hpp"
#include "../../Elder/ImageOperator/ImageSubConstant.hpp"
#include "../../Elder/ImageOperator/ImageMulConstant.hpp"
#include "../../Elder/ImageOperator/ImageDivConstant.hpp"
#include "../../Elder/ImageOperator/ImageMax.hpp"
#include "../../Elder/ImageOperator/ImageMin.hpp"

#include "BufferCore.h"

namespace SHAPER
{
	COperatorImp::COperatorImp(CBufferCore *bufferCore)
		: CSuperOperator()
		, m_bufferCore(bufferCore)
	{
		ASSERT_LOG(m_bufferCore != nullptr, "m_bufferCore is null");
	}

	COperatorImp::~COperatorImp()
	{
	}

	IID COperatorImp::Copy(IID iid1)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		SID bid1 = iid1 >> 16;
		auto buf = std::move(m_bufferCore->AcquireRaw(bid1));

		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			ELDER::OPERATOR::CImageCopy<ELDER::OPERATOR::CCopy8u1c>::Copy(img, img1);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			ELDER::OPERATOR::CImageCopy<ELDER::OPERATOR::CCopy16u1c>::Copy(img, img1);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			ELDER::OPERATOR::CImageCopy<ELDER::OPERATOR::CCopy32f1c>::Copy(img, img1);
		}

		return m_bufferCore->MakeID(std::move(buf), bid1);
	}

	IID COperatorImp::CopyCrop(IID iid1, ELDER::Rect roi)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		auto bid = 0;
		std::any buf;

		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			bid = m_bufferCore->BufferID({ roi.size, 8 });
			buf = std::move(m_bufferCore->AcquireRaw(bid));
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			ELDER::OPERATOR::CImageCopyCrop<ELDER::OPERATOR::CCopyCrop8u1c>::CopyCrop(img, img1, roi.point);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			bid = m_bufferCore->BufferID({ roi.size, 16 });
			buf = std::move(m_bufferCore->AcquireRaw(bid));
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			ELDER::OPERATOR::CImageCopyCrop<ELDER::OPERATOR::CCopyCrop16u1c>::CopyCrop(img, img1, roi.point);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			bid = m_bufferCore->BufferID({ roi.size, 32 });
			buf = std::move(m_bufferCore->AcquireRaw(bid));
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			ELDER::OPERATOR::CImageCopyCrop<ELDER::OPERATOR::CCopyCrop32f1c>::CopyCrop(img, img1, roi.point);
		}

		return m_bufferCore->MakeID(std::move(buf), bid);
	}

	void COperatorImp::CopyROI(IID iid1, IID iid2, ELDER::Rect roi)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		auto buf2 = m_bufferCore->Resume(iid2);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid2");

		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageCopyROI<ELDER::OPERATOR::CCopyROI8u1c>::CopyROI(img1, img2, roi.point);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageCopyROI<ELDER::OPERATOR::CCopyROI16u1c>::CopyROI(img1, img2, roi.point);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageCopyROI<ELDER::OPERATOR::CCopyROI32f1c>::CopyROI(img1, img2, roi.point);
		}
	}

// 	void COperatorImp::Convert(IID iid1, IID iid2)
// 	{
// 		auto buf1 = m_bufferCore->Resume(iid1);
// 		auto buf2 = m_bufferCore->Resume(iid2);
// 		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
// 		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
// 		auto bid1 = iid1 >> 16;
// 		auto bid2 = iid2 >> 16;
// 		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvert<ELDER::OPERATOR::CConvert8u1cTo16u1c>::Convert(img1, img2);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
// 				 buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvert<ELDER::OPERATOR::CConvert8u1cTo32f1c>::Convert(img1, img2);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
// 				 buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvert<ELDER::OPERATOR::CConvert16u1cTo32f1c>::Convert(img1, img2);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
// 				 buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvert<ELDER::OPERATOR::CConvert16u1cTo8u1c>::Convert(img1, img2);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
// 				 buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvert<ELDER::OPERATOR::CConvert32f1cTo8u1c>::Convert(img1, img2);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
// 				 buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvert<ELDER::OPERATOR::CConvert32f1cTo16u1c>::Convert(img1, img2);
// 		}
// 	}

// 	void COperatorImp::ConvertCrop(IID iid1, IID iid2, ELDER::Point offset)
// 	{
// 		auto buf1 = m_bufferCore->Resume(iid1);
// 		auto buf2 = m_bufferCore->Resume(iid2);
// 		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
// 		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
// 		auto bid1 = iid1 >> 16;
// 		auto bid2 = iid2 >> 16;
// 		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertCrop<ELDER::OPERATOR::CConvertCrop8u1cTo16u1c>::ConvertCrop(img1, img2, offset);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertCrop<ELDER::OPERATOR::CConvertCrop8u1cTo32f1c>::ConvertCrop(img1, img2, offset);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertCrop<ELDER::OPERATOR::CConvertCrop16u1cTo32f1c>::ConvertCrop(img1, img2, offset);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertCrop<ELDER::OPERATOR::CConvertCrop16u1cTo8u1c>::ConvertCrop(img1, img2, offset);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertCrop<ELDER::OPERATOR::CConvertCrop32f1cTo8u1c>::ConvertCrop(img1, img2, offset);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertCrop<ELDER::OPERATOR::CConvertCrop32f1cTo16u1c>::ConvertCrop(img1, img2, offset);
// 		}
// 	}

// 	void COperatorImp::ConvertROI(IID iid1, IID iid2, ELDER::Point offset)
// 	{
// 		auto buf1 = m_bufferCore->Resume(iid1);
// 		auto buf2 = m_bufferCore->Resume(iid2);
// 		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
// 		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
// 		auto bid1 = iid1 >> 16;
// 		auto bid2 = iid2 >> 16;
// 		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertROI<ELDER::OPERATOR::CConvertROI8u1cTo16u1c>::ConvertROI(img1, img2, offset);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertROI<ELDER::OPERATOR::CConvertROI8u1cTo32f1c>::ConvertROI(img1, img2, offset);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertROI<ELDER::OPERATOR::CConvertROI16u1cTo32f1c>::ConvertROI(img1, img2, offset);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertROI<ELDER::OPERATOR::CConvertROI16u1cTo8u1c>::ConvertROI(img1, img2, offset);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertROI<ELDER::OPERATOR::CConvertROI32f1cTo8u1c>::ConvertROI(img1, img2, offset);
// 		}
// 		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
// 			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
// 		{
// 			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
// 			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
// 			ELDER::OPERATOR::CImageConvertROI<ELDER::OPERATOR::CConvertROI32f1cTo16u1c>::ConvertROI(img1, img2, offset);
// 		}
// 	}

	IID COperatorImp::Add(IID iid1, IID iid2)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		auto buf2 = m_bufferCore->Resume(iid2);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
		SID bid1 = iid1 >> 16;
		SID bid2 = iid2 >> 16;
		ENSURE_THROW_MSG(bid1 == bid2, "Bid mismatch");
		auto buf = std::move(m_bufferCore->AcquireRaw(bid1));
		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			auto img2= std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageAdd<ELDER::OPERATOR::CAdd8u1c>::Add(img, img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageAdd<ELDER::OPERATOR::CAdd16u1c>::Add(img, img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageAdd<ELDER::OPERATOR::CAdd32f1c>::Add(img, img1, img2);
		}

		return m_bufferCore->MakeID(std::move(buf), bid1);
	}

	void COperatorImp::AddI(IID iid1, IID iid2)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		auto buf2 = m_bufferCore->Resume(iid2);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
		SID bid1 = iid1 >> 16;
		SID bid2 = iid2 >> 16;
		ENSURE_THROW_MSG(bid1 == bid2, "Bid mismatch");
		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageAdd<ELDER::OPERATOR::CAdd8u1c>::Add(img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageAdd<ELDER::OPERATOR::CAdd16u1c>::Add(img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageAdd<ELDER::OPERATOR::CAdd32f1c>::Add(img1, img2);
		}
	}

	IID COperatorImp::Sub(IID iid1, IID iid2)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		auto buf2 = m_bufferCore->Resume(iid2);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
		SID bid1 = iid1 >> 16;
		SID bid2 = iid2 >> 16;
		ENSURE_THROW_MSG(bid1 == bid2, "Bid mismatch");
		auto buf = std::move(m_bufferCore->AcquireRaw(bid1));
		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageSub<ELDER::OPERATOR::CSub8u1c>::Sub(img, img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageSub<ELDER::OPERATOR::CSub16u1c>::Sub(img, img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageSub<ELDER::OPERATOR::CSub32f1c>::Sub(img, img1, img2);
		}

		return m_bufferCore->MakeID(std::move(buf), bid1);
	}

	void COperatorImp::SubI(IID iid1, IID iid2)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		auto buf2 = m_bufferCore->Resume(iid2);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
		SID bid1 = iid1 >> 16;
		SID bid2 = iid2 >> 16;
		ENSURE_THROW_MSG(bid1 == bid2, "Bid mismatch");
		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageSub<ELDER::OPERATOR::CSub8u1c>::Sub(img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageSub<ELDER::OPERATOR::CSub16u1c>::Sub(img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageSub<ELDER::OPERATOR::CSub32f1c>::Sub(img1, img2);
		}
	}

	IID COperatorImp::Mul(IID iid1, IID iid2)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		auto buf2 = m_bufferCore->Resume(iid2);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
		SID bid1 = iid1 >> 16;
		SID bid2 = iid2 >> 16;
		ENSURE_THROW_MSG(bid1 == bid2, "Bid mismatch");
		auto buf = std::move(m_bufferCore->AcquireRaw(bid1));
		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageMul<ELDER::OPERATOR::CMul8u1c>::Mul(img, img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageMul<ELDER::OPERATOR::CMul16u1c>::Mul(img, img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageMul<ELDER::OPERATOR::CMul32f1c>::Mul(img, img1, img2);
		}

		return m_bufferCore->MakeID(std::move(buf), bid1);
	}

	void COperatorImp::MulI(IID iid1, IID iid2)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		auto buf2 = m_bufferCore->Resume(iid2);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
		SID bid1 = iid1 >> 16;
		SID bid2 = iid2 >> 16;
		ENSURE_THROW_MSG(bid1 == bid2, "Bid mismatch");
		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageMul<ELDER::OPERATOR::CMul8u1c>::Mul(img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageMul<ELDER::OPERATOR::CMul16u1c>::Mul(img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageMul<ELDER::OPERATOR::CMul32f1c>::Mul(img1, img2);
		}
	}

	IID COperatorImp::Div(IID iid1, IID iid2)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		auto buf2 = m_bufferCore->Resume(iid2);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
		SID bid1 = iid1 >> 16;
		SID bid2 = iid2 >> 16;
		ENSURE_THROW_MSG(bid1 == bid2, "Bid mismatch");
		auto buf = std::move(m_bufferCore->AcquireRaw(bid1));
		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageDiv<ELDER::OPERATOR::CDiv8u1c>::Div(img, img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageDiv<ELDER::OPERATOR::CDiv16u1c>::Div(img, img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageDiv<ELDER::OPERATOR::CDiv32f1c>::Div(img, img1, img2);
		}

		return m_bufferCore->MakeID(std::move(buf), bid1);
	}

	void COperatorImp::DivI(IID iid1, IID iid2)
	{
		auto buf1 = m_bufferCore->Resume(iid1);
		auto buf2 = m_bufferCore->Resume(iid2);
		ASSERT_LOG(buf1.has_value(), "Invalid buf1 with iid1");
		ASSERT_LOG(buf2.has_value(), "Invalid buf2 with iid1");
		SID bid1 = iid1 >> 16;
		SID bid2 = iid2 >> 16;
		ENSURE_THROW_MSG(bid1 == bid2, "Bid mismatch");
		if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageDiv<ELDER::OPERATOR::CDiv8u1c>::Div(img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageDiv<ELDER::OPERATOR::CDiv16u1c>::Div(img1, img2);
		}
		else if (buf1.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>) &&
			buf2.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img1 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf1);
			auto img2 = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf2);
			ELDER::OPERATOR::CImageDiv<ELDER::OPERATOR::CDiv32f1c>::Div(img1, img2);
		}
	}

	IID COperatorImp::AddConstant(IID iid, float v)
	{
		auto buf = m_bufferCore->Resume(iid);
		ASSERT_LOG(buf.has_value(), "Invalid buf1 with iid1");
		SID bid = iid >> 16;
		auto buffer = std::move(m_bufferCore->AcquireRaw(bid));
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageAddConstant<ELDER::OPERATOR::CAddConstant8u1c>::AddConstant(image, img, static_cast<ELDER::IDT8U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageAddConstant<ELDER::OPERATOR::CAddConstant16u1c>::AddConstant(image, img, static_cast<ELDER::IDT16U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageAddConstant<ELDER::OPERATOR::CAddConstant32f1c>::AddConstant(image, img, static_cast<ELDER::IDT32F>(v));
		}

		return m_bufferCore->MakeID(std::move(buffer), bid);
	}

	void COperatorImp::AddConstantI(IID iid, float v)
	{
		auto buf = m_bufferCore->Resume(iid);
		ASSERT_LOG(buf.has_value(), "Invalid buf1 with iid1");
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			ELDER::OPERATOR::CImageAddConstant<ELDER::OPERATOR::CAddConstant8u1c>::AddConstant(img, static_cast<ELDER::IDT8U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			ELDER::OPERATOR::CImageAddConstant<ELDER::OPERATOR::CAddConstant16u1c>::AddConstant(img, static_cast<ELDER::IDT16U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			ELDER::OPERATOR::CImageAddConstant<ELDER::OPERATOR::CAddConstant32f1c>::AddConstant(img, static_cast<ELDER::IDT32F>(v));
		}
	}

	IID COperatorImp::SubConstant(IID iid, float v)
	{
		auto buf = m_bufferCore->Resume(iid);
		ASSERT_LOG(buf.has_value(), "Invalid buf1 with iid1");
		SID bid = iid >> 16;
		auto buffer = std::move(m_bufferCore->AcquireRaw(bid));
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageSubConstant<ELDER::OPERATOR::CSubConstant8u1c>::SubConstant(image, img, static_cast<ELDER::IDT8U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageSubConstant<ELDER::OPERATOR::CSubConstant16u1c>::SubConstant(image, img, static_cast<ELDER::IDT16U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageSubConstant<ELDER::OPERATOR::CSubConstant32f1c>::SubConstant(image, img, static_cast<ELDER::IDT32F>(v));
		}

		return m_bufferCore->MakeID(std::move(buffer), bid);
	}

	void COperatorImp::SubConstantI(IID iid, float v)
	{
		auto buf = m_bufferCore->Resume(iid);
		ASSERT_LOG(buf.has_value(), "Invalid buf1 with iid1");
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			ELDER::OPERATOR::CImageSubConstant<ELDER::OPERATOR::CSubConstant8u1c>::SubConstant(img, static_cast<ELDER::IDT8U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			ELDER::OPERATOR::CImageSubConstant<ELDER::OPERATOR::CSubConstant16u1c>::SubConstant(img, static_cast<ELDER::IDT16U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			ELDER::OPERATOR::CImageSubConstant<ELDER::OPERATOR::CSubConstant32f1c>::SubConstant(img, static_cast<ELDER::IDT32F>(v));
		}
	}

	IID COperatorImp::MulConstant(IID iid, float v)
	{
		auto buf = m_bufferCore->Resume(iid);
		ASSERT_LOG(buf.has_value(), "Invalid buf1 with iid1");
		SID bid = iid >> 16;
		auto buffer = std::move(m_bufferCore->AcquireRaw(bid));
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageMulConstant<ELDER::OPERATOR::CMulConstant8u1c>::MulConstant(image, img, static_cast<ELDER::IDT8U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageMulConstant<ELDER::OPERATOR::CMulConstant16u1c>::MulConstant(image, img, static_cast<ELDER::IDT16U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageMulConstant<ELDER::OPERATOR::CMulConstant32f1c>::MulConstant(image, img, static_cast<ELDER::IDT32F>(v));
		}

		return m_bufferCore->MakeID(std::move(buffer), bid);
	}

	void COperatorImp::MulConstantI(IID iid, float v)
	{
		auto buf = m_bufferCore->Resume(iid);
		ASSERT_LOG(buf.has_value(), "Invalid buf1 with iid1");
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			ELDER::OPERATOR::CImageMulConstant<ELDER::OPERATOR::CMulConstant8u1c>::MulConstant(img, static_cast<ELDER::IDT8U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			ELDER::OPERATOR::CImageMulConstant<ELDER::OPERATOR::CMulConstant16u1c>::MulConstant(img, static_cast<ELDER::IDT16U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			ELDER::OPERATOR::CImageMulConstant<ELDER::OPERATOR::CMulConstant32f1c>::MulConstant(img, static_cast<ELDER::IDT32F>(v));
		}
	}

	IID COperatorImp::DivConstant(IID iid, float v)
	{
		auto buf = m_bufferCore->Resume(iid);
		ASSERT_LOG(buf.has_value(), "Invalid buf1 with iid1");
		SID bid = iid >> 16;
		auto buffer = std::move(m_bufferCore->AcquireRaw(bid));
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageDivConstant<ELDER::OPERATOR::CDivConstant8u1c>::DivConstant(image, img, static_cast<ELDER::IDT8U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageDivConstant<ELDER::OPERATOR::CDivConstant16u1c>::DivConstant(image, img, static_cast<ELDER::IDT16U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			auto image = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buffer);
			ELDER::OPERATOR::CImageDivConstant<ELDER::OPERATOR::CDivConstant32f1c>::DivConstant(image, img, static_cast<ELDER::IDT32F>(v));
		}

		return m_bufferCore->MakeID(std::move(buffer), bid);
	}

	void COperatorImp::DivConstantI(IID iid, float v)
	{
		auto buf = m_bufferCore->Resume(iid);
		ASSERT_LOG(buf.has_value(), "Invalid buf1 with iid1");
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			ELDER::OPERATOR::CImageDivConstant<ELDER::OPERATOR::CDivConstant8u1c>::DivConstant(img, static_cast<ELDER::IDT8U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			ELDER::OPERATOR::CImageDivConstant<ELDER::OPERATOR::CDivConstant16u1c>::DivConstant(img, static_cast<ELDER::IDT16U>(v));
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			ELDER::OPERATOR::CImageDivConstant<ELDER::OPERATOR::CDivConstant32f1c>::DivConstant(img, static_cast<ELDER::IDT32F>(v));
		}
	}

	float COperatorImp::Max(IID iid)
	{
		auto buf = m_bufferCore->Resume(iid);
		ASSERT_LOG(buf.has_value(), "Invalid buf1 with iid1");
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			return ELDER::OPERATOR::CImageMax<ELDER::OPERATOR::CMax8u1c>::Max(img);
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			return ELDER::OPERATOR::CImageMax<ELDER::OPERATOR::CMax16u1c>::Max(img);
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			return ELDER::OPERATOR::CImageMax<ELDER::OPERATOR::CMax32f1c>::Max(img);
		}

		return 0.0f;
	}

	float COperatorImp::Min(IID iid)
	{
		auto buf = m_bufferCore->Resume(iid);
		ASSERT_LOG(buf.has_value(), "Invalid buf1 with iid1");
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
			return ELDER::OPERATOR::CImageMin<ELDER::OPERATOR::CMin8u1c>::Min(img);
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf);
			return ELDER::OPERATOR::CImageMin<ELDER::OPERATOR::CMin16u1c>::Min(img);
		}
		else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
		{
			auto img = std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf);
			return ELDER::OPERATOR::CImageMin<ELDER::OPERATOR::CMin32f1c>::Min(img);
		}

		return 0.0f;
	}
}
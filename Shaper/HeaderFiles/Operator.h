#ifndef OPERATOR_H
#define OPERATOR_H

#include "Defines.h"

namespace SHAPER
{
	class COperator
	{
	public:
		COperator() {}

		virtual ~COperator() {}

		/// Copy an image with specify iid.
		virtual IID Copy(IID) noexcept(false) = 0;

		/// Copy a small image from a big image with the crop rect.
		virtual IID CopyCrop(IID, ELDER::Rect) noexcept(false) = 0;

		/// Copy an image to a big image with the roi rect.
		/// Param1 : the destination iid which is big.
		/// Param2 : the source iid which is small.
		virtual void CopyROI(IID, IID, ELDER::Rect) noexcept(false) = 0;

// 		virtual void Convert(IID, IID) noexcept(false) = 0;
// 
// 		virtual void ConvertCrop(IID, IID, ELDER::Point) noexcept(false) = 0;
// 
// 		virtual void ConvertROI(IID, IID, ELDER::Point) noexcept(false) = 0;

		/// Add two image and return a new image.
		/// C = A + B
		virtual IID Add(IID, IID) noexcept(false) = 0;

		/// Add two image and take the place of first iid.
		/// A = A + B
		virtual void AddI(IID, IID) noexcept(false) = 0;

		/// Sub two image and return a new image.
		virtual IID Sub(IID, IID) noexcept(false) = 0;

		/// Sub two image and take the place of first iid.
		virtual void SubI(IID, IID) noexcept(false) = 0;

		/// Mul two image and return a new image.
		virtual IID Mul(IID, IID) noexcept(false) = 0;

		/// Mul two image and take the place of first iid.
		virtual void MulI(IID, IID) noexcept(false) = 0;

		/// Div two image and return a new image.
		virtual IID Div(IID, IID) noexcept(false) = 0;

		/// Div two image and take the place of first iid.
		virtual void DivI(IID, IID) noexcept(false) = 0;

		/// Add a constant value to the image and return a new image.
		virtual IID AddConstant(IID, float) noexcept(false) = 0;

		/// Add a constant value to the image and take the place of first iid.
		virtual void AddConstantI(IID, float) noexcept(false) = 0;

		/// Sub a constant value to the image and return a new image.
		virtual IID SubConstant(IID, float) noexcept(false) = 0;

		/// Sub a constant value to the image and take the place of first iid.
		virtual void SubConstantI(IID, float) noexcept(false) = 0;

		/// Mul a constant value to the image and return a new image.
		virtual IID MulConstant(IID, float) noexcept(false) = 0;

		/// Mul a constant value to the image and take the place of first iid.
		virtual void MulConstantI(IID, float) noexcept(false) = 0;

		/// Div a constant value to the image and return a new image.
		virtual IID DivConstant(IID, float) noexcept(false) = 0;

		/// Div a constant value to the image and take the place of first iid.
		virtual void DivConstantI(IID, float) noexcept(false) = 0;

		/// Get the max pixel value of the image
		virtual float Max(IID) noexcept(false) = 0;

		/// Get the min pixel value of the image
		virtual float Min(IID) noexcept(false) = 0;
	};
}

#endif
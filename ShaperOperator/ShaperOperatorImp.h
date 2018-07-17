#ifndef SHAPER_OPERATOR_IMP_H
#define SHAPER_OPERATOR_IMP_H

#include "SuperOperator.h"

namespace SHAPER
{
	class CBufferCore;
	class COperatorImp final : public CSuperOperator
	{
	public:
		COperatorImp(CBufferCore *);

		~COperatorImp();

		IID Copy(IID);

		IID CopyCrop(IID, ELDER::Rect);

		void CopyROI(IID, IID, ELDER::Rect);

// 		void Convert(IID, IID);
// 
// 		void ConvertCrop(IID, IID, ELDER::Point);
// 
// 		void ConvertROI(IID, IID, ELDER::Point);

		IID Add(IID, IID);

		void AddI(IID, IID);

		IID Sub(IID, IID);

		void SubI(IID, IID);

		IID Mul(IID, IID);

		void MulI(IID, IID);

		IID Div(IID, IID);

		void DivI(IID, IID);

		IID AddConstant(IID, float);

		void AddConstantI(IID, float);

		IID SubConstant(IID, float);

		void SubConstantI(IID, float);

		IID MulConstant(IID, float);

		void MulConstantI(IID, float);

		IID DivConstant(IID, float);

		void DivConstantI(IID, float);

		float Max(IID);

		float Min(IID);

	private:
		CBufferCore * m_bufferCore;
	};
}

#endif
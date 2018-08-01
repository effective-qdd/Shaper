#include <windows.h>

#include "../../Elder/Singleton/Singleton.hpp"
#include "../../Elder/Exception/Exception.hpp"
#include "ShaperPy.h"
#include "Shaper.h"

typedef std::shared_ptr<SHAPER::CShaper>(*ShaperInstanceFunc)();

std::shared_ptr<SHAPER::CShaper> g_Shaper = nullptr;
SHAPER::CBuffer *g_shaperBuffer = nullptr;
SHAPER::CFile *g_shaperFile = nullptr;
SHAPER::CStream *g_shaperFileStream = nullptr;
SHAPER::CProcess *g_shaperProcess = nullptr;
SHAPER::CProcessor *g_shaperProcessor = nullptr;
SHAPER::CFilter *g_shaperProcessorFilter = nullptr;
SHAPER::CTransform *g_shaperProcessorTransform = nullptr;
SHAPER::CIntensity *g_shaperProcessorIntensity = nullptr;
SHAPER::COperator *g_shaperOperator = nullptr;
SHAPER::CDisplay *g_shaperDisplay = nullptr;

ShaperPy::ShaperPy()
{
	HINSTANCE shaperLib = LoadLibrary("Shaper.dll");
	if (!shaperLib) throw std::exception("Load shaper library failed!");
	auto shaperInstFunc = (ShaperInstanceFunc)GetProcAddress(shaperLib, "ShaperInstance");
	if (shaperInstFunc == nullptr) throw std::exception("Get shaper instance failed!");
	g_Shaper = shaperInstFunc();
	g_Shaper->Initialize();

	g_shaperBuffer = g_Shaper->Buffer();
	ENSURE_THROW_MSG(g_shaperBuffer != nullptr, "Shaper buffer is null");

	g_shaperFile = g_Shaper->File();
	ENSURE_THROW_MSG(g_shaperFile != nullptr, "Shaper file is null");

	g_shaperProcess = g_Shaper->Process();
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");

	g_shaperProcessor = g_Shaper->Processor();
	ENSURE_THROW_MSG(g_shaperProcessor != nullptr, "Shaper processor is null");

	g_shaperOperator = g_Shaper->Operator();
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	g_shaperDisplay = g_Shaper->Display();
	ENSURE_THROW_MSG(g_shaperDisplay != nullptr, "Shaper display is null");
}

ShaperPy::~ShaperPy()
{
	
}

FilePy *ShaperPy::file()
{
	return ELDER::CSingleton<FilePy>::GetInstance().get();
}

BufferPy *ShaperPy::buffer()
{
	return ELDER::CSingleton<BufferPy>::GetInstance().get();
}

ProcessPy *ShaperPy::process()
{
	return ELDER::CSingleton<ProcessPy>::GetInstance().get();
}

ProcessorPy *ShaperPy::processor()
{
	return ELDER::CSingleton<ProcessorPy>::GetInstance().get();
}

OperatorPy * ShaperPy::operate()
{
	return ELDER::CSingleton<OperatorPy>::GetInstance().get();
}

DisplayPy *ShaperPy::display()
{
	return ELDER::CSingleton<DisplayPy>::GetInstance().get();
}


//////////////////////////////////////////////////////////////////////////
/// StreamPy
ShaperPy::IID StreamPy::first()
{
	ENSURE_THROW_MSG(g_shaperFileStream != nullptr, "Shaper file stream is null");// throw std::exception("Shaper file stream is null");
	return g_shaperFileStream->First();
}

ShaperPy::IID StreamPy::last()
{
	ENSURE_THROW_MSG(g_shaperFileStream != nullptr, "Shaper file stream is null");// throw std::exception("Shaper file stream is null");
	return g_shaperFileStream->Last();
}

ShaperPy::IID StreamPy::index(unsigned long index)
{
	ENSURE_THROW_MSG(g_shaperFileStream != nullptr, "Shaper file stream is null");// throw std::exception("Shaper file stream is null");
	return g_shaperFileStream->Index(index);
}


//////////////////////////////////////////////////////////////////////////
/// FilePy
ShaperPy::FID FilePy::open(std::string const& filePath, ShaperPy::FileTypes fileType)
{
	ENSURE_THROW_MSG(g_shaperFile != nullptr, "Shaper file is null");
	return g_shaperFile->Open(filePath, static_cast<SHAPER::FileTypes>(fileType));
}

StreamPy *FilePy::stream(ShaperPy::FID fid)
{
	ENSURE_THROW_MSG(g_shaperFile != nullptr, "Shaper file is null");
	g_shaperFileStream = g_shaperFile->Stream(fid);

	return ELDER::CSingleton<StreamPy>::GetInstance().get();
}

void FilePy::close(ShaperPy::FID fid)
{
	ENSURE_THROW_MSG(g_shaperFile != nullptr, "Shaper file is null");
	g_shaperFile->Close(fid);
}

void FilePy::save(ShaperPy::IID iid, std::string const& filePath, ShaperPy::FileTypes fileType)
{
	ENSURE_THROW_MSG(g_shaperFile != nullptr, "Shaper file is null");
	g_shaperFile->Save(iid, filePath, static_cast<SHAPER::FileTypes>(fileType));
}


//////////////////////////////////////////////////////////////////////////
/// BufferPy
ShaperPy::IID BufferPy::acquireUint8(unsigned char const* data, int w, int h)
{
	ENSURE_THROW_MSG(g_shaperBuffer != nullptr, "Shaper buffer is null");
	return g_shaperBuffer->Acquire(data, { { w, h }, 8 });
}

ShaperPy::IID BufferPy::acquireUint16(unsigned short const* data, int w, int h)
{
	ENSURE_THROW_MSG(g_shaperBuffer != nullptr, "Shaper buffer is null");
	return g_shaperBuffer->Acquire(data, { { w, h }, 16 });
}

ShaperPy::IID BufferPy::acquireFloat32(float const* data, int w, int h)
{
	ENSURE_THROW_MSG(g_shaperBuffer != nullptr, "Shaper buffer is null");
	return g_shaperBuffer->Acquire(data, { { w, h }, 32 });
}

void BufferPy::release(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperBuffer != nullptr, "Shaper buffer is null");
	g_shaperBuffer->Release(iid);
}

void BufferPy::release(const std::list<ShaperPy::IID> &fidVect)
{
	ENSURE_THROW_MSG(g_shaperBuffer != nullptr, "Shaper buffer is null");
	g_shaperBuffer->Release(fidVect);
}


//////////////////////////////////////////////////////////////////////////
/// ProcessPy
ShaperPy::PID ProcessPy::createList()
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->CreateList();
}

void ProcessPy::attach(ShaperPy::PID pid, ShaperPy::ProcessorTypes processorType)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	g_shaperProcess->Attach(pid, static_cast<SHAPER::ProcessorTypes>(processorType));
}

void ProcessPy::detach(ShaperPy::PID pid, ShaperPy::ProcessorTypes processorType)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	g_shaperProcess->Detach(pid, static_cast<SHAPER::ProcessorTypes>(processorType));
}

bool ProcessPy::has(ShaperPy::PID pid, ShaperPy::ProcessorTypes processorType)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->Has(pid, static_cast<SHAPER::ProcessorTypes>(processorType));
}

bool ProcessPy::empty(ShaperPy::PID pid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->Empty(pid);
}

ShaperPy::IID ProcessPy::apply(ShaperPy::PID pid, ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->Apply(pid, iid);
}

ShaperPy::IID ProcessPy::applyFilterMedian3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterMedian3x3(iid);
}

ShaperPy::IID ProcessPy::applyFilterMedian5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterMedian5x5(iid);
}

ShaperPy::IID ProcessPy::applyFilterGaussian3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterGaussian3x3(iid);
}

ShaperPy::IID ProcessPy::applyFilterGaussian5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterGaussian5x5(iid);
}

ShaperPy::IID ProcessPy::applyFilterMean3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterMean3x3(iid);
}

ShaperPy::IID ProcessPy::applyFilterMean5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterMean5x5(iid);
}

ShaperPy::IID ProcessPy::applyFilterMaximum3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterMaximum3x3(iid);
}

ShaperPy::IID ProcessPy::applyFilterMaximum5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterMaximum5x5(iid);
}

ShaperPy::IID ProcessPy::applyFilterMinimum3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterMinimum3x3(iid);
}

ShaperPy::IID ProcessPy::applyFilterMinimum5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterMinimum5x5(iid);
}

ShaperPy::IID ProcessPy::applyFilterLaplace3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterLaplace3x3(iid);
}

ShaperPy::IID ProcessPy::applyFilterLaplace5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterLaplace5x5(iid);
}

ShaperPy::IID ProcessPy::applyFilterHighPass3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterHighPass3x3(iid);
}

ShaperPy::IID ProcessPy::applyFilterHighPass5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterHighPass5x5(iid);
}

ShaperPy::IID ProcessPy::applyFilterLowPass3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterLowPass3x3(iid);
}

ShaperPy::IID ProcessPy::applyFilterLowPass5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterLowPass5x5(iid);
}

ShaperPy::IID ProcessPy::applyFilterSobel3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterSobel3x3(iid);
}

ShaperPy::IID ProcessPy::applyFilterSobel5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterSobel5x5(iid);
}

ShaperPy::IID ProcessPy::applyFilterBilateral(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFilterBilateral(iid);
}

ShaperPy::IID ProcessPy::applyIntensityHistogramEqual(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyIntensityHistogramEqual(iid);
}

ShaperPy::IID ProcessPy::applyIntensityHistogramLocal(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyIntensityHistogramEqual(iid);
}

ShaperPy::IID ProcessPy::applyIntensityNegative(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyIntensityNegative(iid);
}

ShaperPy::IID ProcessPy::applyIntensityLog(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyIntensityLog(iid);
}

ShaperPy::IID ProcessPy::applyIntensityGamma(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyIntensityGamma(iid);
}

ShaperPy::IID ProcessPy::applyIntensityLinear(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyIntensityLinear(iid);
}

ShaperPy::IID ProcessPy::applyIntensityPiecewiseLinear(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyIntensityPiecewiseLinear(iid);
}

ShaperPy::IID ProcessPy::applyIntensityBrightnessContrast(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyIntensityBrightnessContrast(iid);
}

ShaperPy::IID ProcessPy::applyIntensityThreshold(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyIntensityThreshold(iid);
}

ShaperPy::IID ProcessPy::applyMorphologyDilate3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyMorphologyDilate3x3(iid);
}

ShaperPy::IID ProcessPy::applyMorphologyDilate5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyMorphologyDilate5x5(iid);
}

ShaperPy::IID ProcessPy::applyMorphologyErode3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyMorphologyErode3x3(iid);
}

ShaperPy::IID ProcessPy::applyMorphologyErode5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyMorphologyErode5x5(iid);
}

ShaperPy::IID ProcessPy::applyMorphologyOpen3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyMorphologyOpen3x3(iid);
}

ShaperPy::IID ProcessPy::applyMorphologyOpen5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyMorphologyOpen5x5(iid);
}

ShaperPy::IID ProcessPy::applyMorphologyClose3x3(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyMorphologyClose3x3(iid);
}

ShaperPy::IID ProcessPy::applyMorphologyClose5x5(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyMorphologyClose5x5(iid);
}

ShaperPy::IID ProcessPy::applyTransformRotation(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyTransformRotation(iid);
}

ShaperPy::IID ProcessPy::applyTransformFlip(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyTransformFlip(iid);
}

ShaperPy::IID ProcessPy::applyFFT(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyFFT(iid);
}

ShaperPy::IID ProcessPy::applyAccumulation(std::list<ShaperPy::IID> const& iidList)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyAccumulation(iidList);
}

ShaperPy::IID ProcessPy::applyAverage(std::list<ShaperPy::IID> const& iidList)
{
	ENSURE_THROW_MSG(g_shaperProcess != nullptr, "Shaper process is null");
	return g_shaperProcess->ApplyAverage(iidList);
}

//////////////////////////////////////////////////////////////////////////
/// FilterPy
void FilterPy::setGaussianSigma(float value)
{
	ENSURE_THROW_MSG(g_shaperProcessorFilter != nullptr, "Shaper processor filter is null");
	g_shaperProcessorFilter->SetGaussianSigma(value);
}

void FilterPy::setBilateralRadius(int radius)
{
	ENSURE_THROW_MSG(g_shaperProcessorFilter != nullptr, "Shaper processor filter is null");
	g_shaperProcessorFilter->SetBilateralRadius(radius);
}

void FilterPy::setBilateralSigmaColor(double sigmaColor)
{
	ENSURE_THROW_MSG(g_shaperProcessorFilter != nullptr, "Shaper processor filter is null");
	g_shaperProcessorFilter->SetBilateralSigmaColor(sigmaColor);
}

void FilterPy::setBilateralSigmaSpace(double sigmaSpace)
{
	ENSURE_THROW_MSG(g_shaperProcessorFilter != nullptr, "Shaper processor filter is null");
	g_shaperProcessorFilter->SetBilateralSigmaSpace(sigmaSpace);
}


//////////////////////////////////////////////////////////////////////////
/// RotationPy
void TransformPy::setRotationSpecialAngle(ShaperPy::SpecialAngles specialAngle)
{
	ENSURE_THROW_MSG(g_shaperProcessorTransform != nullptr, "Shaper processor transform is null");
	g_shaperProcessorTransform->SetRotationSpecialAngle(static_cast<SHAPER::SpecialAngles>(specialAngle));
}

void TransformPy::setRotationArbitraryAngle(double angle)
{
	ENSURE_THROW_MSG(g_shaperProcessorTransform != nullptr, "Shaper processor transform is null");
	g_shaperProcessorTransform->SetRotationArbitraryAngle(angle);
}

void TransformPy::setFlipMode(ShaperPy::FlipModes flipMode)
{
	ENSURE_THROW_MSG(g_shaperProcessorTransform != nullptr, "Shaper processor transform is null");
	g_shaperProcessorTransform->SetMode(static_cast<SHAPER::FlipModes>(flipMode));
}


//////////////////////////////////////////////////////////////////////////
/// ProcessorPy
void IntensityPy::setGammaValue(float gammaValue)
{
	ENSURE_THROW_MSG(g_shaperProcessorIntensity != nullptr, "Shaper processor intensity is null");
	g_shaperProcessorIntensity->SetGammaValue(gammaValue);
}

void IntensityPy::setLogValue(double logValue)
{
	ENSURE_THROW_MSG(g_shaperProcessorIntensity != nullptr, "Shaper processor intensity is null");
	g_shaperProcessorIntensity->SetLogValue(logValue);
}

void IntensityPy::setLinearPoint(int lowIn, int lowOut, int highIn, int highOut)
{
	ENSURE_THROW_MSG(g_shaperProcessorIntensity != nullptr, "Shaper processor intensity is null");
	g_shaperProcessorIntensity->SetLinearPoint(lowIn, lowOut, highIn, highOut);
}

void IntensityPy::setPiecewiseLinearPoint(int lowIn, int lowOut, int highIn, int highOut)
{
	ENSURE_THROW_MSG(g_shaperProcessorIntensity != nullptr, "Shaper processor intensity is null");
	g_shaperProcessorIntensity->SetPiecewiseLinearPoint(lowIn, lowOut, highIn, highOut);
}

void IntensityPy::setContrastBrightness(double contrast, double brightness)
{
	ENSURE_THROW_MSG(g_shaperProcessorIntensity != nullptr, "Shaper processor intensity is null");
	g_shaperProcessorIntensity->SetContrastBrightness(contrast, brightness);
}


//////////////////////////////////////////////////////////////////////////
/// ProcessorPy
FilterPy *ProcessorPy::filter()
{
	ENSURE_THROW_MSG(g_shaperProcessor != nullptr, "Shaper processor is null");
	g_shaperProcessorFilter = g_shaperProcessor->Filter();

	return ELDER::CSingleton<FilterPy>::GetInstance().get();
}

TransformPy *ProcessorPy::transform()
{
	ENSURE_THROW_MSG(g_shaperProcessor != nullptr, "Shaper processor is null");
	g_shaperProcessorTransform = g_shaperProcessor->Transform();

	return ELDER::CSingleton<TransformPy>::GetInstance().get();
}

IntensityPy *ProcessorPy::intensity()
{
	ENSURE_THROW_MSG(g_shaperProcessor != nullptr, "Shaper processor is null");
	g_shaperProcessorIntensity = g_shaperProcessor->Intensity();

	return ELDER::CSingleton<IntensityPy>::GetInstance().get();
}


//////////////////////////////////////////////////////////////////////////
/// DisplayPy
ShaperPy::IID OperatorPy::copy(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	return g_shaperOperator->Copy(iid);
}

ShaperPy::IID OperatorPy::copyCrop(ShaperPy::IID iid, int x, int y, int w, int h)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	return g_shaperOperator->CopyCrop(iid, { { x, y}, { w, h} });
}

void OperatorPy::copyROI(ShaperPy::IID iid1, ShaperPy::IID iid2, int x, int y, int w , int h)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	g_shaperOperator->CopyROI(iid1, iid2, { {x, y },{ w, h } });
}

ShaperPy::IID OperatorPy::add(ShaperPy::IID iid1, ShaperPy::IID iid2)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	return g_shaperOperator->Add(iid1, iid2);
}

void OperatorPy::addI(ShaperPy::IID iid, ShaperPy::IID iid1)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	g_shaperOperator->AddI(iid, iid1);
}

ShaperPy::IID OperatorPy::sub(ShaperPy::IID iid1, ShaperPy::IID iid2)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	return g_shaperOperator->Sub(iid1, iid2);
}

void OperatorPy::subI(ShaperPy::IID iid, ShaperPy::IID iid1)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");
	g_shaperOperator->SubI(iid, iid1);
}

ShaperPy::IID OperatorPy::mul(ShaperPy::IID iid1, ShaperPy::IID iid2)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	return g_shaperOperator->Mul(iid1, iid2);
}

void OperatorPy::mulI(ShaperPy::IID iid, ShaperPy::IID iid1)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");
	g_shaperOperator->MulI(iid, iid1);
}

ShaperPy::IID OperatorPy::div(ShaperPy::IID iid1, ShaperPy::IID iid2)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	return g_shaperOperator->Div(iid1, iid2);
}

void OperatorPy::divI(ShaperPy::IID iid, ShaperPy::IID iid1)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");
	g_shaperOperator->DivI(iid, iid1);
}

ShaperPy::IID OperatorPy::addConstant(ShaperPy::IID iid1, float v)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	return g_shaperOperator->AddConstant(iid1, v);
}

void OperatorPy::addConstantI(ShaperPy::IID iid, float v)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");
	g_shaperOperator->AddConstantI(iid, v);
}

ShaperPy::IID OperatorPy::subConstant(ShaperPy::IID iid1, float v)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	return g_shaperOperator->SubConstant(iid1, v);
}

void OperatorPy::subConstantI(ShaperPy::IID iid, float v)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");
	g_shaperOperator->SubConstantI(iid, v);
}

ShaperPy::IID OperatorPy::mulConstant(ShaperPy::IID iid1, float v)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	return g_shaperOperator->MulConstant(iid1, v);
}

void OperatorPy::mulConstantI(ShaperPy::IID iid, float v)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");
	g_shaperOperator->MulConstantI(iid, v);
}

ShaperPy::IID OperatorPy::divConstant(ShaperPy::IID iid1, float v)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");

	return g_shaperOperator->DivConstant(iid1, v);
}

void OperatorPy::divConstantI(ShaperPy::IID iid, float v)
{
	ENSURE_THROW_MSG(g_shaperOperator != nullptr, "Shaper operator is null");
	g_shaperOperator->DivConstantI(iid, v);
}

//////////////////////////////////////////////////////////////////////////
/// DisplayPy
void DisplayPy::show(ShaperPy::IID iid)
{
	ENSURE_THROW_MSG(g_shaperDisplay != nullptr, "Shaper display is null");
	g_shaperDisplay->Show(iid);
}
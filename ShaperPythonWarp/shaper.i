%module shaper

%include std_string.i
%include std_list.i

%{
#define SWIG_FILE_WITH_INIT
#include "ShaperPy.h"
%}


%include "numpy.i"

%init %{
import_array();
%}

%apply (unsigned char* IN_ARRAY2, int DIM1, int DIM2) {(unsigned char* , int, int)}
%apply (unsigned short* IN_ARRAY2, int DIM1, int DIM2) {(unsigned short* , int, int)}
%apply (float* IN_ARRAY2, int DIM1, int DIM2) {(float* , int, int)}

%exception
{
	try
	{
		$action
	}
	catch (std::exception &e)
	{
		PyErr_SetString(PyExc_Exception, const_cast<char*>(e.what()));
		SWIG_fail;
	}
}

// %include "ShaperPy.h"

class FilePy;
class BufferPy;
class ProcessPy;
class ProcessorPy;
class OperatorPy;
class DisplayPy;
class ShaperPy
{
public:
	typedef unsigned long FID;

	typedef unsigned long IID;

	typedef unsigned long PID;

	enum FileTypes
	{
		kTif = 1,
		kMrc,
		kSeq
	};

	enum ProcessorTypes
	{
		kMedian3x3 = 1,
		kMedian5x5,
		kGaussian3x3,
		kGaussian5x5,
		kMean3x3,
		kMean5x5,
		kMaximum3x3,
		kMaximum5x5,
		kMinimum3x3,
		kMinimum5x5,
		kLaplace3x3,
		kLaplace5x5,
		kHighPass3x3,
		kHighPass5x5,
		kLowPass3x3,
		kLowPass5x5,
		KSobel3x3,
		KSobel5x5,
		KBilateral,

		kHistogramEqualization,
		kIntensityNegative,
		kIntensityLog,
		kIntensityGamma,
		kIntensityLinear,
		kIntensityPiecewiseLinear,
		kIntensityContrastBrightness,
		kIntensityThreshold,

		kDilate3x3,
		kDilate5x5,
		kErode3x3,
		kErode5x5,
		kOpen3x3,
		kOpen5x5,
		kClose3x3,
		kClose5x5,
	};

	enum SpecialAngles
	{
		k90Degrees = 1,
		k180Degrees,
		k270Degrees
	};

	enum FlipModes
	{
		kHorizintal = 1,
		kVertical,
		kHorizontalVertical
	};

	ShaperPy();

	~ShaperPy();

	FilePy* file();

	BufferPy* buffer();

	ProcessPy* process();

	ProcessorPy* processor();

	OperatorPy* operate();

	DisplayPy* display();
};

%nodefaultctor;
class StreamPy
{
public:
	ShaperPy::IID first();

	ShaperPy::IID last();

	ShaperPy::IID index(unsigned long);
};

class FilePy
{
public:
	ShaperPy::FID open(std::string const&, ShaperPy::FileTypes);

	StreamPy *stream(ShaperPy::FID);

	void close(ShaperPy::FID);

	void save(ShaperPy::IID, std::string const&, ShaperPy::FileTypes);
};

class BufferPy
{
public:
	ShaperPy::IID acquireUint8(unsigned char const*, int, int);

	ShaperPy::IID acquireUint16(unsigned short const*, int, int);

	ShaperPy::IID acquireFloat32(float const*, int, int);

	void release(ShaperPy::IID);

	void release(const std::list<ShaperPy::FID> &);
};

class ProcessPy
{
public:
	ShaperPy::PID createList();

	void attach(ShaperPy::PID, ShaperPy::ProcessorTypes);

	void detach(ShaperPy::PID, ShaperPy::ProcessorTypes);

	bool has(ShaperPy::PID, ShaperPy::ProcessorTypes);

	ShaperPy::IID apply(ShaperPy::PID, ShaperPy::IID);

	ShaperPy::IID applyFilterMedian3x3(ShaperPy::IID);

	ShaperPy::IID applyFilterMedian5x5(ShaperPy::IID);

	ShaperPy::IID applyFilterGaussian3x3(ShaperPy::IID);

	ShaperPy::IID applyFilterGaussian5x5(ShaperPy::IID);

	ShaperPy::IID applyFilterMean3x3(ShaperPy::IID);

	ShaperPy::IID applyFilterMean5x5(ShaperPy::IID);

	ShaperPy::IID applyFilterMaximum3x3(ShaperPy::IID);

	ShaperPy::IID applyFilterMaximum5x5(ShaperPy::IID);

	ShaperPy::IID applyFilterMinimum3x3(ShaperPy::IID);

	ShaperPy::IID applyFilterMinimum5x5(ShaperPy::IID);

	ShaperPy::IID applyFilterLaplace3x3(ShaperPy::IID);

	ShaperPy::IID applyFilterLaplace5x5(ShaperPy::IID);

	ShaperPy::IID applyFilterHighPass3x3(ShaperPy::IID);

	ShaperPy::IID applyFilterHighPass5x5(ShaperPy::IID);

	ShaperPy::IID applyFilterLowPass3x3(ShaperPy::IID);

	ShaperPy::IID applyFilterLowPass5x5(ShaperPy::IID);

	ShaperPy::IID applyFilterSobel3x3(ShaperPy::IID);

	ShaperPy::IID applyFilterSobel5x5(ShaperPy::IID);

	ShaperPy::IID applyFilterBilateral(ShaperPy::IID);

	ShaperPy::IID applyIntensityHistogramEqual(ShaperPy::IID);

	ShaperPy::IID applyIntensityHistogramLocal(ShaperPy::IID);

	ShaperPy::IID applyIntensityNegative(ShaperPy::IID);

	ShaperPy::IID applyIntensityLog(ShaperPy::IID);

	ShaperPy::IID applyIntensityGamma(ShaperPy::IID);

	ShaperPy::IID applyIntensityLinear(ShaperPy::IID);

	ShaperPy::IID applyIntensityPiecewiseLinear(ShaperPy::IID);

	ShaperPy::IID applyIntensityBrightnessContrast(ShaperPy::IID);

	ShaperPy::IID applyIntensityThreshold(ShaperPy::IID);

	ShaperPy::IID applyMorphologyDilate3x3(ShaperPy::IID);

	ShaperPy::IID applyMorphologyDilate5x5(ShaperPy::IID);

	ShaperPy::IID applyMorphologyErode3x3(ShaperPy::IID);

	ShaperPy::IID applyMorphologyErode5x5(ShaperPy::IID);

	ShaperPy::IID applyMorphologyOpen3x3(ShaperPy::IID);

	ShaperPy::IID applyMorphologyOpen5x5(ShaperPy::IID);

	ShaperPy::IID applyMorphologyClose3x3(ShaperPy::IID);

	ShaperPy::IID applyMorphologyClose5x5(ShaperPy::IID);

	ShaperPy::IID applyAccumulation(std::list<ShaperPy::IID> const&);

	ShaperPy::IID applyAverage(std::list<ShaperPy::IID> const&);

	ShaperPy::IID applyTransformRotation(ShaperPy::IID);

	ShaperPy::IID applyTransformFlip(ShaperPy::IID);

	ShaperPy::IID applyFFT(ShaperPy::IID);
};

class FilterPy
{
public:
	void setGaussianSigma(float);

	void setBilateralRadius(int);

	void setBilateralSigmaColor(double);

	void setBilateralSigmaSpace(double);
};

class TransformPy
{
public:
	void setRotationSpecialAngle(ShaperPy::SpecialAngles);

	void setRotationArbitraryAngle(double);

	void setFlipMode(ShaperPy::FlipModes);
};


class IntensityPy
{
public:
	void setGammaValue(float);

	void setLogValue(double);

	void setLinearPoint(int, int, int, int);

	void setPiecewiseLinearPoint(int, int, int, int);

	void setContrastBrightness(double, double);
};

class ProcessorPy
{
public:
	FilterPy *filter();

	TransformPy *transform();

	IntensityPy *intensity();
};

class OperatorPy
{
public:
	ShaperPy::IID copy(ShaperPy::IID);

	ShaperPy::IID copyCrop(ShaperPy::IID, int, int, int, int);

	void copyROI(ShaperPy::IID, ShaperPy::IID, int, int, int, int);

	ShaperPy::IID add(ShaperPy::IID, ShaperPy::IID);

	void addI(ShaperPy::IID, ShaperPy::IID);

	ShaperPy::IID sub(ShaperPy::IID, ShaperPy::IID);

	void subI(ShaperPy::IID, ShaperPy::IID);

	ShaperPy::IID mul(ShaperPy::IID, ShaperPy::IID);

	void mulI(ShaperPy::IID, ShaperPy::IID);

	ShaperPy::IID div(ShaperPy::IID, ShaperPy::IID);

	void divI(ShaperPy::IID, ShaperPy::IID);

	ShaperPy::IID addConstant(ShaperPy::IID, float);

	void addConstantI(ShaperPy::IID, float);

	ShaperPy::IID subConstant(ShaperPy::IID, float);

	void subConstantI(ShaperPy::IID, float);

	ShaperPy::IID mulConstant(ShaperPy::IID, float);

	void mulConstantI(ShaperPy::IID, float);

	ShaperPy::IID divConstant(ShaperPy::IID, float);

	void divConstantI(ShaperPy::IID, float);
};

class DisplayPy
{
public:
	void show(ShaperPy::IID);
};

%clearnodefaultctor;
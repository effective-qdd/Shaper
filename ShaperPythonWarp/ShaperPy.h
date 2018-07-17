#ifndef _SHAPER_PY_H_
#define _SHAPER_PY_H_

#include <string>
#include <list>

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
		/// Filter
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
		KBilateral,

		/// Enhancement
		kHistogramEqualization, ///< Histogram equalization
		kHistogramLocal, ///< Histogram local
		kIntensityNegative, ///< Intensity negative
		kIntensityLog, ///< Intensity Log
		kIntensityGamma, ///< Intensity Gamma
		kIntensityLinear, ///< Intensity Linear
		kIntensityPiecewiseLinear, ///< Intensity Piecewise Linear
		kIntensityContrastBrightness, ///< Intensity Brightness and Contrast 
		kIntensityThreshold,

		/// Morphology
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

	struct Rect
	{
		int x;
		int y;
		int width;
		int height;
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

class StreamPy
{
public:
	StreamPy() = default;

	~StreamPy() = default;

	ShaperPy::IID first();

	ShaperPy::IID last();

	ShaperPy::IID index(unsigned long);
};

class FilePy
{
public:
	FilePy() = default;

	~FilePy() = default;

	ShaperPy::FID open(std::string const&, ShaperPy::FileTypes);

	StreamPy* stream(ShaperPy::FID);

	void close(ShaperPy::FID);

	void save(ShaperPy::IID, std::string const&, ShaperPy::FileTypes);
};

class BufferPy
{
public:
	BufferPy() = default;

	~BufferPy() = default;
	
	ShaperPy::IID acquireUint8(unsigned char const*, int, int);

	ShaperPy::IID acquireUint16(unsigned short const*, int, int);

	ShaperPy::IID acquireFloat32(float const*, int, int);

	void release(ShaperPy::IID);

	void release(const std::list<ShaperPy::IID> &);
};

class ProcessPy
{
public:
	ProcessPy() = default;

	~ProcessPy() = default;

	ShaperPy::PID createList();

	void attach(ShaperPy::PID, ShaperPy::ProcessorTypes);

	void detach(ShaperPy::PID, ShaperPy::ProcessorTypes);

	bool has(ShaperPy::PID, ShaperPy::ProcessorTypes);

	bool empty(ShaperPy::PID);

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
	FilterPy() = default;

	~FilterPy() = default;

	void setGaussianSigma(float);
};

class TransformPy
{
public:
	TransformPy() = default;

	~TransformPy() = default;

	void setRotationSpecialAngle(ShaperPy::SpecialAngles);

	void setRotationArbitraryAngle(double);

	void setFlipMode(ShaperPy::FlipModes);
};

class IntensityPy
{
public:
	IntensityPy() = default;

	~IntensityPy() = default;

	void setGammaValue(float);

	void setLogValue(double);

	void setLinearPoint(int, int, int, int);

	void setPiecewiseLinearPoint(int, int, int, int);

	void setContrastBrightness(double, double);
};

class ProcessorPy
{
public:
	ProcessorPy() = default;

	~ProcessorPy() = default;

	FilterPy *filter();

	TransformPy *transform();

	IntensityPy *intensity();
};

class OperatorPy
{
public:
	OperatorPy() = default;

	~OperatorPy() = default;

	ShaperPy::IID copy(ShaperPy::IID);

	ShaperPy::IID copyCrop(ShaperPy::IID, ShaperPy::Rect);

	void copyROI(ShaperPy::IID, ShaperPy::IID, ShaperPy::Rect);

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
	DisplayPy() = default;

	~DisplayPy() = default;

	void show(ShaperPy::IID);
};
#endif
#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING

#include <iostream>
#include <windows.h>
#include <thread>
#include <mutex> 
#include <condition_variable>
#include <list>

#include <tbb/tbb.h>
#include "tbb/pipeline.h"

#include "Shaper.h"

#include "../../Elder/Timer/Timer.hpp"
#include "../../Elder/Logger/Logger.hpp"

typedef std::shared_ptr<SHAPER::CShaper> (*ShaperInstanceFunc)();

unsigned short *buf = nullptr;
std::list<SHAPER::IID> imageList;
SHAPER::IID imageID;
std::condition_variable imageCond;
std::mutex mainMutex;

void ThreadMain(SHAPER::CBuffer *buffer, SHAPER::COperator *oper)
{
	auto index = 0;
	auto imageId = imageList.front();
	while (index < 100)
	{
		ELDER::CTimer timer;
		timer.Start();
		auto id = buffer->Acquire(buf, ELDER::ImageInfo({ 4096, 4096 }, 16));
		oper->AddI(imageId, id);
		buffer->Release(id);
		++index;
		std::cout << "Timer = " << timer.ElapsedMsecs() << std::endl;
		while(timer.ElapsedMsecs() < 10)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));	
		if (index % 50 == 0)
		{
			imageList.pop_front();
			imageList.push_back(imageId);
			imageId = imageList.front();

		}
	}
}

void ThreadA(SHAPER::CBuffer *buffer, SHAPER::CProcess *process)
{
	std::mutex mutexA;
	std::list<SHAPER::IID> list;
	while (true)
	{
		std::unique_lock<std::mutex> lck(mutexA);
		//std::cout << "Thread A Waiting..." << std::endl;
		imageCond.wait(lck);
		std::cout << "Thread A Waited" << std::endl;
		{
			std::lock_guard<std::mutex> lckA(mainMutex);
			list = std::move(imageList);
		}
		
		std::cout << "list A size = " << list.size() << std::endl;
		auto accuId = process->ApplyAccumulation(list);
		buffer->Release(list);
		list.clear();
		buffer->Release(accuId);
	}

}

void ThreadB(SHAPER::CBuffer *buffer, SHAPER::CProcess *process)
{
	std::mutex mutexB;
	std::list<SHAPER::IID> list;
	while (true)
	{
		std::unique_lock<std::mutex> lck(mutexB);
		//std::cout << "Thread B Waiting..." << std::endl;
		imageCond.wait(lck);
		std::cout << "Thread B Waited" << std::endl;
		{
			std::lock_guard<std::mutex> lckB(mainMutex);
			list = std::move(imageList);
		}

		std::cout << "list B size = " << list.size() << std::endl;
		auto accuId = process->ApplyAccumulation(list);
		buffer->Release(list);
		list.clear();
		buffer->Release(accuId);
	}

}

class MyInputFilter : public tbb::filter
{
public:
	MyInputFilter(SHAPER::CFile* shaperFile)
		:tbb::filter(serial_in_order)
		, m_value(0)
	{
		auto fid1 = shaperFile->Open("D:/Data/TestImages/lena.tif");
		// 		//auto fid1 = file->Open("D:/Data/t2.tif");
		//auto info = shaperFile->Info(fid1);
		m_shaperStream = shaperFile->Stream(fid1);
		//auto iid = m_shaperStream->First();
	}

	~MyInputFilter() = default;

	void* operator()(void*)
	{
		if (m_value > 10) return nullptr;

		m_iid = new SHAPER::IID;
		*m_iid = m_shaperStream->First();

		auto& iid = m_iid;
		//std::cout << "MyInputFilter => i = " << *iid << "     thread id = " << std::this_thread::get_id() << std::endl;
		//std::this_thread::sleep_for(std::chrono::microseconds(50));
		++m_value;
		return iid;
	}

private:
	int* m_inputList;
	int m_value;
	SHAPER::CStream* m_shaperStream;
	SHAPER::IID* m_iid;
};

class MyTransformFilter : public tbb::filter
{
public:
	MyTransformFilter(SHAPER::CProcess* shaperProcess, SHAPER::CBuffer* shaperBuffer)
		: tbb::filter(parallel)
		, m_shaperProcess(shaperProcess)
		, m_shaperBuffer(shaperBuffer)
	{
		m_pid = m_shaperProcess->CreateList();
		m_shaperProcess->Attach(m_pid, SHAPER::ProcessorTypes::kGaussian3x3);
		m_shaperProcess->Attach(m_pid, SHAPER::ProcessorTypes::kMean3x3);
		m_shaperProcess->Attach(m_pid, SHAPER::ProcessorTypes::kLaplace3x3);
	}

	void* operator()(void* item)
	{
		auto iid = static_cast<SHAPER::IID*>(item);
		auto processedIid = m_shaperProcess->ApplyMultiCore(m_pid, *iid);
		m_shaperBuffer->Release(*iid);
		*iid = processedIid;
		//*list = *list * 10;
		std::cout << "MyTransformFilter => i = " << *iid << "     thread id = " << std::this_thread::get_id() << std::endl;
		//std::this_thread::sleep_for(std::chrono::microseconds(50));


		return iid;
	}

private:
	std::mutex m_mutex;
	SHAPER::CProcess* m_shaperProcess;
	SHAPER::CBuffer* m_shaperBuffer;
	SHAPER::PID m_pid;
};

class MyOutputFilter : public tbb::filter
{
public:
	MyOutputFilter(SHAPER::CBuffer* shaperBuffer, SHAPER::CFile* shaperFile)
		: tbb::filter(serial_in_order)
		, m_shaperBuffer(shaperBuffer)
		, m_shaperFile(shaperFile)
	{

	}

	void* operator()(void* item)
	{
		auto iid = static_cast<SHAPER::IID*>(item);
		m_outList.push_back(*iid);
		auto path = "D:/" + std::to_string(long(*iid)) + ".tif";
		m_shaperFile->Save(*iid, path, SHAPER::FileTypes::kTif);
		m_shaperBuffer->Release(*iid);
		//std::cout << "MyOutputFilter => i = " << *list << "     thread id = " << std::this_thread::get_id() << std::endl;
		delete iid;

		return nullptr;
	}

	void PrintOutList()
	{
		for (auto const& out : m_outList)
		{
			std::cout << out << '\t';
		}
		std::cout << std::endl;
	}

private:
	std::list<SHAPER::IID> m_outList;
	SHAPER::CBuffer* m_shaperBuffer;
	SHAPER::CFile* m_shaperFile;
};

int RunPipeline(SHAPER::CFile* shaperFile, SHAPER::CProcess* shaperProcess, SHAPER::CBuffer* shaperBuffer)
{
	// Create the pipeline
	tbb::pipeline pipeline;

	// Create file-reading writing stage and add it to the pipeline
	MyInputFilter input_filter(shaperFile);
	pipeline.add_filter(input_filter);

	// Create squaring stage and add it to the pipeline
	MyTransformFilter transform_filter(shaperProcess, shaperBuffer);
	pipeline.add_filter(transform_filter);

	// Create file-writing stage and add it to the pipeline
	MyOutputFilter output_filter(shaperBuffer, shaperFile);
	pipeline.add_filter(output_filter);

	// Run the pipeline
	tbb::tick_count t0 = tbb::tick_count::now();
	// Need more than one token in flight per thread to keep all threads 
	// busy; 2-4 works
	pipeline.run(8);
	tbb::tick_count t1 = tbb::tick_count::now();

	std::cout << "end" << std::endl;

	output_filter.PrintOutList();

	return 1;
}

int main()
{
	HINSTANCE shaperLib = LoadLibrary("Shaper.dll");
	if (!shaperLib) std::cout << "Not Find this Dll" << std::endl;
	auto shaperInstFunc = (ShaperInstanceFunc)GetProcAddress(shaperLib, "ShaperInstance");
	if(!shaperInstFunc) std::cout << "Not Find this func" << std::endl;
	auto shaper = shaperInstFunc();
	shaper->Initialize(false, 8);
	//auto core = shaper->Core();
	auto file = shaper->File();
	auto buffer = shaper->Buffer();
	auto process = shaper->Process();
	auto processor = shaper->Processor();
	auto operate = shaper->Operator();
	auto display = shaper->Display();


	/*buf = new unsigned short[4096 * 4096];
	for (auto i = 0; i < 4096 * 4096; ++i)
	{
		buf[i] = 10;
	}*/

	/*imageID = buffer->Acquire(ELDER::ImageInfo({ 4096, 4096 }, 16));
	imageList.push_back(buffer->Acquire(ELDER::ImageInfo({ 4096, 4096 }, 16)));
	imageList.push_back(buffer->Acquire(ELDER::ImageInfo({ 4096, 4096 }, 16)));*/
	//std::thread threadA(ThreadA, buffer, process);
	//std::thread threadB(ThreadB, buffer, process);
	//std::thread threadMain(ThreadMain, buffer, operate);
	//

	//threadMain.join();

	//auto accuId = process->ApplyAccumulation(imageList);
	//operate->DivConstantI(accuId, 100);
	//display->Show(accuId);
	/*for (int i = 0; i < 50; ++i)
	{
		ELDER::CTimer timer;
		timer.Start();
		auto id = buffer->Acquire32f(buf, ELDER::ImageInfo({ 4096, 4096 }, 16));
		std::cout << "Timer = " <<  timer.ElapsedMsecs() << std::endl;
		buffer->Release(id);
	}*/


// 	auto id1 = buffer->Acquire(ELDER::ImageInfo({ 2048, 1536 }, 32));	
// 	auto id2 = buffer->Acquire(ELDER::ImageInfo({ 2048, 1536 }, 32));
// 	auto id3 = buffer->Acquire(ELDER::ImageInfo({ 2048, 1536 }, 32));
// 	buffer->Release(id1);
// 	buffer->Release(id2);
// 
// 	auto id4 = buffer->Acquire(ELDER::ImageInfo({ 2048, 1536 }, 32));
// 	auto id5 = buffer->Acquire(ELDER::ImageInfo({ 2048, 1536 }, 32));
// 	auto id6 = buffer->Acquire(ELDER::ImageInfo({ 2048, 1536 }, 32));
	try
	{
		
		//auto iid0 = buffer->Acquire(ELDER::ImageInfo({ 8192, 8192 }, 16));
		//auto iid1 = buffer->Acquire(ELDER::ImageInfo({ 8192, 8192 }, 16));
// 		unsigned short* input = new unsigned short[8192 * 8192];
// 		memset(input, 0, 8192 * 8192 * 2);
// 		ELDER::CTimer timer;
// 		for (auto i = 0; i < 5; ++i)
// 		{
// 			timer.Start();
// 			auto iid2 = buffer->AcquireByTBB(input, ELDER::ImageInfo({ 8192, 8192 }, 16));
// 			std::cout << "Timer = " << timer.ElapsedMsecs() << std::endl;
// 		}
		
// 		auto pid = process->CreateList();
// 		process->Attach(pid, SHAPER::ProcessorTypes::kGaussian3x3);
 		//process->ApplyMultiCore(pid, iid1);
		//auto iid2 = operate->Copy(iid);
		

		auto fid1 = file->Open("D:/Data/TestImages/t4.tif");
// 		//auto fid1 = file->Open("D:/Data/t2.tif");
//		auto info = file->Info(fid1);
 		auto stream1 = file->Stream(fid1);
 		auto iid1 = stream1->First();

		auto fid2 = file->Open("D:/Data/TestImages/t2.tif");
		auto stream2 = file->Stream(fid2);
		auto iid2 = stream2->First();
// 		auto iidInfo = buffer->Info(iid);
//		process->ApplyMultiCore(pid, iid);


		auto dataBuf = buffer->Data8u(iid1);
		auto corr = processor->Correction();
		corr->SetTemplateReference(dataBuf);
		auto iid = process->ApplyCorrectionCrossCorrelationNormal(iid2);

		//auto iid1 = process->ApplyCorrectionDarkRef(iid);

		display->Show(iid);
		//RunPipeline(file, process, buffer);
		
		
// 		auto intensity = processor->Intensity();
// 		intensity->SetLogValue(0.1);
// 		auto iid1 = process->ApplyFilterGaussian3x3(iid);
// 		display->Show(iid1);

// 		auto iid2 = operate->Sub(iid1, iid);
// 		display->Show(iid2);
		
		//auto data = buffer->Revert(iid);
		
// 		std::list<SHAPER::IID> iidList = { iid, iid1 };
// 		auto iid2 = process->ApplyAccumulation(iidList);
// 		display->Show(iid2);
// 		auto tranfform = processor->Transform();
// 		tranfform->SetMode(SHAPER::FlipModes::kHorizontalVertical);
		//auto rotation = processor->Rotation();
		//rotation->SetRotationSpecialAngle(SHAPER::SpecialAngles::k270Degrees);
		//auto iid2 = process->ApplyHistogramEqualization(iid);
		//file->Save(iid1, "D:/iid1.tif", SHAPER::FileTypes::kTif);
		//display->Show(iid2);
		//auto iid3 = process->ApplyHistogramLocal(iid);
		//display->Show(iid3);

// 		auto iid3 = process->ApplyHistogramLocal(iid);
// 		display->Show(iid3);

// 		auto iid4 = operate->Sub(iid3, iid);
// 		display->Show(iid4);

	}
	catch(std::exception &e)
	{
		//LOG_ERROR(e.what());
		std::cout << e.what() << std::endl;
	}

	//file->Save(iid, "D:/iid.tif", SHAPER::FileTypes::kTif);
	//auto iid2 = process->ApplyFFT(iid);
	//display->Show(iid2);
	//file->Save(iid2, "D:/iid2.tif", SHAPER::FileTypes::kTif);
// 	auto iid1 = stream1->Index(1);
// 	std::list<SHAPER::IID> iidlist;
// 	iidlist.push_back(iid);
// 	iidlist.push_back(iid1);
// 	auto iid2 = process->ApplyEnhancement(iid, SHAPER::kEnhancementContrastBrightness);
// 	display->Show(iid);
// 	display->Show(iid1);
// 	display->Show(iid2);
// 	display->Show(iid1);
// 	auto pid = process->CreateList();
// 	process->Attach(pid, SHAPER::kRotationSepcialAngle);
	//process->Attach(pid, BJI::SHAPER::kEnhancementIntensityPiecewiseLinear);
	//auto enhance = processor->Enhancement();
	//enhance->SetContrastBrightness(1, 0);
	//enhance->SetGammaValue(0.2f);
	//enhance->SetLinearPoint(100, 120, 100, 220);
	//enhance->SetPiecewiseLinearPoint(100, 200, 180, 150);
	//process->Attach(pid, BJI::SHAPER::kEnhancementIntNeg);
// 	auto iid2 = process->Apply(pid, iid1);
// 	display->Show(iid2);

// 	process->Detach(pid, BJI::SHAPER::kFlip);
// 	auto iid3 = process->Apply(pid, iid1);
// 	display->Show(iid3);
	//process->Attach(pid, BJI::SHAPER::kFilterGaussian3x3);
	//auto pdImageID1 = process->Apply(pid, iid1);
	//file->Save(pdImageID1, "D:/t3_median3x3.tif", BJI::SHAPER::kTif);
	//display->Show(pdImageID1);

// 	process->Detach(pid, BJI::SHAPER::kFilterGaussian3x3);
// 	process->Attach(pid, BJI::SHAPER::kFilterGaussian5x5);
// 	auto pdImageID2 = process->Apply(pid, iid1);
// 	file->Save(pdImageID2, "D:/t3_median5x5.tif", BJI::SHAPER::kTif);
	//display->Show(pdImageID2);
//	auto d = buffer->Revert(fid1);
// 	auto filter = processor->Filter();
// 	filter->SetMedianBorderType(BJI::SHAPER::kRepl);
// 	auto fid_median = process->ApplyFilter(fid1, BJI::SHAPER::kMedian3x3);
//	display->Show(fid_median);

	//auto fid_ratation1 = process->ApplyRotation(fid1, BJI::SHAPER::kSepcialAngle);
	//display->Show(fid_ratation1);
	

//  	auto stream2 = file->Open("D:/t2.tif", BJI::SHAPER::kTif);
// 	auto fid2 = stream2->First();
// 	display->Show(fid2);
// 	auto pid1 = process->CreateList();
// 	process->Attach(pid1, BJI::SHAPER::kFilterMedian3x3);
// 	process->Attach(pid1, BJI::SHAPER::kRotationSepcialAngle);
// 	auto fid3 = process->Apply(pid1, fid2);
// 	display->Show(fid3);
	//auto fid_ratation2 = process->ApplyRotation(fid2, BJI::SHAPER::kRotationSepcialAngle);
	//file->Save(fid_ratation2, "D:/t2_save.tif", BJI::SHAPER::kTif);
	//display->Show(fid_ratation2);
// 	process->ApplyFilter(frame2, BJI::SHAPER::FT_MEDIAN_3x3);
//	display->Show(frame2);
	
// 	unsigned char *buf = new unsigned char[512 * 512];
// 	memset(buf, 100, 512 * 512);
// 	auto fid2 = buffer->Acquire32f(buf, {ELDER::Size(512, 512), 8});
	
// 
// 	ELDER::ImageInfo info1;
// 	info1.size.width = 1024;
// 	info1.size.height = 1024;
// 	info1.bitDepth = 32;
// 	info1.channel = 1;
// 	auto iid1 = buffer->Acquire(info1);
// 
// 	ELDER::ImageInfo info2;
// 	info2.size.width = 1024;
// 	info2.size.height = 1024;
// 	info2.bitDepth = 16;
// 	info2.channel = 1;
// 	unsigned short *buf2 = new unsigned short[1024 * 1024];
// 	memset(buf2, 200, 1024 * 1024 * 2);
//  	auto iid2 = buffer->Acquire32f(buf2, { ELDER::Size(1024, 1024), 16 });
// 
// 	unsigned short *buf3 = new unsigned short[1024 * 1024];
// 	memset(buf3, 200, 1024 * 1024 * 2);
// 	auto iid3 = buffer->Acquire32f(buf3, { ELDER::Size(1024, 1024), 16 })
// 	operate->AddI(iid2, iid3);
//  	display->Show(iid2);
// 
// 	float *buf3 = new float[1024 * 1024];
// 	memset(buf3, 255, 1024 * 1024 * 4);
// 	auto fid3 = buffer->Acquire(buf3, { BJI::Size(1024, 1024), 32 });
// 	display->Show(fid3);

// 	unsigned char *buf4 = new unsigned char[1024 * 1024];
// 	memset(buf4, 100, 1024 * 1024);
// 	auto fid4 = buffer->Acquire(buf4, { BJI::Size(1024, 1024), 8 });

// 

// 	auto pid1 = process->Create();
// 	process->AttachFilter(pid1, BJI::SHAPER::FT_MEDIAN_3x3);
// 	core->BindBuffer(iid);
// 	core->BindProcess(pid1);
// 	auto fidVect = core->Apply();
	//display->Show(iid);
// 	file->Save(bidVect[0], "D:/t1-median.tif", BJI::SHAPER::FT_TIF);
// 
// 	auto pid2 = process->Create();
// 	process->AttachFilter(pid2, BJI::SHAPER::FT_GAUSSIAN_3x3);
// 	core->BindProcess(pid2);
// 	auto bidVect2 = core->Apply();
// 	file->Save(bidVect2[0], "D:/t1-gaussian.tif", BJI::SHAPER::FT_TIF);
	

// 	HINSTANCE shaperDisplayLib = LoadLibrary("ShaperDisplay.dll");
// 	auto displayInstFunc = (DisplayInstanceFunc)GetProcAddress(shaperDisplayLib, "DisplayInstance");
// 	auto superDisplay = displayInstFunc(nullptr);
// 	superDisplay->Initialize();
// 	superDisplay->Show(iid);

	getchar();
	return 0;
}
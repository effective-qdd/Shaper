#include "pch.h"

#include <windows.h>
#include <iostream>
#include <thread>
#include <mutex> 

#include "Shaper.h"
#include "../../Elder/Timer/Timer.hpp"

typedef std::shared_ptr<SHAPER::CShaper>(*ShaperInstanceFunc)();

static SHAPER::CFile* shaperFile = nullptr;
static SHAPER::CBuffer* shaperBuffer = nullptr;
static SHAPER::CProcess* shaperProcess = nullptr;
static SHAPER::CProcessor* shaperProcessor = nullptr;
static SHAPER::COperator* shaperOperator = nullptr;
static SHAPER::CDisplay* shaperDisplay = nullptr;

TEST(TestShaper, Load)
{
	HINSTANCE shaperLib = LoadLibrary("Shaper.dll");
	EXPECT_NE(shaperLib, nullptr);
	auto shaperInstFunc = (ShaperInstanceFunc)GetProcAddress(shaperLib, "ShaperInstance");
	EXPECT_NE(shaperInstFunc, nullptr);
	auto shaper = shaperInstFunc();
	shaper->Initialize();
	shaperFile = shaper->File();
	EXPECT_NE(shaperFile, nullptr);
	shaperBuffer = shaper->Buffer();
	EXPECT_NE(shaperBuffer, nullptr);
	shaperProcess = shaper->Process();
	EXPECT_NE(shaperProcess, nullptr);
	shaperProcessor = shaper->Processor();
	EXPECT_NE(shaperProcessor, nullptr);
	shaperOperator = shaper->Operator();
	EXPECT_NE(shaperOperator, nullptr);
	shaperDisplay = shaper->Display();
	EXPECT_NE(shaperDisplay, nullptr);
}


unsigned short *buf = nullptr;
std::list<SHAPER::IID> imageList;
SHAPER::IID imageID;
std::mutex mainMutex;

void ThreadMain(/*SHAPER::CBuffer *buffer, SHAPER::COperator *oper*/)
{
	auto index = 0;
	while (index < 500)
	{
		ELDER::CTimer timer;
		timer.Start();
		auto id = shaperBuffer->Acquire(buf, ELDER::ImageInfo({ 4096, 4096 }, 16));
		shaperOperator->AddI(imageID, id);
		shaperBuffer->Release(id);
		++index;
		std::cout << "Timer = " << timer.ElapsedMsecs() << std::endl;
// 		while (timer.ElapsedMsecs() < 10)
// 			std::this_thread::sleep_for(std::chrono::milliseconds(1));
// 		if (index % 50 == 0)
// 		{
// 			imageList.pop_front();
// 			imageList.push_back(imageId);
// 			imageId = imageList.front();
// 
// 		}
	}
}

TEST(TestShaperBuffer, Acquire)
{
	buf = new unsigned short[4096 * 4096];
	for (auto i = 0; i < 4096 * 4096; ++i)
	{
		buf[i] = 10;
	}

	imageID = shaperBuffer->Acquire(ELDER::ImageInfo({ 4096, 4096 }, 16));
	std::thread threadMain(ThreadMain);
	threadMain.join();
}
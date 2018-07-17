#ifndef SHAPER_DISPLAY_IMP_H
#define SHAPER_DISPLAY_IMP_H

#include <string>
#include <list>
#include <any>

#include <glad/glad.h>

#include "../../Elder/Image/Image.hpp"

#include "SuperDisplay.h"

struct GLFWwindow;

namespace SHAPER
{
	class CBufferCore;

	class CDisplayImp final : public CSuperDisplay
	{
		struct Texture
		{
			int width = 0;
			int height = 0;
			unsigned char *data = nullptr;	
			bool needFree = false;
		};

	public:
		const int MajorVersion = 3;
		const int MinorVersion = 3;

		CDisplayImp(CBufferCore *);

		~CDisplayImp();

		void Show(IID);

	private:
		GLuint LoadVertexShader(std::string const&);

		GLuint LoadFragmentShader(std::string const&);

		bool CheckShader(GLuint);

		bool CheckProgram(GLuint);

		ELDER::Size WindowSize(int, int);

		Texture ConvertTextureImage(std::any&&);

		void RenderThread(std::any&&);

	private:
		CBufferCore* m_bufferCore;
		std::list<std::shared_ptr<std::thread>> m_renderThreadList;
		std::mutex m_renderThreadMutex;
	};
}

#endif

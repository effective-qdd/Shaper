#include "ShaperDisplayImp.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <any>

#include <GLFW/glfw3.h>

#include "../../Elder/Logger/Logger.hpp"
#include "../../Elder/ImageOperator/ImageMax.hpp"
#include "../../Elder/ImageOperator/ImageMin.hpp"
#include "../../Elder/ImageOperator/ImageSubConstant.hpp"
#include "../../Elder/ImageOperator/ImageMulConstant.hpp"
#include "../../Elder/ImageOperator/ImageConvert.hpp"
#include "../../Elder/ImageOperator/ImageSetConstant.hpp"

#include "BufferCore.h"

namespace SHAPER
{
	const GLuint WINDOW_WEIGHT_DEFAULT = 800;
	const GLuint WINDOW_HEIGHT_DEFAULT = 600;

	GLfloat Vertices[] =
	{
		1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 0.0f
	};

	const unsigned int Indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	void ProcessInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}


	CDisplayImp::CDisplayImp(CBufferCore* bufferCore)
		: CSuperDisplay()
		, m_bufferCore(bufferCore)
	{
		glfwInit();
	}


	CDisplayImp::~CDisplayImp()
	{
		for (auto renderThread : m_renderThreadList)
		{
			if (renderThread)
			{
				renderThread->join();
				renderThread = nullptr;
			}
		}
		m_renderThreadList.swap(std::list<std::shared_ptr<std::thread>>());
	}

	void CDisplayImp::Show(IID iid)
	{
		m_renderThreadList.push_back(std::make_shared<std::thread>(&CDisplayImp::RenderThread, this, m_bufferCore->Resume(iid)));
	}
		
	//////////////////////////////////////////////////////////////////////////
	GLuint CDisplayImp::LoadVertexShader(std::string const& vertexFilePath)
	{
		// Read the Vertex Shader code from the file
		std::ifstream vertexShaderStream(vertexFilePath, std::ios::in);
		ENSURE_THROW_MSG(vertexShaderStream.is_open(), "Open vertex shader file failed");
		vertexShaderStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		std::string vertexShaderCode;
		try
		{
			std::stringstream sstr;
			sstr << vertexShaderStream.rdbuf();
			vertexShaderCode = sstr.str();
			vertexShaderStream.close();
		}
		catch (std::ifstream::failure)
		{
			THROW_MSG("Read vertex shader file failed");
		}

		// Create the shader
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

		// Compile Vertex Shader
		auto vertexSourcePointer = vertexShaderCode.c_str();
		glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
		glCompileShader(vertexShaderID);

		return vertexShaderID;
	}

	GLuint CDisplayImp::LoadFragmentShader(std::string const& fragmentFilePath)
	{
		// Read the Fragment Shader code from the file
		std::ifstream fragmentShaderStream(fragmentFilePath, std::ios::in);
		ENSURE_THROW_MSG(fragmentShaderStream.is_open(), "Open fragment shader file failed");
		fragmentShaderStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		std::string fragmentShaderCode;
		try
		{
			std::stringstream sstr;
			sstr << fragmentShaderStream.rdbuf();
			fragmentShaderCode = sstr.str();
			fragmentShaderStream.close();
		}
		catch (std::ifstream::failure)
		{
			THROW_MSG("Read fragment shader file failed");
		}

		// Create the shader
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Compile Fragment Shader
		auto fragmentSourcePointer = fragmentShaderCode.c_str();
		glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
		glCompileShader(fragmentShaderID);

		return fragmentShaderID;
	}

	bool CDisplayImp::CheckShader(GLuint shaderID)
	{
		GLint result = GL_FALSE;
		int infoLogLength;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if ((result == GL_FALSE) && (infoLogLength > 0))
		{
			std::vector<char> shaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
			std::stringstream sstr;
			sstr << "ERROR::SHADER::COMPILATION_FAILED\n" << "Shader ID: " << shaderID << " Error: " << &shaderErrorMessage[0] << std::endl;
			LOG_ERROR(sstr.str());
			return false;
		}

		return true;
	}

	bool CDisplayImp::CheckProgram(GLuint programID)
	{
		GLint result = GL_FALSE;
		int infoLogLength;
		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if ((result == GL_FALSE) && (infoLogLength > 0))
		{
			std::vector<char> programErrorMessage(infoLogLength + 1);
			glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
			std::stringstream sstr;
			sstr << "ERROR::PROGRAM::LINKING_FAILED\n" << "Shader ID: " << programID << " Error: " << &programErrorMessage[0] << std::endl;
			LOG_ERROR(sstr.str());
			return false;
		}

		return true;
	}

	ELDER::Size CDisplayImp::WindowSize(int width, int height)
	{
		auto monitor = glfwGetPrimaryMonitor();
		auto vidMode = glfwGetVideoMode(monitor);
		auto screenSizeX = vidMode->width;
		auto screenSizeY = vidMode->height;
		auto maxWindowSizeX = static_cast<int>(screenSizeX * 0.7)/*screenSizeX > 2048 ? 2048 : screenSizeX*/;
		auto maxWindowSizeY = static_cast<int>(screenSizeY * 0.7)/*screenSizeY > 1024 ? 1024 : screenSizeY*/;
		ELDER::Size windowSize(width, height);

		if (width > maxWindowSizeX && height < maxWindowSizeY)
		{
			windowSize.width = maxWindowSizeX;
			windowSize.height = static_cast<int>(height * (maxWindowSizeX * 1.0f / width));
		}
		else if (width < maxWindowSizeX && height > maxWindowSizeY)
		{
			windowSize.width = static_cast<int>(width * (maxWindowSizeY * 1.0f / height));
			windowSize.height = maxWindowSizeY;
		}
		else if (width > maxWindowSizeX && height > maxWindowSizeY)
		{
			auto ratioX = maxWindowSizeX * 1.0f / width;
			auto ratioY = maxWindowSizeY * 1.0f / height;
			auto ratio = ratioX < ratioY ? ratioX : ratioY;
			windowSize.width = static_cast<int>(width * ratio);
			windowSize.height = static_cast<int>(height * ratio);
		}
		return windowSize;
	}

	CDisplayImp::Texture CDisplayImp::ConvertTextureImage(std::any&& buf)
	{
		
		std::shared_ptr<ELDER::CImage8u1cIPPI> image = nullptr;
		if (buf.type() == typeid(std::shared_ptr<ELDER::CImage8u1cIPPI>))
		{
			image = std::any_cast<std::shared_ptr<ELDER::CImage8u1cIPPI>>(buf);
		}
		else 
		{
			auto value = 0.0f;
			auto max = 0.0f;
			auto min = 0.0f;
			auto tmpImage32f1c = std::make_shared<ELDER::CImage32f1cIPPI>();
			image = std::make_shared<ELDER::CImage8u1cIPPI>();
			if (buf.type() == typeid(std::shared_ptr<ELDER::CImage16u1cIPPI>))
			{
				auto preRenderImage = std::move(std::any_cast<std::shared_ptr<ELDER::CImage16u1cIPPI>>(buf));
				tmpImage32f1c->Initialize(preRenderImage->Width(), preRenderImage->Height());
				image->Initialize(preRenderImage->Width(), preRenderImage->Height());
				ELDER::OPERATOR::CImageConvert<ELDER::OPERATOR::CConvert16u1cTo32f1c>::Convert(preRenderImage, tmpImage32f1c);
				max = ELDER::OPERATOR::CImageMax<ELDER::OPERATOR::CMax32f1c>::Max(tmpImage32f1c);
				min = ELDER::OPERATOR::CImageMin<ELDER::OPERATOR::CMin32f1c>::Min(tmpImage32f1c);
				value = max - min;
				ELDER::OPERATOR::CImageSubConstant<ELDER::OPERATOR::CSubConstant32f1c>::SubConstant(tmpImage32f1c, min);
			}
			else if (buf.type() == typeid(std::shared_ptr<ELDER::CImage32f1cIPPI>))
			{
				auto preRenderImage = std::move(std::any_cast<std::shared_ptr<ELDER::CImage32f1cIPPI>>(buf));
				tmpImage32f1c->Initialize(preRenderImage->Width(), preRenderImage->Height());
				image->Initialize(preRenderImage->Width(), preRenderImage->Height());
				max = ELDER::OPERATOR::CImageMax<ELDER::OPERATOR::CMax32f1c>::Max(preRenderImage);
				min = ELDER::OPERATOR::CImageMin<ELDER::OPERATOR::CMin32f1c>::Min(preRenderImage);
				value = max - min;
				ELDER::OPERATOR::CImageSubConstant<ELDER::OPERATOR::CSubConstant32f1c>::SubConstant(tmpImage32f1c, preRenderImage, min);
			}

			if (abs(value) > 0.00001)
			{
				auto scale = 255.0f / value;
				ELDER::OPERATOR::CImageMulConstant<ELDER::OPERATOR::CMulConstant32f1c>::MulConstant(tmpImage32f1c, scale);
				ELDER::OPERATOR::CImageConvert<ELDER::OPERATOR::CConvert32f1cTo8u1c>::Convert(tmpImage32f1c, image);
			}
			else
			{
				ELDER::OPERATOR::CImageSetConstant<ELDER::OPERATOR::CSetConstant8u1c>::SetConstant(image, static_cast<ELDER::IDT8U>(max) % 255);
			}
		}

		Texture texture;		
		texture.width = image->Width();
		texture.height = image->Height();
		//texture.data = new unsigned char[texture.width * texture.height];
		texture.data = ippsMalloc_8u(texture.width * texture.height);
		if (texture.data != nullptr)
		{
			auto status = ippiCopy_8u_C1R
			(
				image->Data(),
				image->WidthBytes(),
				texture.data,
				texture.width,
				{ texture.width, texture.height }
			);
			if (status != ippStsNoErr)
			{
				ippsFree(texture.data);
				texture.data = nullptr;
			}
			else
			{
				texture.needFree = true;
			}
		}
		
		return texture;
	}

	void CDisplayImp::RenderThread(std::any&& buf)
	{
		auto textureImage = ConvertTextureImage(std::forward<std::any>(buf));
		if (textureImage.data != nullptr)
		{
			try
			{
				std::unique_lock<std::mutex> lock(m_renderThreadMutex);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MajorVersion);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MinorVersion);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				glfwWindowHint(GLFW_FLOATING, GL_TRUE);

				auto textureWidth = textureImage.width;
				auto textureHeight = textureImage.height;
				auto windowSize = WindowSize(textureWidth, textureHeight);
				auto window = glfwCreateWindow(windowSize.width, windowSize.height, "ShaperDisplay", NULL, NULL);
				ENSURE_THROW_MSG(window != nullptr, "Failed to create GLFW window");

				glfwMakeContextCurrent(window);
				glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

				ENSURE_THROW_MSG(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != GL_FALSE, "Failed to initialize GLAD")

				// vertex shader
				auto vertexShaderID = LoadVertexShader("shader.vs");
				ENSURE_THROW_MSG(CheckShader(vertexShaderID), "Check vertex shader failed");

				// fragment shader
				auto fragmentShaderID = LoadFragmentShader("shader.fs");
				ENSURE_THROW_MSG(CheckShader(fragmentShaderID), "Check fragment shader failed");

				// link shaders
				GLuint programID = glCreateProgram();
				glAttachShader(programID, vertexShaderID);
				glAttachShader(programID, fragmentShaderID);
				glLinkProgram(programID);
				// check for linking errors
				ENSURE_THROW_MSG(CheckProgram(programID), "Check program failed");

				glDeleteShader(vertexShaderID);
				glDeleteShader(fragmentShaderID);

				//Create Vertex Array
				GLuint vertexArrayID = 0;
				glGenVertexArrays(1, &vertexArrayID);
				glBindVertexArray(vertexArrayID);

				//Create Vertex Buffer
				GLuint vertexBufferID = 0;
				glGenBuffers(1, &vertexBufferID);
				glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

				//Create Element Array Buffer
				GLuint indicesBufferID = 0;
				glGenBuffers(1, &indicesBufferID);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

				//Create Texture
				GLuint texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST/*GL_LINEAR*/);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.width, textureImage.height, 0, GL_RED, GL_UNSIGNED_BYTE, textureImage.data);
				glGenerateMipmap(GL_TEXTURE_2D);

				//Position attribute
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				//Color attribute
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);
				//Texture coord attribute
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
				glEnableVertexAttribArray(2);

				glUseProgram(programID);
				glBindVertexArray(vertexArrayID);

				lock.unlock();

				while (!glfwWindowShouldClose(window))
				{
					// input
					// -----
					ProcessInput(window);

					// render
					// ------
					glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

					// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
					// -------------------------------------------------------------------------------
					glfwSwapBuffers(window);
					glfwPollEvents();
				}
				//glfwSetWindowShouldClose(m_window, false);
				//glfwHideWindow(m_window);

				glDeleteVertexArrays(1, &vertexArrayID);
				glDeleteBuffers(1, &vertexBufferID);
				glDeleteBuffers(1, &indicesBufferID);
				glfwDestroyWindow(window);
			}
			catch (ELDER::CException &e)
			{
				LOG_ERROR(e.what());
			}

			if (textureImage.needFree && (textureImage.data != nullptr))
			{
				ippsFree(textureImage.data);
				textureImage.data = nullptr;
			}
		}		
	}
}
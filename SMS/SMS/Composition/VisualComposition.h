#pragma once

#include <Tools/MeanLeanWindows.h>
#include <Tools/FragmentShader.h>
#include <Tools/Queue.h>
#include <Tools/Memory.h>

namespace Discrepancy
{
	namespace Composition
	{
		struct VisualFrameContent
		{
			FragmentShader *Shader;
		};

		enum EVisualFrameType
		{
			FRAGMENT_SHADER = 0
		};

		//template<typename T>
		class VisualFrame
		{
		public:

			bool ManualEnd; // ignores EndTime and take a callback insted to set a bool
			bool HasEnded;

			float StartTime;
			float EndTime;
			void  *Content;
			EVisualFrameType FrameType;
			GLuint _frameBuffer;
			GLuint _renderTexture;
		public:

			void Render(unsigned int srcBuffer, unsigned int dstBuffer, float curTime)
			{
				switch (FrameType)
				{
				case EVisualFrameType::FRAGMENT_SHADER:
					ShaderParameters params;

					params.CurrentTime = curTime;
					params.DestinationTexture = dstBuffer;
					params.SourceTexture = srcBuffer;

					((FragmentShader *)Content)->Render(params, _renderTexture, _frameBuffer);
					break;
				default:
					break;
				}
			}

			VisualFrame() = delete;

			template <typename T>
			VisualFrame(T content, bool manualEnd, bool hasEnded, float startTime, float endTime, GLuint frameBuffer) :
				ManualEnd(manualEnd), HasEnded(hasEnded),
				StartTime(startTime), EndTime(endTime),
				Content(content),
				_frameBuffer(frameBuffer), _renderTexture(0U)
			{
				 

				glGenTextures(1, &_renderTexture);

				// "Bind" the newly created texture : all future texture functions will modify this texture
				glBindTexture(GL_TEXTURE_2D, _renderTexture);

				// Give an empty image to OpenGL ( the last "0" )
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

				// Poor filtering. Needed !
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



			}
		};

		class VisualChannel
		{
		public:
			Tools::Queue<VisualFrame *> Frames;
		};

		class VisualComposition
		{
			unsigned int _channelsCount;
			VisualChannel *_channelsContent;
			bool _locked;

		public:
			void Init(unsigned int channelsCount)
			{
				_channelsContent = (decltype(_channelsContent))Memory::HeapAlloc(channelsCount * sizeof(*_channelsContent));
				_channelsCount = channelsCount;
				_locked = false;
			}

			void Lock()
			{
				// Todo perform checks and reordering if needed
				_locked = false;
			}

			unsigned int GetChannelsCount()
			{
				if (!_locked)
					return _channelsCount;
				return 0;
			}

			VisualChannel *GetChannelsContent()
			{
				if (!_locked)
					return _channelsContent;
				return nullptr;
			}


			VisualComposition(unsigned int channelsCount) :
				_channelsCount(channelsCount),
				_channelsContent(nullptr),
				_locked(false)
			{
				Init(channelsCount);
			}

			~VisualComposition()
			{
				if (_channelsContent != nullptr)
					Memory::HeapFree(_channelsContent);
			}

			VisualComposition() = delete;

			void ExecuteComposition(float curTime)
			{
				DWORD lastFrame = GetTickCount();

				GLint myUniformLocation = glGetUniformLocation((GLuint)shaderIdx, "time");
				glUniform1f(myUniformLocation, accTime);

				GLint myResLocation = glGetUniformLocation((GLuint)shaderIdx, "resolution");
				glUniform2f(myResLocation, (float)startParams.Width, (float)startParams.Height);


				glRects(-1, -1, 1, 1);
				SwapBuffers(hDC);
				elapsedLastFrame = (float)(GetTickCount() - lastFrame) / 1000.0f;
				accTime += elapsedLastFrame;

				if (!_locked) // Lock need to be called before starting rendering
					return;
				for (int iChannel = 0; iChannel < 5; ++iChannel)
				{
					const auto &headPtr = _channelsContent[iChannel].Frames.Head();
					if (headPtr == nullptr)
						continue;

					const auto &headFrame = *headPtr;
					bool hasEnded = curTime > headFrame->EndTime;
					if (curTime > headFrame->StartTime && !hasEnded)
					{
						headFrame->Render(iChannel - 1, iChannel, curTime);
					}
					else if (hasEnded)
					{
						_channelsContent[iChannel].Frames.Pop();
					}

				}
			}
		};
	}
}
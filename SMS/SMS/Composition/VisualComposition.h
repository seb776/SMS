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
					((FragmentShader *)Content)->Render(params);
					break;
				default:
					break;
				}
			}

			VisualFrame() = delete;

			template <typename T>
			VisualFrame(T content, bool manualEnd, bool hasEnded, float startTime, float endTime) :
				ManualEnd(manualEnd), HasEnded(hasEnded),
				StartTime(startTime), EndTime(endTime),
				Content(content)
			{

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
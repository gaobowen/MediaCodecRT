#pragma once
#include <mutex>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};

#include "DllLoader.h"

using namespace Platform;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Media::Core;
namespace MediaCodecRT
{
    public ref class VideoFrames sealed
    {
    public:
		static VideoFrames^ VideoFrames::CreatVideoFrames(IRandomAccessStream^ stream, uint32_t outwidth, uint32_t outheight);
		Platform::Array<uint8_t>^ GetFrameBuffer(TimeSpan index);
		bool ConvertVideoFrames(IOutputStream^ outstream );
		void InitAutoIncrementFrameBuffer();
		void InitAutoIncrementFrameJpegBuffer();
		Platform::Array<uint8_t>^ GetAutoIncrementFrameBuffer();
		InMemoryRandomAccessStream^ GetAutoIncrementFrameJpegBuffer();

		virtual ~VideoFrames();
		/*
		视频原始宽
		*/
		property int Width
		{
			int get()
			{
				return m_width;
			};
		};
		/*
		视频原始高
		*/
		property int Height
		{
			int get()
			{
				return m_height;
			};
		};
		/*
		视频输出宽
		*/
		property int RenderWidth
		{
			int get()
			{
				return m_render_width;
			};
		};
		/*
		视频输出高
		*/
		property int RenderHeight
		{
			int get()
			{
				return m_render_height;
			};
		};
		property TimeSpan Duration
		{
			TimeSpan get()
			{
				return mediaDuration;
			};
		};
		property double FrameRate
		{
			double get()
			{
				return m_FrameRate;
			};
		};
		property double TestDoub
		{
			double get()
			{
				return m_TestDoub;
			};
		};
		property int64_t PtsSec
		{
			int64_t get()
			{
				return m_PtsSec;
			};
		};
		property int FrameIndex
		{
			int get()
			{
				return m_FrameIndex;
			};
		};
		 
		property String^ ErrorString
		{
			String^ get()
			{
				return m_errorString;
			};
		};
	private:
		VideoFrames::VideoFrames();
		HRESULT VideoFrames::MyCreatVideoFrames(IRandomAccessStream^ stream, uint32_t outwidth, uint32_t outheight);

		InMemoryRandomAccessStream^ MediaCodecRT::VideoFrames::SaveJpegStream(AVFrame * picture);

		bool isDisposing;
		int m_width;
		int m_height;
		int m_render_width;
		int m_render_height;
		int m_render_area;
		int m_render_area_byte;
		double m_FrameRate;
		int m_FrameIndex;
		int64_t m_PtsSec;
		double m_TestDoub;

		String ^ m_input_url;
		String ^ m_output_url;
		String ^ m_errorString;

		IStream* fileStreamData;
		IStream* outStreamData;
		unsigned char* fileStreamBuffer, *outStreamBuffer;
		

		TimeSpan mediaDuration;
		AVDictionary* avDict;
		std::recursive_mutex    m_mutexGuard;
		int32                            my_duration;
		const char*                            m_input_charStr;
		const char*                            m_output_charStr;

		AVIOContext* avIOCtx;
		AVFormatContext	    *pFormatCtx;
		int				                i, videoindex;
		AVCodecContext	        *pCodecCtx;
		AVCodec			            *pCodec, *pOutCodec;
		AVFrame	                    *pFrame, *pFrameYUV;
		unsigned char             *out_buffer;
		AVPacket                    *packet;



		//int                               y_size;
		//int                               ret, got_picture;
		struct SwsContext      *img_convert_ctx;

    };


}

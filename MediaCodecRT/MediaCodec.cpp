#include "pch.h"
#include "MediaCodec.h"
#include "shcore.h"

using namespace MediaCodecRT;
using namespace Platform;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Media::Core;

const int FILESTREAMBUFFERSZ = 16384;
static int FileStreamRead(void* ptr, uint8_t* buf, int bufSize);
static int64_t FileStreamSeek(void* ptr, int64_t pos, int whence);
static int FileStreamWrite(void* ptr, uint8_t* buf, int bufSize);
const char* ConvertToCStr(String^ str);
HRESULT ConvertCodecName(const char* codecName, String^ *outputCodecName);

static bool isRegistered = false;
static Platform::Array<uint8_t>^ m_JpegReturn;

MediaCodecRT::VideoFrames::VideoFrames()
	: avDict(nullptr)
	, avIOCtx(nullptr)
	, pCodecCtx(nullptr)
	, fileStreamData(nullptr)
	, fileStreamBuffer(nullptr)
	, outStreamData(nullptr)
{
	if (!isRegistered)
	{
		m_av_register_all();
		//my_static_av_register_all();
	}
}

HRESULT MediaCodecRT::VideoFrames::MyCreatVideoFrames(IRandomAccessStream ^ stream, uint32_t outwidth, uint32_t outheight)
{
	HRESULT hr = S_OK;
	if (!stream)
	{
		hr = E_INVALIDARG;
	}
	if (SUCCEEDED(hr))
	{
		hr = CreateStreamOverRandomAccessStream(reinterpret_cast<IUnknown*>(stream), IID_PPV_ARGS(&fileStreamData));
	}
	if (SUCCEEDED(hr))
	{
		fileStreamBuffer = (unsigned char*)m_av_malloc(FILESTREAMBUFFERSZ);
		if (fileStreamBuffer == nullptr)
		{
			hr = E_OUTOFMEMORY;
		}
	}

	if (SUCCEEDED(hr))
	{
		avIOCtx = m_avio_alloc_context(fileStreamBuffer, FILESTREAMBUFFERSZ, 0, fileStreamData, FileStreamRead, FileStreamWrite, FileStreamSeek);
		if (avIOCtx == nullptr)
		{
			hr = E_OUTOFMEMORY;
			m_errorString = "m_avio_alloc_context";
			return hr;
		}
	}

	if (SUCCEEDED(hr))
	{
		pFormatCtx = m_avformat_alloc_context();
		if (pFormatCtx == nullptr)
		{
			hr = E_OUTOFMEMORY;
			m_errorString = "m_avformat_alloc_context";
			return hr;
		}
	}

	if (SUCCEEDED(hr))
	{
		pFormatCtx->pb = avIOCtx;
		pFormatCtx->flags |= AVFMT_FLAG_CUSTOM_IO;

		if (m_avformat_open_input(&pFormatCtx, "", NULL, &avDict) < 0)
		{
			m_errorString = "m_avformat_open_input";
			hr = E_FAIL;
			return hr;
		}
		if (avDict != nullptr)
		{
			m_errorString = "Invalid FFmpeg option(s)";
			m_av_dict_free(&avDict);
			avDict = nullptr;
		}
	}

	if (SUCCEEDED(hr))
	{
		if (m_avformat_find_stream_info(pFormatCtx, NULL) < 0)
		{
			hr = E_FAIL; 
			m_errorString = "m_avformat_find_stream_info";
			return hr;
		}
	}

	mediaDuration = { LONGLONG(pFormatCtx->duration * 10000000 / double(AV_TIME_BASE)) };
	AVCodec* avVideoCodec = nullptr;
	videoindex = m_av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &avVideoCodec, 0);
	int videoStreamIndex = videoindex;
	pCodecCtx = m_avcodec_alloc_context3(avVideoCodec);
	// allocate a new decoding context
	if (!pCodecCtx)
	{
		m_errorString =L"Could not allocate a decoding context\n";
		m_avformat_close_input(&pFormatCtx);
		hr = E_OUTOFMEMORY;
	}

	//if (SUCCEEDED(hr))
	//{
	//	// initialize the stream parameters with demuxer information
	//	if (m_avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStreamIndex]->codecpar) < 0)
	//	{
	//		m_errorString = L"Could not m_avcodec_parameters_to_context\n";
	//		m_avformat_close_input(&pFormatCtx);
	//		m_avcodec_free_context(&pCodecCtx);
	//		hr = E_FAIL;
	//	}
	//}

	pCodecCtx = pFormatCtx->streams[videoindex]->codec;
	if (videoindex == AVERROR_STREAM_NOT_FOUND || !avVideoCodec)
	{
		hr = E_FAIL; 
	}
	//pCodec = avVideoCodec;
	//pCodec = m_avcodec_find_decoder_by_name(pCodecCtx->codec->name);
	pCodec = m_avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL)
	{
		hr = E_INVALIDARG;
		m_errorString = "Codec not found.\n";
	}
	if (m_avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		m_errorString = "Could not open codec.\n";
		hr = E_INVALIDARG;
	}
	m_width = pCodecCtx->width;
	m_height = pCodecCtx->height;
	m_render_width = m_width;
	m_render_height = m_height;
	if (m_width > outwidth || m_height  > outheight)
	{
		float scal = m_width / static_cast<float>(outwidth);
		float scal2 = m_height / static_cast<float>(outheight);
		if (scal > scal2)
		{
			m_render_width = static_cast<int>(m_width / scal);
			m_render_height = static_cast<int>(m_height / scal);
		}
		else
		{
			m_render_width = static_cast<int>(m_width / scal2);
			m_render_height = static_cast<int>(m_height / scal2);
		}
		m_render_width -= m_render_width % 4;
		m_render_height -= m_render_height % 4;
	}
	m_render_area = m_render_width * m_render_height;
	m_render_area_byte = m_render_area * 4;
	if (pFormatCtx->streams[videoStreamIndex]->avg_frame_rate.num != 0 || pFormatCtx->streams[videoStreamIndex]->avg_frame_rate.den != 0)
	{
		int num = pFormatCtx->streams[videoStreamIndex]->avg_frame_rate.num;
		int den = pFormatCtx->streams[videoStreamIndex]->avg_frame_rate.den;
		m_FrameRate = (double)num / den;
	}
	return hr;
}


VideoFrames^ VideoFrames::CreatVideoFrames(IRandomAccessStream^ stream, uint32_t outwidth, uint32_t outheight)
{
	auto videoFrames = ref new VideoFrames();

	HRESULT hr = videoFrames->MyCreatVideoFrames(stream, outwidth, outheight);

	return videoFrames;
}


Platform::Array<uint8_t>^ MediaCodecRT::VideoFrames::GetFrameBuffer(TimeSpan index)
{
	if (isDisposing)
	{
		return nullptr;
	}
	int ret, y_size, got_picture;

	m_av_frame_free(&pFrameYUV);
	m_av_frame_free(&pFrame);
	pFrame = m_av_frame_alloc();
	pFrameYUV = m_av_frame_alloc();
	out_buffer = (unsigned char *)m_av_malloc(m_av_image_get_buffer_size(AV_PIX_FMT_BGRA, pCodecCtx->width, pCodecCtx->height, 1));
	m_av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
		AV_PIX_FMT_BGRA, pCodecCtx->width, pCodecCtx->height, 1);
	img_convert_ctx = m_sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		m_render_width, m_render_height, AV_PIX_FMT_BGRA, SWS_BICUBIC, NULL, NULL, NULL);
	if (videoindex < 0)
	{
		return nullptr;
	}
	packet = (AVPacket *)m_av_malloc(sizeof(AVPacket));
	//InMemoryRandomAccessStream^ memstream = ref new InMemoryRandomAccessStream();
	//DataWriter^ dataWriter = ref new DataWriter(memstream->GetOutputStreamAt(0));
	double realbase = (av_q2d(pFormatCtx->streams[videoindex]->time_base) * 10000000);
	//double steprealbase = realbase / 10;
	int64_t seekTarget = static_cast<int64_t>(index.Duration / realbase);
	if (m_av_seek_frame(pFormatCtx, videoindex, seekTarget, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME) < 0)
	{
		m_avcodec_flush_buffers(pCodecCtx);
		return nullptr;
	}
	double framedur = 1000000 / m_FrameRate;
	int read_index = 0;
	while (m_av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index != videoindex)
		{
			m_av_packet_unref(packet);
			continue;
		}
		ret = m_avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
		m_PtsSec = static_cast<int64_t>(packet->pts * realbase);
		if (m_PtsSec + read_index * framedur < index.Duration)
		{
			read_index++;
			m_av_packet_unref(packet);
			continue;
		}
		if (ret < 0)
		{
			m_avcodec_flush_buffers(pCodecCtx);
			m_av_packet_unref(packet);
			return nullptr;
		}
		if (got_picture)
		{
			m_sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, m_render_height,
				pFrameYUV->data, pFrameYUV->linesize);//pCodecCtx->height
			//return nullptr;
			y_size = m_render_area_byte;
			//m_TestDoub = pFrameYUV->linesize[0];
			//m_render_area_byte = pFrameYUV->linesize[1];
			auto YBuffer = ref new Platform::Array<uint8_t>(pFrameYUV->data[0], m_render_area_byte);
			//auto br = Platform::ArrayReference<uint8_t>((pFrameYUV->data[0]), m_render_area_byte);
			//dataWriter->WriteBytes(YBuffer);
			//auto op = (concurrency::create_task(dataWriter->StoreAsync()));
			//while (!op.is_done());
			//auto op2 = concurrency::create_task(dataWriter->FlushAsync());
			//while (!op2.is_done());
			m_av_packet_unref(packet);
			m_avcodec_flush_buffers(pCodecCtx);
			return YBuffer;
			break;
		}
	}
	return nullptr;
}

bool MediaCodecRT::VideoFrames::ConvertVideoFrames(IOutputStream ^ outstream)
{


	return false;
}

void MediaCodecRT::VideoFrames::InitAutoIncrementFrameBuffer()
{
	if (isDisposing)
	{
		return;
	}

	m_FrameIndex = -1;
	m_av_frame_free(&pFrameYUV);
	m_av_frame_free(&pFrame);
	//avcodec_flush_buffers(pCodecCtx);
	pFrame = m_av_frame_alloc();
	pFrameYUV = m_av_frame_alloc();
	
	out_buffer = (unsigned char *)m_av_malloc(m_av_image_get_buffer_size(AV_PIX_FMT_BGRA, pCodecCtx->width, pCodecCtx->height, 1));
	m_av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
		AV_PIX_FMT_BGRA, pCodecCtx->width, pCodecCtx->height, 1);
	img_convert_ctx = m_sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		m_render_width, m_render_height, AV_PIX_FMT_BGRA, SWS_POINT, NULL, NULL, NULL);
	pFrameYUV->linesize[0] = m_render_width*4;
}

Platform::Array<uint8_t>^ MediaCodecRT::VideoFrames::GetAutoIncrementFrameBuffer()
{
	if (isDisposing)
	{
		return nullptr;
	}
	int ret, y_size, got_picture;
	packet = (AVPacket *)m_av_malloc(sizeof(AVPacket));
	while (m_av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index != videoindex)
		{
			m_av_packet_unref(packet);
			continue;
		}
		ret = m_avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
		if (ret < 0)
		{
			m_av_packet_unref(packet);
			return nullptr;
		}
		if (got_picture)
		{
			m_FrameIndex++;
			m_sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
				pFrameYUV->data, pFrameYUV->linesize);
			y_size = m_render_area_byte;
			auto YBuffer = ref new Platform::Array<uint8_t>(pFrameYUV->data[0], y_size);
			/*dataWriter->WriteBytes(YBuffer);
			auto op = (concurrency::create_task(dataWriter->StoreAsync()));
			while (!op.is_done());
			auto op2 = concurrency::create_task(dataWriter->FlushAsync());
			while (!op2.is_done());*/
			m_av_packet_unref(packet);
			return YBuffer;
			break;
		}
	}
	return nullptr;
}

void MediaCodecRT::VideoFrames::InitAutoIncrementFrameJpegBuffer()
{
	if (isDisposing)
	{
		return;
	}
	m_FrameIndex = -1;
	m_av_frame_free(&pFrameYUV);
	m_av_frame_free(&pFrame);
	//avcodec_flush_buffers(pCodecCtx);
	pFrame = m_av_frame_alloc();
	pFrameYUV = m_av_frame_alloc();

	out_buffer = (unsigned char *)m_av_malloc(m_av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
	m_av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
		AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);
	img_convert_ctx = m_sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		m_render_width, m_render_height, AV_PIX_FMT_YUV420P, SWS_POINT, NULL, NULL, NULL);

	pFrameYUV->linesize[0] = m_render_width;
	pFrameYUV->linesize[1] = m_render_width >> 1;
	pFrameYUV->linesize[2] = m_render_width >> 1;

	pOutCodec = m_avcodec_find_encoder_by_name("mjpeg");
	outStreamBuffer = (unsigned char*)m_av_malloc(FILESTREAMBUFFERSZ);
}

InMemoryRandomAccessStream^ MediaCodecRT::VideoFrames::GetAutoIncrementFrameJpegBuffer()
{
	if (isDisposing)
	{
		return nullptr;
	}
	int ret, y_size, got_picture;
	packet = (AVPacket *)m_av_malloc(sizeof(AVPacket));
	while (m_av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index != videoindex)
		{
			m_av_packet_unref(packet);
			continue;
		}
		ret = m_avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
		if (ret < 0)
		{
			m_av_packet_unref(packet);
			return nullptr;
		}
		if (got_picture)
		{
			m_FrameIndex++;
			m_sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
				pFrameYUV->data, pFrameYUV->linesize);

			auto bytes = SaveJpegStream(pFrameYUV);

			m_av_packet_unref(packet);
			return bytes;
			break;
		}
	}
	return nullptr;
}

InMemoryRandomAccessStream^ MediaCodecRT::VideoFrames::SaveJpegStream(AVFrame * inpicture)
{
	if (isDisposing)
	{
		return nullptr;
	}
	int ret, y_size, got_picture;
	AVFormatContext* pOutFormatCtx;
	AVIOContext* avOutIOCtx;
	AVOutputFormat* outfmt;
	AVStream* out_video_st;
	AVCodecContext* pOutCodecCtx;
	//AVCodec* pOutCodec;
	AVFrame* picture;
	uint8_t* out_picture_buf;
	AVPacket out_pkt;
	//初始化输出流
	InMemoryRandomAccessStream^ memstream = ref new InMemoryRandomAccessStream();
	HRESULT hr = S_OK;
	if (!memstream)
	{
		hr = E_INVALIDARG;
	}
	if (SUCCEEDED(hr))
	{
		hr = CreateStreamOverRandomAccessStream(reinterpret_cast<IUnknown*>(memstream), IID_PPV_ARGS(&outStreamData));
	}
	if (SUCCEEDED(hr))
	{
		avOutIOCtx = m_avio_alloc_context(outStreamBuffer, FILESTREAMBUFFERSZ, 1, outStreamData, FileStreamRead, FileStreamWrite, FileStreamSeek);
		if (avOutIOCtx == nullptr)
		{
			hr = E_OUTOFMEMORY;
			m_errorString = "m_static_avio_alloc_context";
			return nullptr;
		}
	}

	if (SUCCEEDED(hr))
	{
		pOutFormatCtx = m_avformat_alloc_context();
		if (pOutFormatCtx == nullptr)
		{
			hr = E_OUTOFMEMORY;
			m_errorString = "m_static_avformat_alloc_context";
			return nullptr;
		}
	}

	if (SUCCEEDED(hr))
	{
		pOutFormatCtx->pb = avOutIOCtx;
		pOutFormatCtx->flags |= AVFMT_FLAG_CUSTOM_IO;
		outfmt = m_av_guess_format("mjpeg", NULL, NULL);
		pOutFormatCtx->oformat = outfmt;
	}

	out_video_st = m_avformat_new_stream(pOutFormatCtx, 0);
	if (out_video_st == NULL) 
	{
		return nullptr;
	}
	pOutCodecCtx = out_video_st->codec;
	pOutCodecCtx->codec_id = outfmt->video_codec;
	pOutCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pOutCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
	pOutCodecCtx->width = m_render_width;
	pOutCodecCtx->height = m_render_height;
	pOutCodecCtx->time_base.num = 1;
	pOutCodecCtx->time_base.den = 25;

	if (!pOutCodec)
	{
		pOutCodec = m_avcodec_find_encoder(pOutCodecCtx->codec_id);
		if (!pOutCodec)
		{
			pOutCodec = m_avcodec_find_encoder_by_name("mjpeg");
		}
	}

	if (!pOutCodec)
	{
		return nullptr;
	}
	if (m_avcodec_open2(pOutCodecCtx, pOutCodec, NULL) < 0)
	{
		return nullptr;
	}
	m_avformat_write_header(pOutFormatCtx, NULL);
	y_size = m_render_area;
	m_av_new_packet(&out_pkt, y_size * 3);

	picture = inpicture;

	ret = m_avcodec_encode_video2(pOutCodecCtx, &out_pkt, picture, &got_picture);
	if (ret < 0) 
	{
		return nullptr;
	}
	if (got_picture == 1) 
	{
		out_pkt.stream_index = out_video_st->index;
		ret = m_av_write_frame(pOutFormatCtx, &out_pkt);
	}
	m_av_packet_unref(&out_pkt);
	m_av_write_trailer(pOutFormatCtx);
	if (out_video_st) 
	{
		m_avcodec_close(out_video_st->codec);
	}

	m_av_free(avOutIOCtx);
	//m_static_avio_close(pOutFormatCtx->pb);
	m_avformat_free_context(pOutFormatCtx);

	return memstream;
}

static int FileStreamRead(void* ptr, uint8_t* buf, int bufSize)
{
	IStream* pStream = reinterpret_cast<IStream*>(ptr);
	ULONG bytesRead = 0;
	HRESULT hr = pStream->Read(buf, bufSize, &bytesRead);

	if (FAILED(hr))
	{
		return -1;
	}
	if (bytesRead == 0)
	{
		return AVERROR_EOF;
	}

	return bytesRead;
}
static int FileStreamWrite(void* ptr, uint8_t* buf, int bufSize)
{
	//m_JpegReturn = ref new Platform::Array<uint8_t>(buf, bufSize);
	IStream* pStream = reinterpret_cast<IStream*>(ptr);
	ULONG bytesWrite = 0;
	HRESULT hr = pStream->Write(buf, bufSize, &bytesWrite);
	//bytesWrite = m_JpegReturn->Length;
	if (FAILED(hr))
	{
		return -1;
	}
	if (bytesWrite == 0)
	{
		return AVERROR_EOF;
	}

	return bytesWrite;
}

static int64_t FileStreamSeek(void* ptr, int64_t pos, int whence)
{
	IStream* pStream = reinterpret_cast<IStream*>(ptr);
	LARGE_INTEGER in;
	in.QuadPart = pos;
	ULARGE_INTEGER out = { 0 };

	if (FAILED(pStream->Seek(in, whence, &out)))
	{
		return -1;
	}

	return out.QuadPart;
}

const char* ConvertToCStr(String^ str)
{
	const char* charStr = nullptr;
	std::wstring uriW(str->Begin());
	std::string uriA(uriW.begin(), uriW.end());
	charStr = uriA.c_str();
	return  charStr;
}

HRESULT ConvertCodecName(const char* codecName, String^ *outputCodecName)
{
	HRESULT hr = S_OK;

	auto codecNameChars = codecName;
	size_t newsize = strlen(codecNameChars) + 1;
	wchar_t * wcstring = new(std::nothrow) wchar_t[newsize];
	if (wcstring == nullptr)
	{
		hr = E_OUTOFMEMORY;
	}

	if (SUCCEEDED(hr))
	{
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, codecNameChars, _TRUNCATE);
		*outputCodecName = ref new Platform::String(wcstring);
		delete[] wcstring;
	}

	return hr;
}

MediaCodecRT::VideoFrames::~VideoFrames()
{
	isDisposing = true;
	m_mutexGuard.lock();
	m_av_frame_free(&pFrameYUV);//util
	m_av_frame_free(&pFrame);
	m_avcodec_close(pCodecCtx);
	m_avformat_close_input(&pFormatCtx);
	m_av_free(avIOCtx);//util
	m_av_free(out_buffer);
	m_av_dict_free(&avDict);//util

	if (fileStreamData != nullptr)
	{
		fileStreamData->Release();
	}
	m_mutexGuard.unlock();
}




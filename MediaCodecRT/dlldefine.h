#pragma once
#include <pch.h>
#include <Windows.h>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};

typedef HMODULE WINAPI
LoadLibraryExWPtr(
	_In_ LPCWSTR lpLibFileName,
	_Reserved_ HANDLE hFile,
	_In_ DWORD dwFlags
);
typedef WINUSERAPI int WINAPI MessageBoxWPtr(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCWSTR lpText,
	_In_opt_ LPCWSTR lpCaption,
	_In_ UINT uType);

typedef PVOID DLL_DIRECTORY_COOKIE, *PDLL_DIRECTORY_COOKIE;
typedef WINBASEAPI DLL_DIRECTORY_COOKIE WINAPI AddDllDirectory(
	PCWSTR NewDirectory
);

typedef void WINAPIV def_av_register_all(void);

typedef void WINAPIV def_avcodec_register_all(void);

typedef  void* WINAPIV def_av_malloc(size_t size) av_malloc_attrib av_alloc_size(1);

typedef AVIOContext * WINAPIV def_avio_alloc_context(
	unsigned char *buffer,
	int buffer_size,
	int write_flag,
	void *opaque,
	int(*read_packet)(void *opaque, uint8_t *buf, int buf_size),
	int(*write_packet)(void *opaque, uint8_t *buf, int buf_size),
	int64_t(*seek)(void *opaque, int64_t offset, int whence));

typedef AVFormatContext * WINAPIV def_avformat_alloc_context(void);

typedef int WINAPIV def_avformat_open_input(AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options);

typedef void WINAPIV def_av_dict_free(AVDictionary **m);

typedef int WINAPIV def_avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options);

typedef int WINAPIV def_av_find_best_stream(AVFormatContext *ic,
	enum AVMediaType type,
	int wanted_stream_nb,
	int related_stream,
	AVCodec **decoder_ret,
	int flags);

typedef AVCodecContext * WINAPIV def_avcodec_alloc_context3(const AVCodec *codec);
//avcodec_parameters_to_context
//typedef int WINAPIV def_avcodec_parameters_to_context(AVCodecContext *codec,
//	const AVCodecParameters *par);

typedef AVCodec * WINAPIV def_avcodec_find_decoder(enum AVCodecID id);

typedef AVCodec * WINAPIV def_avcodec_find_decoder_by_name(const char *name);

typedef int WINAPIV def_avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);

typedef void WINAPIV def_av_frame_free(AVFrame **frame);

typedef AVFrame * WINAPIV def_av_frame_alloc(void);

typedef int WINAPIV def_av_image_get_buffer_size(enum AVPixelFormat pix_fmt, int width, int height, int align);

typedef int WINAPIV def_av_image_fill_arrays(uint8_t *dst_data[4], int dst_linesize[4],
	const uint8_t *src,
	enum AVPixelFormat pix_fmt, int width, int height, int align);

typedef struct SwsContext * WINAPIV def_sws_getContext(int srcW, int srcH, enum AVPixelFormat srcFormat,
	int dstW, int dstH, enum AVPixelFormat dstFormat,
	int flags, SwsFilter *srcFilter,
	SwsFilter *dstFilter, const double *param);

typedef int WINAPIV def_av_seek_frame(AVFormatContext *s, int stream_index, int64_t timestamp, int flags);

typedef void WINAPIV def_avcodec_flush_buffers(AVCodecContext *avctx);

typedef int WINAPIV def_av_read_frame(AVFormatContext *s, AVPacket *pkt);

typedef void WINAPIV def_av_packet_unref(AVPacket *pkt);

typedef int WINAPIV def_avcodec_decode_video2(AVCodecContext *avctx, AVFrame *picture,
	int *got_picture_ptr,
	const AVPacket *avpkt);

typedef int WINAPIV def_sws_scale(struct SwsContext *c, const uint8_t *const srcSlice[],
	const int srcStride[], int srcSliceY, int srcSliceH,
	uint8_t *const dst[], const int dstStride[]);

typedef int WINAPIV def_avcodec_close(AVCodecContext *avctx);

typedef void WINAPIV def_av_free(void *ptr);

typedef void WINAPIV def_avformat_close_input(AVFormatContext **s);

//typedef int WINAPIV def_avcodec_parameters_to_context(AVCodecContext *codec,
//	const AVCodecParameters *par);

typedef void WINAPIV def_avcodec_free_context(AVCodecContext **avctx);

//2019-1-4 jpeg
typedef AVOutputFormat * WINAPIV def_av_guess_format(const char *short_name,
	const char *filename,
	const char *mime_type);

typedef int WINAPIV def_avformat_alloc_output_context2(AVFormatContext **ctx, AVOutputFormat *oformat,
	const char *format_name, const char *filename);

typedef AVStream * WINAPIV def_avformat_new_stream(AVFormatContext *s, const AVCodec *c);

typedef AVCodec * WINAPIV def_avcodec_find_encoder(enum AVCodecID id);

typedef AVCodec * WINAPIV def_avcodec_find_encoder_by_name(const char *name);

typedef int WINAPIV def_avcodec_encode_video2(AVCodecContext *avctx, AVPacket *avpkt,
	const AVFrame *frame, int *got_packet_ptr);

typedef int WINAPIV def_av_write_frame(AVFormatContext *s, AVPacket *pkt);

typedef int WINAPIV def_av_write_trailer(AVFormatContext *s);

typedef int WINAPIV def_avio_close(AVIOContext *s);

typedef void WINAPIV def_avformat_free_context(AVFormatContext *s);

typedef int WINAPIV def_avformat_write_header(AVFormatContext *s, AVDictionary **options);

typedef int WINAPIV def_av_new_packet(AVPacket *pkt, int size);

typedef AVCodec * WINAPIV def_av_codec_next(const AVCodec *c);


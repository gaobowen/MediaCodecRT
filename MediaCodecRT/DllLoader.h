#pragma once
#include <pch.h>
#include "dlldefine.h"
using namespace Platform;

extern  HMODULE avutil_55_mod;
extern  HMODULE avcodec_57_mod;
extern  HMODULE avformat_57_mod;
extern  HMODULE swscale_4_mod;
extern  HMODULE kernelAddr;

extern  def_av_register_all * m_av_register_all;
extern  def_av_malloc* m_av_malloc;
extern  def_avio_alloc_context* m_avio_alloc_context;
extern  def_avformat_alloc_context* m_avformat_alloc_context;
extern  def_avformat_open_input* m_avformat_open_input;
extern  def_av_dict_free* m_av_dict_free;
extern  def_avformat_find_stream_info* m_avformat_find_stream_info;
extern  def_av_find_best_stream* m_av_find_best_stream;
extern  def_avcodec_alloc_context3* m_avcodec_alloc_context3;
//extern  def_avcodec_parameters_to_context* m_avcodec_parameters_to_context;
extern  def_avcodec_find_decoder* m_avcodec_find_decoder;
extern  def_avcodec_open2* m_avcodec_open2;
extern  def_av_frame_free* m_av_frame_free;
extern  def_av_frame_alloc* m_av_frame_alloc;
extern  def_av_image_get_buffer_size* m_av_image_get_buffer_size;
extern  def_av_image_fill_arrays* m_av_image_fill_arrays;
extern  def_sws_getContext* m_sws_getContext;
extern  def_av_seek_frame* m_av_seek_frame;
extern  def_avcodec_flush_buffers* m_avcodec_flush_buffers;
extern  def_av_read_frame* m_av_read_frame;
extern  def_av_packet_unref* m_av_packet_unref;
extern  def_avcodec_decode_video2* m_avcodec_decode_video2;
extern  def_sws_scale* m_sws_scale;
extern  def_avcodec_close* m_avcodec_close;
extern  def_avformat_close_input* m_avformat_close_input;
extern  def_av_free* m_av_free;
extern def_avcodec_free_context* m_avcodec_free_context;
extern def_avcodec_find_decoder_by_name* m_avcodec_find_decoder_by_name;
extern def_av_guess_format* m_av_guess_format;
extern def_avformat_alloc_output_context2* m_avformat_alloc_output_context2;
extern def_avformat_new_stream* m_avformat_new_stream;
extern def_avcodec_find_encoder* m_avcodec_find_encoder;
extern def_avcodec_find_encoder* m_static_avcodec_find_encoder;
extern def_avcodec_find_encoder_by_name* m_avcodec_find_encoder_by_name;
extern def_avcodec_find_encoder_by_name* m_static_avcodec_find_encoder_by_name;
extern def_avcodec_encode_video2* m_avcodec_encode_video2;
extern def_av_write_frame* m_av_write_frame;
extern def_av_write_trailer* m_av_write_trailer;
extern def_avio_close* m_avio_close;
extern def_avformat_free_context* m_avformat_free_context;
extern def_avformat_write_header* m_avformat_write_header;
extern def_av_new_packet* m_av_new_packet;

extern  def_avio_alloc_context* m_static_avio_alloc_context;
extern  def_avformat_alloc_context* m_static_avformat_alloc_context;
extern  def_avcodec_open2* m_static_avcodec_open2;
extern  def_av_guess_format* m_static_av_guess_format;
extern  def_avformat_alloc_output_context2* m_static_avformat_alloc_output_context2;
extern  def_avformat_new_stream* m_static_avformat_new_stream;
extern  def_avcodec_encode_video2* m_static_avcodec_encode_video2;
extern  def_av_write_frame* m_static_av_write_frame;;
extern  def_av_write_trailer* m_static_av_write_trailer;
extern  def_avio_close* m_static_avio_close;
extern  def_avformat_free_context* m_static_avformat_free_context;
extern  def_avformat_write_header* m_static_avformat_write_header;
extern  def_avcodec_find_encoder* m_static_avcodec_find_encoder;
extern  def_avcodec_find_encoder_by_name* m_static_avcodec_find_encoder_by_name;
extern  def_av_malloc* m_static_av_malloc;
extern   def_av_frame_alloc* m_static_av_frame_alloc;
extern def_av_new_packet* m_static_av_new_packet;
extern def_av_packet_unref * m_static_av_packet_unref;
extern  def_avcodec_close* m_static_avcodec_close;
extern   def_av_free* m_static_av_free;
extern def_av_register_all * my_static_av_register_all;
extern def_avcodec_register_all* m_avcodec_register_all;
extern def_av_codec_next* m_av_codec_next;

namespace MediaCodecRT
{
	public ref class DllLoader sealed
	{
	public:
		DllLoader();
		bool LoadAll();

		virtual ~DllLoader();
	private:

	};
}



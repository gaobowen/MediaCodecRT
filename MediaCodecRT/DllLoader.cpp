#include "pch.h"
#include "DllLoader.h"
using namespace MediaCodecRT;

  HMODULE avutil_55_mod;
  HMODULE avcodec_57_mod;
  HMODULE avformat_57_mod;
  HMODULE swscale_4_mod;
  HMODULE swresample_2_mod;
  HMODULE avdevice_57_mod;
  HMODULE avfilter_6_mod;

  HMODULE kernelAddr;

  def_av_register_all * m_av_register_all;
  def_av_malloc* m_av_malloc;
  def_avio_alloc_context* m_avio_alloc_context;
  def_avformat_alloc_context* m_avformat_alloc_context;
  def_avformat_open_input* m_avformat_open_input;
  def_av_dict_free* m_av_dict_free;
  def_avformat_find_stream_info* m_avformat_find_stream_info;
  def_av_find_best_stream* m_av_find_best_stream;
  def_avcodec_alloc_context3* m_avcodec_alloc_context3;

  //def_avcodec_parameters_to_context* m_avcodec_parameters_to_context;
  
  def_avcodec_open2* m_avcodec_open2;
  def_av_frame_free* m_av_frame_free;
  def_av_frame_alloc* m_av_frame_alloc;
  def_av_image_get_buffer_size* m_av_image_get_buffer_size;
  def_av_image_fill_arrays* m_av_image_fill_arrays;
  def_sws_getContext* m_sws_getContext;
  def_av_seek_frame* m_av_seek_frame;
  def_avcodec_flush_buffers* m_avcodec_flush_buffers;
  def_av_read_frame* m_av_read_frame;
  def_av_packet_unref* m_av_packet_unref;
  def_avcodec_decode_video2* m_avcodec_decode_video2;
  def_sws_scale* m_sws_scale;
  def_avcodec_close* m_avcodec_close;
  def_avformat_close_input* m_avformat_close_input;
  def_av_free* m_av_free;
  def_avcodec_free_context* m_avcodec_free_context;

  def_avcodec_find_decoder* m_avcodec_find_decoder;
  def_avcodec_find_decoder_by_name* m_avcodec_find_decoder_by_name;

  def_av_guess_format* m_av_guess_format;
  def_avformat_alloc_output_context2* m_avformat_alloc_output_context2;
  def_avformat_new_stream* m_avformat_new_stream;
  def_avcodec_encode_video2* m_avcodec_encode_video2;
  def_av_write_frame* m_av_write_frame;;
  def_av_write_trailer* m_av_write_trailer;
  def_avio_close* m_avio_close;
  def_avformat_free_context* m_avformat_free_context;
  def_avformat_write_header* m_avformat_write_header;
  def_av_new_packet* m_av_new_packet;
  def_avcodec_find_encoder* m_avcodec_find_encoder;
  def_avcodec_find_encoder_by_name* m_avcodec_find_encoder_by_name;
  def_avcodec_register_all* m_avcodec_register_all;
  def_av_codec_next* m_av_codec_next;


  //def_avio_alloc_context* m_static_avio_alloc_context;
  //def_avformat_alloc_context* m_static_avformat_alloc_context;
  //def_avcodec_open2* m_static_avcodec_open2;
  //def_av_guess_format* m_static_av_guess_format;
  //def_avformat_alloc_output_context2* m_static_avformat_alloc_output_context2;
  //def_avformat_new_stream* m_static_avformat_new_stream;
  //def_avcodec_encode_video2* m_static_avcodec_encode_video2;
  //def_av_write_frame* m_static_av_write_frame;
  //def_av_write_trailer* m_static_av_write_trailer;
  //def_avio_close* m_static_avio_close;
  //def_avformat_free_context* m_static_avformat_free_context;
  //def_avformat_write_header* m_static_avformat_write_header;
  //def_avcodec_find_encoder* m_static_avcodec_find_encoder;
  //def_avcodec_find_encoder_by_name* m_static_avcodec_find_encoder_by_name;
  //def_av_malloc* m_static_av_malloc;
  //def_av_frame_alloc* m_static_av_frame_alloc;
  //def_av_new_packet* m_static_av_new_packet;
  //def_av_packet_unref * m_static_av_packet_unref;
  //def_avcodec_close* m_static_avcodec_close;
  //def_av_free* m_static_av_free;
  //def_av_register_all * my_static_av_register_all;



DllLoader::DllLoader()
{
}

bool MediaCodecRT::DllLoader::LoadAll()
{
	MEMORY_BASIC_INFORMATION info = {};
	if (VirtualQuery(VirtualQuery, &info, sizeof(info)))
	{
		kernelAddr = (HMODULE)info.AllocationBase;
		auto loadlibraryPtr = (int64_t)GetProcAddress(kernelAddr, "LoadLibraryExW");
		auto loadLibrary = (LoadLibraryExWPtr*)loadlibraryPtr;
		auto user32_mod = loadLibrary(L"user32.dll", nullptr, 0);
		//auto kernel32Addr = loadLibrary(L"kernel32.dll", nullptr, 0);
		//auto allfmpeg = loadLibrary(L"ffmpeg.adll", nullptr, 0);
		//m_av_malloc/av_frame_free/av_frame_alloc/av_image_get_buffer_size/av_image_fill_arrays/
		avutil_55_mod = loadLibrary(L"avutil-55.dll", nullptr, 0);
		//avutil_55_mod = allfmpeg;
		m_av_malloc = (def_av_malloc*)GetProcAddress(avutil_55_mod, "av_malloc");
		m_av_frame_free = (def_av_frame_free*)GetProcAddress(avutil_55_mod, "av_frame_free");
		m_av_frame_alloc = (def_av_frame_alloc*)GetProcAddress(avutil_55_mod, "av_frame_alloc");
		m_av_image_get_buffer_size = (def_av_image_get_buffer_size*)GetProcAddress(avutil_55_mod, "av_image_get_buffer_size");
		m_av_image_fill_arrays = (def_av_image_fill_arrays*)GetProcAddress(avutil_55_mod, "av_image_fill_arrays");
		m_av_free = (def_av_free*)GetProcAddress(avutil_55_mod, "av_free");
		m_av_dict_free = (def_av_dict_free*)GetProcAddress(avutil_55_mod, "av_dict_free");


		//avcodec_alloc_context3/avcodec_find_decoder/avcodec_open2/avcodec_flush_buffers/av_packet_unref/avcodec_decode_video2
		avcodec_57_mod = loadLibrary(L"avcodec-57.dll", nullptr, 0);
		//avcodec_57_mod = allfmpeg;
		m_avcodec_alloc_context3 = (def_avcodec_alloc_context3*)GetProcAddress(avcodec_57_mod, "avcodec_alloc_context3");
		m_avcodec_find_decoder = (def_avcodec_find_decoder*)GetProcAddress(avcodec_57_mod, "avcodec_find_decoder");
		m_avcodec_open2 = (def_avcodec_open2*)GetProcAddress(avcodec_57_mod, "avcodec_open2");
		m_avcodec_flush_buffers = (def_avcodec_flush_buffers*)GetProcAddress(avcodec_57_mod, "avcodec_flush_buffers");
		m_av_packet_unref = (def_av_packet_unref*)GetProcAddress(avcodec_57_mod, "av_packet_unref");
		m_avcodec_decode_video2 = (def_avcodec_decode_video2*)GetProcAddress(avcodec_57_mod, "avcodec_decode_video2");
		//m_avcodec_parameters_to_context = (def_avcodec_parameters_to_context*)GetProcAddress(avcodec_57_mod, "avcodec_parameters_to_context");
		m_avcodec_free_context = (def_avcodec_free_context*)GetProcAddress(avcodec_57_mod, "avcodec_free_context");
		m_avcodec_find_decoder_by_name = (def_avcodec_find_decoder_by_name*)GetProcAddress(avcodec_57_mod, "avcodec_find_decoder_by_name");
		m_avcodec_close = (def_avcodec_close*)GetProcAddress(avcodec_57_mod, "avcodec_close");
		m_avcodec_find_encoder = (def_avcodec_find_encoder*)GetProcAddress(avcodec_57_mod, "avcodec_find_encoder");
		m_avcodec_encode_video2 = (def_avcodec_encode_video2*)GetProcAddress(avcodec_57_mod, "avcodec_encode_video2");

		m_av_new_packet = (def_av_new_packet*)GetProcAddress(avcodec_57_mod, "av_new_packet");
		m_avcodec_find_encoder_by_name = (def_avcodec_find_encoder_by_name*)GetProcAddress(avcodec_57_mod, "avcodec_find_encoder_by_name");
		m_avcodec_register_all = (def_avcodec_register_all*)GetProcAddress(avcodec_57_mod, "avcodec_register_all");
		m_av_codec_next = (def_av_codec_next*)GetProcAddress(avcodec_57_mod, "av_codec_next");


		//m_av_register_all/avio_alloc_context/avformat_alloc_context/avformat_open_input/avformat_find_stream_info/av_find_best_stream
		//av_seek_frame/av_read_frame
		avformat_57_mod = loadLibrary(L"avformat-57.dll", nullptr, 0);
		//avformat_57_mod = allfmpeg;
		m_av_register_all = (def_av_register_all*)GetProcAddress(avformat_57_mod, "av_register_all");
		m_avio_alloc_context = (def_avio_alloc_context*)GetProcAddress(avformat_57_mod, "avio_alloc_context");
		m_avformat_alloc_context = (def_avformat_alloc_context*)GetProcAddress(avformat_57_mod, "avformat_alloc_context");
		m_avformat_open_input = (def_avformat_open_input*)GetProcAddress(avformat_57_mod, "avformat_open_input");
		m_avformat_find_stream_info = (def_avformat_find_stream_info*)GetProcAddress(avformat_57_mod, "avformat_find_stream_info");
		m_av_find_best_stream = (def_av_find_best_stream*)GetProcAddress(avformat_57_mod, "av_find_best_stream");
		m_av_seek_frame = (def_av_seek_frame*)GetProcAddress(avformat_57_mod, "av_seek_frame");
		m_av_read_frame = (def_av_read_frame*)GetProcAddress(avformat_57_mod, "av_read_frame");
		m_avformat_close_input = (def_avformat_close_input*)GetProcAddress(avformat_57_mod, "avformat_close_input");
		m_av_guess_format = (def_av_guess_format*)GetProcAddress(avformat_57_mod, "av_guess_format");
		m_avformat_alloc_output_context2 = (def_avformat_alloc_output_context2*)GetProcAddress(avformat_57_mod, "avformat_alloc_output_context2");
		m_avformat_new_stream = (def_avformat_new_stream*)GetProcAddress(avformat_57_mod, "avformat_new_stream");
		m_avio_close = (def_avio_close*)GetProcAddress(avformat_57_mod, "avio_close");
		m_avformat_free_context = (def_avformat_free_context*)GetProcAddress(avformat_57_mod, "avformat_free_context");
		m_avformat_write_header = (def_avformat_write_header*)GetProcAddress(avformat_57_mod, "avformat_write_header");
		m_av_write_frame = (def_av_write_frame*)GetProcAddress(avformat_57_mod, "av_write_frame");
		m_av_write_trailer = (def_av_write_trailer*)GetProcAddress(avformat_57_mod, "av_write_trailer");



		//sws_getContext/sws_scale
		swscale_4_mod = loadLibrary(L"swscale-4.dll", nullptr, 0);
		//swscale_4_mod = allfmpeg;
		m_sws_getContext = (def_sws_getContext*)GetProcAddress(swscale_4_mod, "sws_getContext");
		m_sws_scale = (def_sws_scale*)GetProcAddress(swscale_4_mod, "sws_scale");

		//swresample_2_mod = loadLibrary(L"swresample-2.dll", nullptr, 0);
		//avdevice_57_mod = loadLibrary(L"avdevice-57.dll", nullptr, 0);
		//avfilter_6_mod = loadLibrary(L"avfilter-6.dll", nullptr, 0);

		if (avutil_55_mod != nullptr && avcodec_57_mod != nullptr&& avformat_57_mod != nullptr&& swscale_4_mod != nullptr)
		{
			m_avcodec_register_all();
			//AVCodec *c = m_av_codec_next(NULL);

			//AVCodec* bbb = m_avcodec_find_encoder_by_name("mjpeg");
			//decodec->
			//if (encodec == NULL)
			//{
			//	return false;
			//}
			//auto allfmpeg = loadLibrary(L"ffmpeg.txt", nullptr, 0);
			//if (allfmpeg != NULL)
			//{
			//	my_static_av_register_all = (def_av_register_all*)GetProcAddress(allfmpeg, "av_register_all");
			//	m_static_avio_alloc_context = (def_avio_alloc_context*)GetProcAddress(allfmpeg, "avio_alloc_context");
			//	m_static_avformat_alloc_context = (def_avformat_alloc_context*)GetProcAddress(allfmpeg, "avformat_alloc_context");
			//	m_static_avcodec_open2 = (def_avcodec_open2*)GetProcAddress(allfmpeg, "avcodec_open2");
			//	m_static_av_guess_format = (def_av_guess_format*)GetProcAddress(allfmpeg, "av_guess_format");
			//	m_static_avformat_alloc_output_context2 = (def_avformat_alloc_output_context2*)GetProcAddress(allfmpeg, "avformat_alloc_output_context2");
			//	m_static_avformat_new_stream = (def_avformat_new_stream*)GetProcAddress(allfmpeg, "avformat_new_stream");
			//	m_static_avcodec_encode_video2 = (def_avcodec_encode_video2*)GetProcAddress(allfmpeg, "avcodec_encode_video2");
			//	m_static_av_write_frame = (def_av_write_frame*)GetProcAddress(allfmpeg, "av_write_frame");
			//	m_static_av_write_trailer = (def_av_write_trailer*)GetProcAddress(allfmpeg, "av_write_trailer");
			//	m_static_avio_close = (def_avio_close*)GetProcAddress(allfmpeg, "avio_close");
			//	m_static_avformat_free_context = (def_avformat_free_context*)GetProcAddress(allfmpeg, "avformat_free_context");
			//	m_static_avformat_write_header = (def_avformat_write_header*)GetProcAddress(allfmpeg, "avformat_write_header");
			//	m_static_avcodec_find_encoder = (def_avcodec_find_encoder*)GetProcAddress(allfmpeg, "avcodec_find_encoder");
			//	m_static_avcodec_find_encoder_by_name = (def_avcodec_find_encoder_by_name*)GetProcAddress(allfmpeg, "avcodec_find_encoder_by_name");
			//	m_static_av_malloc = (def_av_malloc*)GetProcAddress(allfmpeg, "av_malloc");
			//	m_static_av_frame_alloc = (def_av_frame_alloc*)GetProcAddress(allfmpeg, "av_frame_alloc");
			//	m_static_av_new_packet = (def_av_new_packet*)GetProcAddress(allfmpeg, "av_new_packet");
			//	m_static_av_packet_unref = (def_av_packet_unref*)GetProcAddress(allfmpeg, "av_packet_unref");
			//	m_static_avcodec_close = (def_avcodec_close*)GetProcAddress(allfmpeg, "avcodec_close");
			//	m_static_av_free = (def_av_free*)GetProcAddress(allfmpeg, "av_free");
			//}

			return true;
		}



	}
	return false;
}


DllLoader::~DllLoader()
{
}

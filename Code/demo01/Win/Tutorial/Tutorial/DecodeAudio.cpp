#include "DecodeAudio.h"

#pragma comment(lib,"Winmm")



DecodeAudio::DecodeAudio()
{


}


DecodeAudio::~DecodeAudio()
{
}
 

bool DecodeAudio::init_audio_device(char* block, int size)
{
	return true;
}

int DecodeAudio::decode_audio()
{
	AVFormatContext *pFormatCtx;
	int i, audioStream;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	char* url = "./wangzhe_.mp4";
	//strcpy(url, no_use);

	av_register_all();
	//支持网络流输入
	avformat_network_init();
	//初始化
	pFormatCtx = avformat_alloc_context();
	//有参数 avdic
	//if(avformat_open_input(&pFormatCtx,url,NULL,&avdic)!=0){
	if (avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0) 
	{
		printf("Couldn't open file.\n");
		return -1;
	}
	// Retrieve stream information
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}
	// Dump valid information onto standard error
	av_dump_format(pFormatCtx, 0, url, false);
	// Find the first audio stream
	audioStream = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++) {
		//原为 codec_type==CODEC_TYPE_AUDIO
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStream = i;
			break;
		}
	}
	
	if (audioStream == -1)
	{
		printf("Didn't find a audio stream.\n");
		return -1;
	}
	// Get a pointer to the codec context for the audio stream
	pCodecCtx = pFormatCtx->streams[audioStream]->codec;
	// Find the decoder for the audio stream
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL)
	{
		printf("Codec not found.\n");
		return -1;
	}
	// Open codec
	if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
	{
		printf("Could not open codec.\n");
		return -1;
	}



	//把结构体改为指针
	AVPacket *packet = (AVPacket *)malloc(sizeof(AVPacket));
	av_init_packet(packet);
	//音频和视频解码更加统一！
	//新加
	AVFrame *pFrame;
	pFrame = av_frame_alloc();


	//-----------------------------------------------------
	printf("比特率 %3d\n", pFormatCtx->bit_rate);
	printf("解码器名称 %s\n", pCodecCtx->codec->long_name);
	printf("time_base %d \n", pCodecCtx->time_base);
	printf("声道数 %d \n", pCodecCtx->channels);
	printf("sample per second %d \n", pCodecCtx->sample_rate);
	//新版不再需要

		//  short decompressed_audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
		//  int decompressed_audio_buf_size;
	uint32_t ret, len = 0;
	int got_picture;
	int index = 0;

	FILE *pFile = fopen("./src.pcm", "wb");

	while (av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index == audioStream)
		{
			//decompressed_audio_buf_size = (AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2;
			//原为 avcodec_decode_audio2
			//ret = avcodec_decode_audio4( pCodecCtx, decompressed_audio_buf,
			//&decompressed_audio_buf_size, packet.data, packet.size );
			//改为
			ret = avcodec_decode_audio4(pCodecCtx, pFrame,
				&got_picture, packet);
			if (ret < 0) // if error len = -1
			{
				printf("Error in decoding audio frame.\n");
				exit(0);
			}
			if (got_picture > 0)
			{
#if 1
				printf("index %3d\n", index);
				printf("pts %5d\n", packet->pts);
				printf("dts %5d\n", packet->dts);
				printf("packet_size %5d\n", packet->size);
				//printf("test %s\n", rtmp->m_inChunkSize);
#endif
//直接写入
//注意：数据是 data【0】 ，长度是 linesize【0】
#if 1



				fwrite(pFrame->data[0], 1, pFrame->linesize[0], pFile);



				//fwrite(pFrame, 1, got_picture, pFile);
				//len+=got_picture;
				index++;
				//fprintf(pTSFile, "%4d,%5d,%8d\n", index, decompressed_audio_buf_size, packet.pts);
#endif
			}

		}
		// Free the packet that was allocated by av_read_frame
		//已改
		av_free_packet(packet);
	}
	avcodec_close(pCodecCtx);

	avformat_close_input(&pFormatCtx);
	fclose(pFile);
	return 0;

}

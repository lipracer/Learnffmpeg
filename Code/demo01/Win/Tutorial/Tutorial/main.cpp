#include <Windows.h>
#include <cstdio>
#include <iostream>

#define SRC_FILE "./wangzhe.mp4"

extern "C" 
{
#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libswscale\swscale.h"
}
//swscale-5.def;swresample-3.def;postproc-55.def;avcodec-58.def;avdevice-58.def;avfilter-7.def;avformat-58.def;avutil-56.def
using namespace std;

void WriteToBmp(char* filename, char *data, int width, int height, int ByteCount=3)
{
    int width_ = width;
    if (ByteCount != 4)
    {
        while (width_ * 3 % 4) ++width_;
    }

    tagBITMAPFILEHEADER tagFileHead;
    tagBITMAPINFOHEADER tagInfoHead;

    memset(&tagFileHead, 0, sizeof(tagFileHead));
    memset(&tagInfoHead, 0, sizeof(tagInfoHead));

    tagFileHead.bfType = (('M' << 8) | 'B');
    tagFileHead.bfSize = sizeof(tagFileHead) + sizeof(tagInfoHead) + width_ * height * ByteCount;
    tagFileHead.bfOffBits = sizeof(tagFileHead) + sizeof(tagInfoHead);



    tagInfoHead.biSize = sizeof(tagInfoHead);
    tagInfoHead.biWidth = width_;
    tagInfoHead.biHeight = height;
    tagInfoHead.biPlanes = 1;
    tagInfoHead.biBitCount = ByteCount << 3;
    tagInfoHead.biCompression = 0;

    tagInfoHead.biSizeImage = width_ * height * ByteCount;

    FILE *pf = nullptr;
    pf = fopen(filename, "wb");
    if (pf) 
    {
        fwrite(&tagFileHead, sizeof(tagFileHead), 1, pf);
        fwrite(&tagInfoHead, sizeof(tagInfoHead), 1, pf);

        if (4 == ByteCount)
        {
            fwrite(data, width * height * 4, 1, pf);
        }
        else if(3 == ByteCount)
        {
            int nOLineSize = (width * 3);
            int addSize = width_ * 3 - width * 3;
            for (int i = height-1; i >= 0; --i)
            {
                fwrite(data + i * nOLineSize, nOLineSize, 1, pf);
                fwrite(data, addSize, 1, pf);
            }
        }
        
        fclose(pf);
    }


}

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) 
{
    FILE *pFile = nullptr;
    char szFilename[32] = { 0 };

    int y;
    //Open file
    sprintf(szFilename, "fram%d.ppm", iFrame);
    pFile = fopen(szFilename, "wb");

    if (NULL == pFile)
    {
        return;
    }

    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    for (y=0; y < height; y++)
    {
        fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);
    }
    fclose(pFile);

    memset(szFilename, 0, sizeof(szFilename));
    sprintf(szFilename, "fram%d.bmp", iFrame);
    WriteToBmp(szFilename, (char*)pFrame->data[0], width, height);
}


int main(int argc, char* *argv) 
{
    //char* src = new char[250 * 250 * 3];
    //memset(src, 0, 250*250*3);
    //WriteToBmp("./test.bmp", src, 250, 250, 3);
    //return 0;

    AVFormatContext *pFormatCtx;
    int i, videoStream;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pFrame;
    AVFrame *pFrameRGB;
    AVPacket packet;
    int frameFinished;
    int numBytes;
    uint8_t *buffer;


    //register all formats and codes
    av_register_all();
    //support network stream input
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    if (0 != avformat_open_input(&pFormatCtx, SRC_FILE, NULL, NULL))
    {
        printf("couldn't open file\n");
        return -1;
    }

    //retrieve stream information
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        return -1;
    }

    av_dump_format(pFormatCtx, -1, SRC_FILE, 0);

    videoStream = -1;

    for (int i = 0; i < pFormatCtx->nb_streams; ++i)
    {
        if (AVMEDIA_TYPE_VIDEO == pFormatCtx->streams[i]->codec->codec_type) 
        {
            videoStream = i;
            break;
        }
    }

    if (-1 == videoStream)
    {
        return - 1;
    }

    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

    if (NULL == pCodec)
    {

        return -1;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) 
    {
        return -1;
    }

    pFrame = av_frame_alloc();
    if (NULL == pFrame)
    {
        return -1;
    }

    pFrameRGB = av_frame_alloc();
    if (NULL == pFrameRGB)
    {
        return -1;
    }

    numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,
        pCodecCtx->height);

    buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture*)pFrameRGB, buffer, AV_PIX_FMT_RGB24,
        pCodecCtx->width, pCodecCtx->height);

    i = 0;
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if (videoStream == packet.stream_index) 
        {
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
                &packet);
            if (frameFinished)
            {
                struct SwsContext *img_convert_ctx = NULL;
                img_convert_ctx = sws_getCachedContext(img_convert_ctx,
                    pCodecCtx->width, pCodecCtx->height,
                    pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                    AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
                
                if (!img_convert_ctx) 
                {
                    exit(1);
                }

                sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                    pFrameRGB->data, pFrameRGB->linesize);

                if (i++ < 50) 
                {
                    SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);
                }
            }
        }
        av_free_packet(&packet);
    }

    av_free(buffer);
    av_free(pFrameRGB);
    av_free(pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    system("pause");
}
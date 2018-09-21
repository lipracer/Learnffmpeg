#include <Windows.h>
#include <cstdio>
#include <iostream>

#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libswscale\swscale.h"

using namespace std;

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
}


int main(int argc, char* *argv) 
{
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

    if (argc < 2)
    {
        cout << "please provide a movie file\n";
        return -1;
    }

    system("pause");
}
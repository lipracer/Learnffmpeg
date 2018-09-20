#include <Windows.h>
#include <cstdio>

#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libswscale\swscale.h"

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) 
{

    FILE *pFile = nullptr;
    char szFilename[32] = { 0 };

    int y;

    sprintf(szFilename, "fram");

}


int main() 
{
    system("pause");
}
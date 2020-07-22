/**
 * @file thumbnail.h
 *
 * @brief YUV NV12 to Scaled down PNG export
 * @ingroup Recorder
 * @author Naveen K
 *
 */

#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdint>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/error.h>
}

class thumbnail
{
    public:
        thumbnail(int inwidth, int inheight, int tnailwidth, int tnailheight);
        ~thumbnail();

        int savepng(const char *pngfilename, uint8_t *Ybuf, uint8_t *UVbuf);

    private:
        int W_in;
        int H_in;
        int W_out;
        int H_out;

        AVFrame* pYUVframe = nullptr;
        AVFrame *pRGBframe = nullptr;
        struct SwsContext *sws_ctx = nullptr;

        uint8_t* RGBbuffer;

        AVCodecContext* pCodecCtxPng;
        AVCodec* pCodecPng;
        AVPacket pktPng{};

};


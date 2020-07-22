/**
 * @file thumbnail.cpp
 *
 * @brief YUV NV12 to Scaled down PNG export
 * @ingroup Recorder
 * @author Naveen K
 *
 */

#include "thumbnail.h"

/**
 * @brief thumbnail::thumbnail
 * @param inwidth
 * @param inheight
 * @param tnailwidth
 * @param tnailheight
 */
thumbnail::thumbnail(int inwidth, int inheight, int tnailwidth, int tnailheight):
    W_in(inwidth),
    H_in(inheight),
    W_out(tnailwidth),
    H_out(tnailheight)
{
    av_register_all ();

    pYUVframe  = av_frame_alloc ();
    pRGBframe  = av_frame_alloc ();
    av_init_packet( &pktPng );

    RGBbuffer = new uint8_t[W_out * H_out * 3];

    pYUVframe->format   = AV_PIX_FMT_NV12;
    pYUVframe->width    = W_in;
    pYUVframe->height   = H_in;
    pYUVframe->linesize[0] = W_in;
    pYUVframe->linesize[1] = W_in;

    pRGBframe->format   = AV_PIX_FMT_RGB24;
    pRGBframe->width = W_out;
    pRGBframe->height = H_out;
    pRGBframe->data[0] = RGBbuffer; // RGB
    pRGBframe->linesize[0] = W_out * 3;

    sws_ctx = sws_getContext
              (
                  W_in,H_in,AV_PIX_FMT_NV12, //NV12
                  W_out,H_out,AV_PIX_FMT_RGB24, //RGB888
                  SWS_BILINEAR,nullptr,nullptr,nullptr
                  );
    if ( sws_ctx == nullptr ){
        std::cerr << "[THUMB]: SwScaler error" << std::endl;
    }

    pCodecPng = avcodec_find_encoder( AV_CODEC_ID_PNG );
    if ( pCodecPng == nullptr ){
        std::cerr << "[THUMB]: AV_CODEC_ID_PNG Find Encoder error" << std::endl;
    }

    pCodecCtxPng = avcodec_alloc_context3( pCodecPng );
    pCodecCtxPng->pix_fmt = AV_PIX_FMT_RGB24;
    pCodecCtxPng->width = W_out;
    pCodecCtxPng->height = H_out;
    pCodecCtxPng->time_base.num = pCodecCtxPng->time_base.den = 1;
    pCodecCtxPng->compression_level = 10;

    if(avcodec_open2( pCodecCtxPng, pCodecPng, nullptr ) < 0) {
        std::cerr << "[THUMB]: AV_CODEC_ID_PNG Encoder Open error" << std::endl;
    }

}

/**
 * @brief thumbnail::savepng
 * @param pngfilename
 * @param Ybuf
 * @param UVbuf
 * @return
 */
int thumbnail::savepng(const char *pngfilename, uint8_t *Ybuf, uint8_t *UVbuf)
{

    int res = 0;
    FILE *pngFile;

    if(!Ybuf || !UVbuf){
        std::cerr << "[THUMB]: Input buffer invalid" << std::endl;
        return -1;
    }

    pYUVframe->data[0]  = Ybuf;  // Y
    pYUVframe->data[1]  = UVbuf; // UV

    res = sws_scale(sws_ctx,
                    pYUVframe->data, pYUVframe->linesize, 0, H_in,
                    pRGBframe->data, pRGBframe->linesize);
    if(res <= 0){
        std::cerr << "[THUMB]: sws_scale failed" << std::endl;
        return -1;
    }

    if ( avcodec_send_frame( pCodecCtxPng, pRGBframe ) < 0 ) {
        std::cerr << "[THUMB]: avcodec_send_frame failed" << std::endl;
        return -1;
    }
    if ( avcodec_receive_packet( pCodecCtxPng, &pktPng ) < 0 ) {
        std::cerr << "[THUMB]: avcodec_receive_packet failed" << std::endl;
        return -1;
    }

    pngFile = fopen( pngfilename, "wb") ;

    if(pngFile){
        res = fwrite( pktPng.data, 1, pktPng.size, pngFile );
        fclose (pngFile);

        if(res <= 0){
            std::cerr << "[THUMB]: PNG File write failed" << std::endl;
            return -1;
        }
    }else {
        std::cerr << "[THUMB]: PNG File open failed" << std::endl;
        return -1;
    }

    return 0;
}

/**
 * @brief thumbnail::~thumbnail
 */
thumbnail::~thumbnail ()
{
    sws_freeContext(sws_ctx);
    avcodec_free_context(&pCodecCtxPng);
    av_frame_free (&pYUVframe);
    av_frame_free (&pRGBframe);
    av_packet_unref (&pktPng);

    delete[] RGBbuffer;
}
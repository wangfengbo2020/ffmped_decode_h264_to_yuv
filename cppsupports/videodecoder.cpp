#include <stdio.h>

#include "videodecoder.h"

VideoCodec::VideoCodec()
{
    printf("init VideoCodec \n");
    pAVCodecCtxDecoder = NULL;
    pAVCodecDecoder = NULL;
    pAVFrameDecoder = NULL;
    pImageConvertCtxDecoder = NULL;
    pFrameYUVDecoder = NULL;
}

VideoCodec::~VideoCodec()
{
    ;   
}


int VideoCodec::ffmpeg_init_video_decoder(AVCodecParameters *codecParameters)
{
    printf("ffmpeg_init_video_decoder 11111111 \n");
    if (!codecParameters) {
        printf("Source codec context is NULL.");   //printf需替换为printf
        return -1;
    }
    
    printf("ffmpeg_init_video_decoder 2222222 \n");
    ffmpeg_release_video_decoder();
    printf("ffmpeg_init_video_decoder 3333333 \n");
    avcodec_register_all();
    printf("ffmpeg_init_video_decoder 4444444 \n");
    
    pAVCodecDecoder = avcodec_find_decoder(codecParameters->codec_id);
    if (!pAVCodecDecoder) {
        printf("Can not find codec:%d\n", codecParameters->codec_id);
        return -2;
    }
    printf("ffmpeg_init_video_decoder 555555 \n");
    pAVCodecCtxDecoder = avcodec_alloc_context3(pAVCodecDecoder);
    if (!pAVCodecCtxDecoder) {
        printf("Failed to alloc codec context.");
        ffmpeg_release_video_decoder();
        return -3;
    }
    printf("ffmpeg_init_video_decoder 666666 \n");
    if (avcodec_parameters_to_context(pAVCodecCtxDecoder, codecParameters) < 0) {
        printf("Failed to copy avcodec parameters to codec context.");
        ffmpeg_release_video_decoder();
        return -3;
    }
 
    
    if (avcodec_open2(pAVCodecCtxDecoder, pAVCodecDecoder, NULL) < 0){
        printf("Failed to open h264 decoder");
        ffmpeg_release_video_decoder();
        return -4;
    }
    
    av_init_packet(&mAVPacketDecoder);
    
    pAVFrameDecoder = av_frame_alloc();
    pFrameYUVDecoder = av_frame_alloc();

    return 0;
}

int VideoCodec::ffmpeg_init_h264_decoder()
{
    printf("init 1111111111111111111 \n");
    avcodec_register_all();
    printf("init 2222222222222222222 \n");
    AVCodec *pAVCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!pAVCodec){
        printf("can not find H264 codec\n");
        return -1;
    }
    printf("init 33333333333333333333 \n");
 
    AVCodecContext *pAVCodecCtx = avcodec_alloc_context3(pAVCodec);
    if (pAVCodecCtx == NULL) {
        printf("Could not alloc video context!\n");
        return -2;
    }
    printf("init 44444444444444444444 \n");
    
    AVCodecParameters *codecParameters = avcodec_parameters_alloc();
    printf("init 55555555555555555555 \n");
    if (avcodec_parameters_from_context(codecParameters, pAVCodecCtx) < 0) {
        printf("Failed to copy avcodec parameters from codec context.");
        avcodec_parameters_free(&codecParameters);
        avcodec_free_context(&pAVCodecCtx);
        return -3;
    }
    
    printf("init 666666666666666666666 \n");
    int ret = ffmpeg_init_video_decoder(codecParameters);
    printf("init 777777777777777777777 \n");
    avcodec_parameters_free(&codecParameters);
    printf("init 888888888888888888888 \n");
    avcodec_free_context(&pAVCodecCtx);
    printf("init 999999999999999999999 \n");
    
    return ret;
}
 
int VideoCodec::ffmpeg_release_video_decoder() 
{
    printf("ffmpeg_release_video_decoder 11111 \n");

    if (pAVCodecCtxDecoder != NULL) {
        avcodec_free_context(&pAVCodecCtxDecoder);
        pAVCodecCtxDecoder = NULL;
    }
        printf("ffmpeg_release_video_decoder 22222 \n");

    if (pAVFrameDecoder != NULL) {
        av_packet_unref(&mAVPacketDecoder);
        av_free(pAVFrameDecoder);
        pAVFrameDecoder = NULL;
    }

    printf("ffmpeg_release_video_decoder 33333 \n");

    if (pFrameYUVDecoder) {
        av_frame_unref(pFrameYUVDecoder);
        av_free(pFrameYUVDecoder);
        pFrameYUVDecoder = NULL;
    }
    printf("ffmpeg_release_video_decoder 44444 \n");
    
    if (pImageConvertCtxDecoder) {
        sws_freeContext(pImageConvertCtxDecoder);
        pImageConvertCtxDecoder = NULL;
    }
    printf("ffmpeg_release_video_decoder 55555 \n");
        
    return 0;
}
 
int VideoCodec::ffmpeg_decode_h264(unsigned char *inbuf, int inbufSize, int *framePara, unsigned char **outRGBBuf, unsigned char *outYUVBuf)
{
    if (!pAVCodecCtxDecoder || !pAVFrameDecoder || !inbuf || inbufSize<=0 || !framePara || (!outRGBBuf && !outYUVBuf)) {
        return -1;
    }
    av_frame_unref(pAVFrameDecoder);
    av_frame_unref(pFrameYUVDecoder);
    
    framePara[0] = framePara[1] = 0;
    mAVPacketDecoder.data = inbuf;
    mAVPacketDecoder.size = inbufSize;
    
    int ret = avcodec_send_packet(pAVCodecCtxDecoder, &mAVPacketDecoder);
    if (ret == 0) {
        ret = avcodec_receive_frame(pAVCodecCtxDecoder, pAVFrameDecoder);
        if (ret == 0) {
            framePara[0] = pAVFrameDecoder->width;
            framePara[1] = pAVFrameDecoder->height;
            
            if (outYUVBuf) {
                //*outYUVBuf = (unsigned char *)pAVFrameDecoder->data;
                memcpy(outYUVBuf, pAVFrameDecoder->data, sizeof(pAVFrameDecoder->data));

                framePara[2] = pAVFrameDecoder->linesize[0];
                framePara[3] = pAVFrameDecoder->linesize[1];
                framePara[4] = pAVFrameDecoder->linesize[2];
            } else if (outRGBBuf) {
                //pFrameYUVDecoder->data[0] = outRGBBuf;
                pFrameYUVDecoder->data[1] = NULL;
                pFrameYUVDecoder->data[2] = NULL;
                pFrameYUVDecoder->data[3] = NULL;
                int linesize[4] = { pAVCodecCtxDecoder->width * 3, pAVCodecCtxDecoder->height * 3, 0, 0 };
                pImageConvertCtxDecoder = sws_getContext(pAVCodecCtxDecoder->width, pAVCodecCtxDecoder->height, 
                                                            AV_PIX_FMT_YUV420P, 
                                                            pAVCodecCtxDecoder->width, 
                                                            pAVCodecCtxDecoder->height, 
                                                            AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, 
                                                            NULL, NULL, NULL);
                sws_scale(pImageConvertCtxDecoder, (const uint8_t* const *) pAVFrameDecoder->data, pAVFrameDecoder->linesize, 0, pAVCodecCtxDecoder->height, pFrameYUVDecoder->data, linesize);
                sws_freeContext(pImageConvertCtxDecoder);
            }
            return 1;
        } else if (ret == AVERROR(EAGAIN)) {
            return 0;
        } else {
            return -1;
        }
    }
    
    return 0;
}
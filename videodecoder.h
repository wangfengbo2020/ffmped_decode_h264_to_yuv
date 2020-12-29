#include <libavcodec/avcodec.h>
 
/**
 视频流解码器初始化
 @param ctx 解码参数结构体AVCodecParameters
 @see ffmpeg_init_video_decoder，此为解码H264视频流
 @return 初始化成功返回0，否则<0
 */
int ffmpeg_init_video_decoder(AVCodecParameters *ctx);
 
/**
 H264视频流解码器初始化
 @return 初始化成功返回0，否则<0
 */
int ffmpeg_init_h264_decoder(void);


/**
 释放解码器
 @return 初始化成功返回0，否则<0
 */
int ffmpeg_release_video_decoder(void);
 
//return 0:暂未收到解码数据，-1：解码失败，1：解码成功
 
/**
 解码视频流数据
 @param inbuf 视频裸流数据
 @param inbufSize 视频裸流数据大小
 @param framePara 接收帧参数数组：{width,height,linesize1,linesiz2,linesize3}
 @param outRGBBuf 输出RGB数据(若已申请内存)
 @param outYUVBuf 输出YUV数据(若已申请内存)
 @return 成功返回解码数据帧大小，否则<=0
 */
int ffmpeg_decode_h264(unsigned char * inbuf, int inbufSize, int *framePara, unsigned char **outRGBBuf, unsigned char *outYUVBuf);
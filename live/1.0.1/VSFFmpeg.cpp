
#include "VSFFmpeg.h"
#include <stdbool.h>

// 把yuv AVFrame Data做转换
static DecoderError_e process_to_yuv_bytes(AVFrame* frame, unsigned char* buffer, int width, int height)
{

    unsigned char* src = NULL;
    unsigned char* dst = buffer;
    int            i = 0;

    if (frame == NULL || buffer == NULL) {
        return kError_InvalidParam;
    }

    if (!frame->data[0] || !frame->data[1] || !frame->data[2]) {
        return kError_InvalidParam;
    }

    for (i = 0; i < height; i++) {
        src = frame->data[0] + i * frame->linesize[0];
        memcpy(dst, src, width);
        dst += width;
    }

    for (i = 0; i < height / 2; i++) {
        src = frame->data[1] + i * frame->linesize[1];
        memcpy(dst, src, width / 2);
        dst += width / 2;
    }

    for (i = 0; i < height / 2; i++) {
        src = frame->data[2] + i * frame->linesize[2];
        memcpy(dst, src, width / 2);
        dst += width / 2;
    }
    return KError_None;
}

VSFFmpeg::VSFFmpeg() : _videoContext(NULL), _vframe(NULL), _rgbBuffer(NULL) {}

VSFFmpeg::~VSFFmpeg()
{
    if (_videoContext) {
        avcodec_free_context(&_videoContext);
        _videoContext = NULL;
    }
    if (_vframe) {
        av_frame_free(&_vframe);
        _vframe = NULL;
    }
    if (_vframeBGR) {
        av_frame_free(&_vframeBGR);
        _vframeBGR = NULL;
    }
    if (_rgbBuffer) {
        av_free(_rgbBuffer);
    }

    if (_swsContext) {
        sws_freeContext(_swsContext);
    }
}

DecoderError_e VSFFmpeg::openDecoder(enum AVCodecID codecId)
{
    AVDictionary* opts = NULL;
    if (codecId == AV_CODEC_ID_NONE) {
        return KError_FindDecoder;
    }
    av_register_all();
    AVCodec* pDecoder = avcodec_find_decoder(codecId);
    if (NULL == pDecoder) {
        return KError_FindDecoder;
    }
    _videoContext = avcodec_alloc_context3(pDecoder);
    av_dict_set(&opts, "refcounted_frames", "0", 0);
    if (avcodec_open2(_videoContext, pDecoder, NULL) < 0) {
        return KError_OpenDecoder;
    }
    _vframe = av_frame_alloc();
    if (NULL == _vframe) {
        return KError_AllocAVFrame;
    }
    _vframeBGR = av_frame_alloc();
    if (NULL == _vframeBGR) {
        return KError_AllocAVFrame;
    }
    return KError_None;
}

DecoderError_e VSFFmpeg::decoderOnepacket(unsigned char* frameBuf, int frameLen)
{
    DecoderError_e retCode = KError_None;
    AVPacket       avpkt;
    av_init_packet(&avpkt);
    avpkt.data = frameBuf;
    avpkt.size = frameLen;
    int ret = avcodec_send_packet(_videoContext, &avpkt);
    if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
        retCode = KError_FFmpegDecodec;
        goto Error;
    }
    ret = avcodec_receive_frame(_videoContext, _vframe);
    if (ret < 0) {
        retCode = KError_FFmpegDecodec;
        goto Error;
    }
    // 解码数据回调
    _mutex.lock();
    // yuv->rgb
    if (_swsContext == nullptr) {
        _swsContext = sws_getCachedContext(_swsContext, _vframe->width, _vframe->height,
                                           AVPixelFormat::AV_PIX_FMT_YUV420P, _vframe->width, _vframe->height,
                                           AVPixelFormat::AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
        _videoSize = avpicture_get_size(AV_PIX_FMT_RGB24, _videoContext->width, _videoContext->height);
        _rgbBuffer = ( uint8_t* )av_malloc(_videoSize);
        avpicture_fill(( AVPicture* )_vframeBGR, _rgbBuffer, AV_PIX_FMT_RGB24, _videoContext->width,
                       _videoContext->height);
    }
    sws_scale(_swsContext, ( const uint8_t* const* )_vframe->data, _vframe->linesize, 0, _videoContext->width,
              _vframeBGR->data, _vframeBGR->linesize);
    _mutex.unlock();

Error:
    av_packet_unref(&avpkt);
    return retCode;
}

void VSFFmpeg::getframe(unsigned char* data, int& length)
{
    if (_videoSize <= 0) {
        return;
    }
    _mutex.lock();
    if (data) {
        memcpy(data, _rgbBuffer, _videoSize);
        length = _videoSize;
    }
    _mutex.unlock();
}
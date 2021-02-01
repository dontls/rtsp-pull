#include "Onvif.h"
#include "Dump.h"
#include "wsseapi.h"
#include <assert.h>

void soap_perror(struct soap* soap, const char* str);

#define SOAP_CHECK_ERROR(result, soap, str)                    \
    do {                                                       \
        if (SOAP_OK != (result) || SOAP_OK != (soap)->error) { \
            soap_perror((soap), (str));                        \
            if (SOAP_OK == (result)) {                         \
                (result) = (soap)->error;                      \
            }                                                  \
            goto EXIT;                                         \
        }                                                      \
    } while (0)

#define SOAP_SOCK_TIMEOUT (10)

void soap_perror(struct soap* soap, const char* str)
{
    if (NULL == str) {
        printf("[soap] error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap));
    } else {
        printf("[soap] %s error: %d, %s, %s\n", str, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));
    }
    return;
}

struct soap* ONVIF_soap_new(int timeout)
{
    struct soap* soap = soap_new();  // soap��������
    if (soap != NULL) {
        soap_set_namespaces(soap, namespaces);  // ����soap��namespaces
        soap->recv_timeout = timeout;  // ���ó�ʱ������ָ��ʱ��û�����ݾ��˳���
        soap->send_timeout = timeout;
        soap->connect_timeout = timeout;

#if defined(__linux__) || defined(__linux)  // �ο�https://www.genivia.com/dev.html#client-c���޸ģ�
        soap->socket_flags = MSG_NOSIGNAL;  // To prevent connection reset errors
#endif

        soap_set_mode(soap, SOAP_C_UTFSTRING);  // ����ΪUTF-8���룬�����������OSD������
    }
    return soap;
}

void ONVIF_soap_delete(struct soap* soap)
{
    soap_destroy(soap);  // remove deserialized class instances (C++ only)
    soap_end(soap);      // Clean up deserialized data (except class instances) and temporary data
    soap_done(soap);     // Reset, close communications, and remove callbacks
    soap_free(soap);     // Reset and deallocate the context created with soap_new or soap_copy
}

/************************************************************************
**函数：make_uri_withauth
**功能：构造带有认证信息的URI地址
**参数�?
        [in]  src_uri       - 未带认证信息的URI地址
        [in]  username      - 有户�?
        [in]  password      - 密码
        [out] dest_uri      - 返回的带认证信息的URI地址
        [in]  size_dest_uri - dest_uri缓存大小
**返回�?
        0成功，非0失败
**备注
    1). 例子
    无认证信息的uri：rtsp://100.100.100.140:554/av0_0
    带认证信息的uri：rtsp://username:password@100.100.100.140:554/av0_0
************************************************************************/
int make_uri_withauth(char* src_uri, char* username, char* password, char* dest_uri, unsigned int size_dest_uri)
{
    unsigned int needBufSize = 0;
    memset(dest_uri, 0x00, size_dest_uri);
    needBufSize =
        strlen(src_uri) + strlen(username) + strlen(password) + 3;  // 检查缓存是否足够，包括�?:’和‘@’和字符串结束符
    if (size_dest_uri < needBufSize) {
        printf("[warning] dest uri buf size is not enough.\n");
        return -1;
    }

    if (0 == strlen(username) && 0 == strlen(password)) {  // 生成新的uri地址
        strcpy(dest_uri, src_uri);
    } else {
        char* p = strstr(src_uri, "//");
        if (NULL == p) {
            printf("[warning] can't found '//', src uri is: %s.\n", src_uri);
            return 1;
        }
        p += 2;
        memcpy(dest_uri, src_uri, p - src_uri);
        sprintf(dest_uri + strlen(dest_uri), "%s:%s@", username, password);
        strcat(dest_uri, p);
    }
    return 0;
}

int ONVIF_getInformation(const char* ip, const char* username, const char* password, int channel,
                         struct IPCameraInfo* info)
{

    int                               result = 0;
    struct soap*                      soap = NULL;
    struct _trt__GetProfiles          req;
    struct _trt__GetProfilesResponse  rep;
    struct tt__StreamSetup            StreamSetup;
    struct tt__Transport              Transport;
    struct _trt__GetStreamUri         trt__GetStreamUri;
    struct _trt__GetStreamUriResponse trt__GetStreamUriResponse;
    struct tt__Profile*               ttProfile;
    char                              MediaXAddr[128];
    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));
    memset(&StreamSetup, 0x00, sizeof(StreamSetup));
    memset(&Transport, 0x00, sizeof(Transport));
    memset(&trt__GetStreamUri, 0x00, sizeof(trt__GetStreamUri));
    memset(&trt__GetStreamUriResponse, 0x00, sizeof(trt__GetStreamUriResponse));
    soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT);
    if (NULL != ip && NULL != soap) {
        do {
            soap_wsse_add_UsernameTokenDigest(soap, NULL, username, password);
            //封装�?"http://10.0.97.102/onvif/media_service";
            sprintf(MediaXAddr, "%s%s%s", "http://", ip, "/onvif/media_service");
            result = soap_call___trt__GetProfiles(soap, MediaXAddr, NULL, &req, &rep);
            SOAP_CHECK_ERROR(result, soap, "GetProfiles");
            // dump_trt__GetProfilesResponse(&rep);

            if (rep.__sizeProfiles > 0 && rep.__sizeProfiles > channel) {
                ttProfile = &rep.Profiles[channel];
                if (NULL == ttProfile->token || NULL == ttProfile->VideoEncoderConfiguration) {  // 配置文件Token
                    goto EXIT;
                }
                info->profile_token = ttProfile->token;
                if (NULL != ttProfile->VideoEncoderConfiguration->token) {  // 视频编码器Token
                    info->encodec_token = ttProfile->VideoEncoderConfiguration->token;
                    ;
                }
                if (NULL != ttProfile->VideoEncoderConfiguration->Resolution) {  //
                    info->width = ttProfile->VideoEncoderConfiguration->Resolution->Width;
                    info->height = ttProfile->VideoEncoderConfiguration->Resolution->Height;
                }
                //获取RTSP的URI
                StreamSetup.Stream = tt__StreamType__RTP_Unicast;  // stream type;
                StreamSetup.Transport = &Transport;
                StreamSetup.Transport->Protocol = tt__TransportProtocol__RTSP;
                StreamSetup.Transport->Tunnel = NULL;
                trt__GetStreamUri.StreamSetup = &StreamSetup;
                trt__GetStreamUri.ProfileToken = info->profile_token;
                soap_wsse_add_UsernameTokenDigest(soap, NULL, username, password);
                result = soap_call___trt__GetStreamUri(soap, MediaXAddr, NULL, &trt__GetStreamUri,
                                                       &trt__GetStreamUriResponse);
                SOAP_CHECK_ERROR(result, soap, "GetStreamUri");

                result = make_uri_withauth(trt__GetStreamUriResponse.MediaUri->Uri, username, password, info->rtsp_uri,
                                           MAX_STREAM_URI_LEN);
                if (NULL != soap) {
                    ONVIF_soap_delete(soap);
                }
                return 0;
            }
        } while (0);
    }
EXIT:
    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }
    return -1;
}

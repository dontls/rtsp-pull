#ifndef __LIB_ONVIF_H__
#define __LIB_ONVIF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#define MAX_STREAM_URI_LEN 256

typedef struct IPCameraInfo {
    char* profile_token;
    char* encodec_token;
    int   width;
    int   height;
    char  rtsp_uri[MAX_STREAM_URI_LEN];
} IPCameraInfo_t;

/**
 * @description: 获取IPC的信息
 * @param {type}         [in]  ip            - IPC的IP
 *                       [in]  username      - 有户名
 *                       [in]  password      - 密码
 *                       [in]  channel       - 码流
 *                       [out] info          - 获取的IPC会保存在此 ipcamera_info_t 结构体中
 * @return:  0成功，非0失败
 */
int ONVIF_getInformation(const char* ip, const char* username, const char* password, int channel,
                         struct IPCameraInfo* info);
						 
#ifdef __cplusplus
}
#endif

#endif

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
}
#endif

#include <opencv2/opencv.hpp>

#define __GDB__

#ifdef __GDB__
#define gdbprintf printf
#else
#define gdbprintf
#endif

#define GDB_RED       "\033[1;32;31m"     //ERROR
#define GDB_VIOLET    "\033[1;32;35m"     //WARN
#define GDB_GREEN	  "\033[1;32;34m"     //Info
#define GDB_END 	  "\033[m"


#define MAX_TASK_QUEUE_CNT  3
#define MAX_TASK_NUM  		256

#define MAX_STREAM_URI_LEN  256


#define TASK_FACE_ID 		0x00000001

typedef unsigned char BYTE;


enum ENUM_IPC_STATE {
	IPC_MEDIA_ERROR = -1,
	IPC_MEDIA_OK = 0
};

enum ENUM_VIDEO_STATE {
	VIDEO_MEDIA_ERROR = 0,
	VIDEO_EXCEPTION,
	VIDEO_PLAY,
	VIDEO_STOP,
	VIDEO_CLOSE,
	VIDEO_AVTICE,
	VIDEO_SLEEP
};


struct IpcInformation {

	char* ip;
	char* username;
	char* password;
	int channel;
	char* profile_token;
	char* encodec_token;
	enum ENUM_IPC_STATE state;
	int width;
	int height;
	char rtsp_uri[MAX_STREAM_URI_LEN];
};


struct VideoStream {
	struct IpcInformation IPC;
    enum ENUM_VIDEO_STATE state;
};
#endif // IPC_COMMON_H

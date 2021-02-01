LIB-build := ../libOnvif.so
# 头文件路径
INCLUDES += -I./ssl/include 
INCLUDES += -I./onvif

# -DWITH_DOM -DWITH_OPENSSL 两个宏是链接openssl
# -DDEBUG 用于开启soap协议收发日志，日志存于文件RECV.log SENT.log TEST.log
CFLAGS += $(INCLUDES) -DWITH_DOM -DWITH_OPENSSL 

LDFLAGS +=  ./ssl/lib/libssl.a	 	    \
            ./ssl/lib/libcrypto.a 	    \
			-ldl

# 源文件
DIRS := onvif
SRCS += Dump.c Onvif.c

-include ../../scripts/compile.mk
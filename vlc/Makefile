include ./Makefile.inc

INCLUDE += -I /usr/local/include
INCLUDE += -I ./ -I ../

LDLIBS += -lipcdevice -lvlc -lvlccore -pthread
LDLIBS += -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc 

CFLAGS += -std=gnu++11

SRCS := Main.cpp  			\
	VAbstract.cpp         	\
	VInstance.cpp           \
	VMediaplayer.cpp        \
	VStream.cpp
	
OBJS := $(patsubst %.cpp, $(TEMPDIR)%.o, $(filter %.cpp, $(SRCS)))

PROGRAM := vlcplayer

all: $(PROGRAM) 

$(PROGRAM) : $(OBJS)

	$(CXX) -o $@ $(OBJS) $(LDLIBS) 

	
.PHONY: install 

install:
	@ sudo cp $(SHAREDLIB) /usr/lib

clean:
	rm -rf $(OBJS) $(PROGRAM)

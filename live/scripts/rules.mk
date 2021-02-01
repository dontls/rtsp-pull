# Compilation tools
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar
LD = $(CROSS_COMPILE)ld
STRIP = $(CROSS_COMPILE)strip
NM = $(CROSS_COMPILE)nm
# export CC CXX AR LD NM STRIP 

Q := @
ECHO := @echo
export Q ECHO
#-------------------------------------------------------------------------------

DIR-build ?= ./build

build ?= debug

ifeq ($(build), debug)
CFLAGS_OPT +=-Wall -g
else
CFLAGS_OPT +=-DNDEBUG -Os 
endif

#-------------------------------------------------------------------------------
# 这里定义通用的编译参数，不同项目在对应Makefile中配置
# c 编译参数
CFLAGS += -fPIC -Wdeprecated-declarations $(CFLAGS_OPT)
# c++ 编译参数
CXXFLAGS += $(CFLAGS) -std=c++11
LDFLAGS += 

#-------------------------------------------------------------------------------
#		Rules
#-------------------------------------------------------------------------------
all::

include Makefile.inc

vpath %.cpp $(CURDIR)
vpath %.c $(CURDIR)
vpath %.S $(CURDIR)

OBJS := $(addprefix $(DIR-build)/,$(obj-y))
LIBS := $(addprefix $(DIR-build)/,$(lib-y))

-include $(OBJS:.o=.d)

PHONY := all 
all:: $(LIB-build) $(APP-build)

$(DIR-build):
	$(Q)mkdir -p $(DIR-build)

$(APP-build):: $(OBJS) $(LIBS)
	$(Q)$(ECHO) LINK $@
	$(Q)$(CXX) -o $@ $(OBJS) $(LIBS) $(LDFLAGS) 

$(LIB-build):: $(OBJS) $(LIBS)
	$(Q)$(ECHO) LINK $@
	$(Q)$(CXX) -fPIC -shared -o $@ $(OBJS) $(LIBS) $(LDFLAGS) 

#--------------------------------------------------------------------
$(DIR-build)/%.d: %.cpp
	$(Q)mkdir -p $(dir $@)
	$(ECHO) DEP $<
	$(Q)$(CXX) $(CXXFLAGS) $(INCLUDES) -MM $< -MT $(basename $@).o -o $(basename $@).d

$(DIR-build)/%.o: %.cpp
	$(Q)mkdir -p $(dir $@)
	$(ECHO) CXX $<
	$(Q)$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(DIR-build)/%.d: %.c
	$(Q)mkdir -p $(dir $@)
	$(ECHO) DEP $<
	$(Q)$(CC) $(CFLAGS) $(INCLUDES) -MM $< -MT $(basename $@).o -o $(basename $@).d

$(DIR-build)/%.o: %.c
	$(Q)mkdir -p $(dir $@)
	$(ECHO) CC $<
	$(Q)$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

PHONY += clean
clean:
	$(Q)@rm -rf $(DIR-build)

.PHONY: $(PHONY)

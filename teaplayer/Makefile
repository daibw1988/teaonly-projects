AR = ar
CC = g++

TALK_INC_DIR =  .
CFLAGS = -Wall -Wno-non-virtual-dtor -Wno-ctor-dtor-privacy -Wno-switch -fno-rtti -fno-exceptions -fvisibility=hidden -ffunction-sections -fdata-sections -O0 -DLOGGING=1 -DPOSIX -DLINUX -D_DEBUG -g -I$(TALK_INC_DIR)

LIB_OBJS_ =
include lib_build.mk
LIB_OBJS = $(patsubst %, %, $(LIB_OBJS_))
LIB_TARGET = libjingle.a

OBJS_ =
DEPS_ =
include app_build.mk
DEPS = $(patsubst %, %, $(DEPS_))
OBJS = $(patsubst %, %, $(OBJS_))
TARGET = tea

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 
%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $< 
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 


$(TARGET): $(OBJS) $(LIB_TARGET)
	$(CC) -g $(OBJS) -o $@ -L./ -ljingle -lm -lpthread -lavformat -lavutil -lavcodec

$(LIB_TARGET): $(LIB_OBJS)
	$(AR) -cr $@ $(LIB_OBJS)

.PHONY: clean all
all: $(TARGET)
clean:
	rm -f $(TARGET)
	rm -f $(LIB_TARGET)
	rm -f $(LIB_OBJS) $(OBJS)

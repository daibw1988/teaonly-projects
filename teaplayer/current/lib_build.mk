SOURCE_DIR = talk/base
LIB_OBJS_ += $(SOURCE_DIR)/common.o \
                   $(SOURCE_DIR)/logging.o \
                   $(SOURCE_DIR)/time.o \
                   $(SOURCE_DIR)/stringencode.o \
                   $(SOURCE_DIR)/stream.o \
                   $(SOURCE_DIR)/asyncfile.o \
                   $(SOURCE_DIR)/asyncsocket.o \
                   $(SOURCE_DIR)/nethelpers.o \
                   $(SOURCE_DIR)/ipaddress.o \
                   $(SOURCE_DIR)/socketaddress.o \
                   $(SOURCE_DIR)/physicalsocketserver.o \
                   $(SOURCE_DIR)/signalthread.o \
                   $(SOURCE_DIR)/messagehandler.o \
				   $(SOURCE_DIR)/messagequeue.o \
				   $(SOURCE_DIR)/thread.o 

                  

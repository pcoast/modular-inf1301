SRC := MATRIZ.c \
       TESTMAT.C \
       LISTA.C

APP := testmat
APP_WIN := testmat.exe
LIB := ../arcabouc/Produto/ArcaboucoTeste.a
LIB_WIN := ../arcabouc/Produto/ArcaboucoTeste.lib
HEADERS := ../arcabouc/Fontes
OBJ_PATH := Build
OBJ_PATH_WIN := Build-win

HEADERS := $(addprefix -I ,$(HEADERS))
CFLAGS := -x c -g -Wall $(HEADERS) -std=c99 -Wno-comment
LDFLAGS := -g -Wall

CC_WIN := i686-w64-mingw32-gcc
AR_WIN := i686-w64-mingw32-ar
CFLAGS_WIN := $(CFLAGS) -fexec-charset="ISO-8859-1" -finput-charset="UTF-8"
LDFLAGS_WIN := $(LDFLAGS)

######################################

OBJ := $(SRC:.c=.o)
OBJ_P := $(addprefix $(OBJ_PATH)/,$(OBJ))
OBJ_WIN := $(SRC:.c=.obj)
OBJ_P_WIN := $(addprefix $(OBJ_PATH_WIN)/,$(OBJ_WIN))

AUTODEPS := $(patsubst %.c,$(OBJ_PATH)/%.d,$(SRC))
DF = $(OBJ_PATH)/$(*F)

all: $(OBJ_P)
	$(CC) $(LDFLAGS) $(OBJ_P) $(LIB) -o $(APP)

win: $(OBJ_P_WIN)
	$(CC_WIN) $(LDFAGS_WIN) $(OBJ_P_WIN) $(LIB_WIN) -o $(APP_WIN)

clean:
	rm $(OBJ_P) $(APP) $(AUTODEPS) $(APP_WIN) $(OBJ_P_WIN)

$(OBJ_PATH)/%.o: %.c
	@mkdir -p $(OBJ_PATH)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_PATH_WIN)/%.obj: %.c
	@mkdir -p $(OBJ_PATH_WIN)
	$(CC_WIN) -c $(CFLAGS_WIN) $< -o $@

$(OBJ_PATH)/%.d: %.c
	@mkdir -p $(OBJ_PATH)
#	$(CC) $(CFLAGS) -MM -MP -MT $(DF).o -MT $(DF).d $< > $(DF).d
	$(CC) $(CFLAGS) -MM -MP -MT $(DF).o $< > $(DF).d

-include $(AUTODEPS)
.PHONY: clean

SOURCES = $(wildcard $(SRC_DIR)/../common/*.c) $(wildcard $(SRC_DIR)/*.c)
TMP_OBJECTS = $(subst $(SRC_DIR)/../common,$(OBJ_DIR),$(SOURCES:.c=.o))
OBJECTS = $(subst $(SRC_DIR),$(OBJ_DIR),$(TMP_OBJECTS:.c=.o))

INCLUDES += -I$(SRC_DIR)/..  -I$(SRC_DIR)/../common

all: $(EXECUTABLE) strip
ifndef ANT_TASK
	@echo "Please use ant to build the project!"
	@exit 1
endif

strip:
ifeq ($(DEBUG), true)
	@echo "Debug enabled: skipping object file symbol stripping."
else
	$(STRIP) $(STRIPARGS) $(EXECUTABLE)
endif

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) ${LIBS} -o $@

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

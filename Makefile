SOURCES = $(wildcard $(SRC_DIR)/../common/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(subst $(SRC_DIR),$(OBJ_DIR),$(SOURCES:.c=.o))
INCLUDES += -I$(abspath $(SRC_DIR)/../common)

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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

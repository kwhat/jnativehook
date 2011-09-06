ifndef ANT_TASK
	die "Please use ant to build the project!"
endif

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(subst $(SRC_DIR),$(OBJ_DIR),$(SOURCES:.c=.o))
INCLUDES += -I$(SRC_DIR)/..

all: $(EXECUTABLE) strip

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

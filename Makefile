SHELL = /bin/sh

SRC_DIR = src/org/jnativehook/c_source/unix
OBJ_DIR = bin/obj

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(subst $(SRC_DIR),$(OBJ_DIR),$(SOURCES:.c=.o))

EXECUTABLE = JNativeHook
INCLUDES = -I$(JAVA_INCLUDE)
for dir in $(JAVA_INCLUDE); do \
	INCLUDES += -I$$dir; \
done

INCLUDES += -I$(SRC_DIR)/..

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -Wall -O3 -fno-strict-aliasing -shared $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -Wall -O3 -fno-strict-aliasing -shared $(CFLAGS) $(INCLUDES) $< -o $@

ifeq ($(patsubst i%86,i586,$(MARCH)),i586)
	MARCH=i586
	BITS=32
else
	ifeq ($(MARCH),x86_64)
		
	else
		die "Cannot determine system architecture!"
	endif
endif
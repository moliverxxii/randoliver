SOURCE_DIR = sources
HEADER_DIR = headers
OBJECT_DIR = objects
DEPENDENCY_DIR = dependencies

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
HEADERS = $(SOURCES:$(SOURCE_DIR)/%.c=$(HEADER_DIR)/%.h)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)
DEPENDENCIES = $(SOURCES:$(SOURCE_DIR)/%.c=$(DEPENDENCY_DIR)/%.d)
DIRS = $(OBJECT_DIR) $(DEPENDENCY_DIR)

OPTIMISE = 3
CC_FLAGS = -c -g -pedantic -Wall -Wextra -Wno-strict-prototypes -O$(OPTIMISE) $(SYSTEM) -I$(HEADER_DIR) $(SANITIZE)
DEPENDENCY_FLAGS = -MMD
CC = gcc
PROJECT = randoliver

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) -o $@ $^ $(SANITIZE) -lm

$(OBJECTS): $(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c | $(OBJECT_DIR) $(DEPENDENCY_DIR)
	$(CC) $(CC_FLAGS) $(DEPENDENCY_FLAGS) -MF $(patsubst $(@D)%.o,$(DEPENDENCY_DIR)%.d,$@) -o $@ $< 

-include $(DEPENDENCIES)

$(DIRS):
	mkdir -p $@

clean:
	rm -fr $(OBJECT_DIR) $(DEPENDENCY_DIR) $(PROJECT)
	
rebuild: clean all

debug: SANITIZE = -fsanitize=address
debug: OPTIMISE = 0
#debug: CC = $(shell /usr/local/bin/brew --prefix llvm)/bin/clang
debug: rebuild
debug: CC_FLAGS += -DNDEBUG

#ANALYZER = $(shell /usr/local/bin/brew --prefix llvm)/bin/scan-build
ANALYZER = scan-build

analysis: clean
	$(ANALYZER) -o $(PROJECT)-analysis make $(PROJECT)

.PHONY: clean analysis

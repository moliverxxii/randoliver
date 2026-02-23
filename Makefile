SOURCE_DIR = sources
HEADER_DIR = headers
OBJECT_DIR = objects
DEPENDENCY_DIR = dependencies

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
HEADERS = $(SOURCES:$(SOURCE_DIR)/%.c=$(HEADER_DIR)/%.h)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)
DEPENDENCIES = $(SOURCES:$(SOURCE_DIR)/%.c=$(DEPENDENCY_DIR)/%.d)
DIRS = $(OBJECT_DIR) $(DEPENDENCY_DIR)
#SYSTEM := -D__BSD_VISIBLE
SYSTEM = -D_POSIX_C_SOURCE=200112L

#CC_FLAGS = -std=c99 -Wall -Wextra -pedantic -D$(SYSTEM) -g -I$(HEADER_DIR)
CC_FLAGS = -Wall -Wextra -pedantic -O1 $(SYSTEM) -g -I$(HEADER_DIR) $(SANITIZE)
DEPENDENCY_FLAGS = -MMD
CC = clang
PROJECT = randoliver

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) -o $@ $^ $(SANITIZE) -lm
	
$(OBJECTS): $(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c | $(OBJECT_DIR) $(DEPENDENCY_DIR)
	$(CC) $(CC_FLAGS) $(DEPENDENCY_FLAGS) -MF $(patsubst $(@D)%.o,$(DEPENDENCY_DIR)%.d, $@) -c -o $@ $< 

-include $(DEPENDENCIES)

$(DIRS):
	mkdir -p $@

clean:
	rm -fr $(OBJECT_DIR) $(DEPENDENCY_DIR) $(PROJECT)
	
rebuild: clean all

debug: SANITIZE = -fsanitize=address
debug: CC = $(shell /usr/local/bin/brew --prefix llvm)/bin/clang
debug: rebuild

ANALYZER = $(shell /usr/local/bin/brew --prefix llvm)/bin/scan-build
#ANALYZER = scan-build

analysis: clean
	$(ANALYZER) -o $(PROJECT)-analysis make $(PROJECT)

.PHONY: clean analysis

SOURCE_DIR = sources
HEADER_DIR = headers
OBJECT_DIR = objects

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
HEADERS = $(SOURCES:$(SOURCE_DIR)/%.c=$(HEADER_DIR)/%.h)
HEADERS = $(wildcard src/*.h)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)
CC_FLAGS = -Wall -g
CC = gcc
PROJECT = bmp-generator

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) -o $@ $^ -lm


$(OBJECTS): $(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADER_DIR)/main.h | $(OBJECT_DIR)
	$(CC) $(CC_FLAGS) -I$(HEADER_DIR) -c -o $@ $< 

$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)

$(HEADER_DIR)/main.h: $(HEADERS)
	touch $@

clean:
	rm -fr $(OBJECT_DIR) $(PROJECT) 
	
ANALYZER = $(shell /usr/local/bin/brew --prefix llvm)/bin/scan-build
analysis:
	$(ANALYZER) -v -v -v -o $(PROJECT)-analysis make $(PROJECT)

.PHONY: clean analysis

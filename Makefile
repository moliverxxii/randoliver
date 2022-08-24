SOURCE_DIR = sources
HEADER_DIR = headers
OBJECT_DIR = objects

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
HEADERS = $(SOURCES:$(SOURCE_DIR)/%.c=$(HEADER_DIR)/%.h)
HEADERS = $(wildcard src/*.h)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)
CC = gcc
PROJECT = bmp-generator

all: $(PROJECT)

bmp-generator: $(OBJECTS)
	$(CC) -o $@ $^ -lm

$(OBJECTS): $(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADER_DIR)/main.h
	mkdir -p $(OBJECT_DIR)
	$(CC) -c -o $@ -I$(HEADER_DIR)  $< 

$(HEADER_DIR)/main.h: $(HEADERS)
	touch $@

clean:
	rm -fr $(OBJECT_DIR) $(PROJECT) 
	
.PHONY: clean

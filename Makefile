# Main files
FILE = main.c
TARGET := out

# Include other modules here
SRCS ?= constants.c

FLAGS := -g

all: build run

build:
	@gcc -o $(TARGET) $(FILE) $(SRCS)

run:
	@./$(TARGET)

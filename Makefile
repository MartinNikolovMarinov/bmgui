CC            = g++
COMMON_CFLAGS = -ggdb -std=c++17 `pkg-config --cflags sdl2` -Wno-unknown-pragmas
CFLAGS        = $(COMMON_CFLAGS) -O0 -g -Wall -Wno-unused
PROD_CFLAGS   = $(COMMON_CFLAGS) -O3
DEBUG_DEFINES = -DDEBUG=1 -DBACKEND=SDL3
PROD_DEFINES  = -DDEBUG=0 -DBACKEND=SDL3
SRC           = main.cpp # $(shell find ./src/ -name "*.cpp")
OUT_DIR       = build
BIN_NAME      = main
LIBS          = `pkg-config --libs sdl2` `pkg-config --libs SDL2_ttf` -lm
# HEADERS       = -I/usr/include/SDL2

.PHONY: help
help:
	@echo Supported targets:
	@cat $(MAKEFILE_LIST) | grep -e "^[\.a-zA-Z0-9_-]*: *.*## *" | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-35s\033[0m %s\n", $$1, $$2}'

.DEFAULT_GOAL := help

.PHONY: build
build: ## Build the project in the build folder. Creates ./build folder if it does not exist.
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) $(DEBUG_DEFINES) $(HEADERS) -o $(OUT_DIR)/$(BIN_NAME) $(SRC) $(LIBS)

.PHONY: build_prod
build_prod: clean ## Same as build, but optimizes aggresivly and generates no debug information.
	mkdir -p $(OUT_DIR)
	$(CC) $(PROD_CFLAGS) $(PROD_ENV) $(HEADERS) -o $(OUT_DIR)/$(BIN_NAME) $(SRC) $(LIBS)

.PHONY: clean
clean: ## Deletes the build folder.
	rm -rf $(OUT_DIR)
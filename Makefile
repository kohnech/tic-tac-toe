## Project
COMPONENT_NAME ?= tic-tac-toe
export PROJ_ROOT := $(CURDIR)
SUBDIRS = tests
SRCDIR = $(PROJ_ROOT)/src
BUILDDIR = $(PROJ_ROOT)/build
TOOLSDIR = $(PROJ_ROOT)/tools
3RDPARTYDIR = $(PROJ_ROOT)/3rd-party

## 3rd-party settings
SDL_VERSION=2.0.8
SDL_PATH = $(3RDPARTYDIR)/SDL2-$(SDL_VERSION)
SDL_IMAGE_VERSION=2.0.3
SDL_IMAGE_PATH =$(3RDPARTYDIR)/SDL2_image-$(SDL_IMAGE_VERSION)
SDL_TTF_PATH =$(3RDPARTYDIR)/SDL2_ttf-2.0.14
SDL_FREETYPE_PATH =$(3RDPARTYDIR)/freetype-2.4.10

INCLUDE_DIRS += -I$(PROJ_ROOT)/inc \
				-I$(SDL_PATH)/include \
				-I$(SDL_PATH)/include/SDL2 \
				-I$(SDL_IMAGE_PATH)/include \
				-I$(SDL_TTF_PATH)/include \
				-I$(SDL_FREETYPE_PATH)/include \

## Libs dynamical linkage
LIBS = -lSDL2 -lSDL2main -lSDL2_image -lsndio -lSDL2_ttf -lfreetype
LIBS_PATH = -L$(BUILDDIR) -L$(SDL_PATH)/lib -L$(SDL_IMAGE_PATH)/lib -L$(3RDPARTYDIR)/sndio -L$(SDL_TTF_PATH)/lib -L$(SDL_FREETYPE_PATH)/lib

## Libs statial linkage (make monolit release for distribution)
#LIBS_STATIC = $(SDL_PATH)/lib/libSDL2.a \
#		      $(SDL_PATH)/lib/libSDL2main.a \
#		      $(SDL_PATH)/lib/libdl.a \
#			  $(SDL_IMAGE_PATH)/lib/libSDL2_image.a
#$(info LIBS_STATIC:$(LIBS_STATIC))

## Libs 3rd-party dependencies
export LD_LIBRARY_PATH=$(SDL_PATH)/lib:$(SDL_IMAGE_PATH)/lib:$(SDL_TTF_PATH)/lib:$(3RDPARTYDIR)/sndio:$LD_LIBRARY_PATH
$(info LD_LIBRARY_PATH: $(LD_LIBRARY_PATH))

## Compiler
BUILD_TYPE ?= DEBUG
CXX = g++
STATIC = lib$(COMPONENT_NAME).a
DYNAMIC = lib$(COMPONENT_NAME).so
CXXFLAGS = -Wall -Winline -Werror -pipe -std=c++11 -fPIC
#LDFLAGS = -static-libgcc -Wl,-Bstatic
#LDFLAGS = -pthread
ifeq ($(BUILD_TYPE),DEBUG)
	CXXFLAGS += -g -O0
else ifeq ($(BUILD_TYPE),RELEASE)
	CXXFLAGS += -O3
endif

## Machine
CORES ?= $(shell nproc)
MAKEFLAGS+="-j $(CORES)"

## Sources
SRCS = 	CApp.cpp \
		CSurface.cpp \
		CEvent.cpp

HDRS = 	inc/CApp.h \
		inc/CSurface.h \
		inc/CEvent.h

OBJS = $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS))

## Make targets
.PHONY: all clean run

help:
	@echo
	@echo '  all                   - build and create main executable.'
	@echo '  lint                  - run clang formating for c++'
	@echo '  clang-format          - run clang-format on c++ files following rules specified in .clang-format.'
	@echo '  run                   - run main executable.'
	@echo '  clean                 - clean project.'
	@echo

all: $(BUILDDIR) $(BUILDDIR)/$(COMPONENT_NAME)

run: $(BUILDDIR)/$(COMPONENT_NAME)
	$(BUILDDIR)/$(COMPONENT_NAME)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(COMPONENT_NAME): $(BUILDDIR)/main.o $(OBJS) $(HDRS)
	$(CXX) -o $@ $(BUILDDIR)/main.o $(OBJS) $(LIBS_PATH) $(LIBS)

$(BUILDDIR)/main.o: $(SRCDIR)/main.cpp $(HDRS)
	$(CXX) -o $@ $(INCLUDE_DIRS) -c $(CXXFLAGS) $(SRCDIR)/main.cpp

$(OBJS): $(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	@echo [Compile] $<
	@$(CXX) $(INCLUDE_DIRS) -c $(CXXFLAGS) $< -o $@

$(BUILDDIR)/$(STATIC): $(OBJS)
	@echo "[Link (Static)]"
	@ar rcs $@ $^

lint: clang-format

clang-format:
	find ./inc ./src -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\)' -exec clang-format-5.0 -style=file -i {} \;

clean:
	rm -f $(OBJS)
	rm -f $(BUILDDIR)/$(COMPONENT_NAME)
	rm -f $(BUILDDIR)/main.o
	rm -f $(BUILDDIR)/$(STATIC)

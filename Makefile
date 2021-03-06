#variables de compilation
SHELL := /bin/sh
CC    := gcc
CXX   := g++
CFLAGS   += -O3 -flto -Wall -w -pipe -std=gnu90 -ffast-math
CXXFLAGS += -O3 -flto -Wall -w -pipe -std=gnu++98 -fabi-version=2 -ffast-math
TARGET   := borntro-by-insolitdust

#variable de nettoyage
RM_F := rm -f
#variables d'installation
INSTALL := install
INSTALL_DIR     := $(INSTALL) -p -d -o root -g root  -m  755
INSTALL_PROGRAM := $(INSTALL) -p    -o root -g root  -m  755

prefix          := /usr
exec_prefix     := $(prefix)
bindir          := $(prefix)/bin

#compilation en paralelle si disponible
ifneq (,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
 NUMJOBS = $(patsubst parallel=%,%,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
 MAKEFLAGS += -j$(NUMJOBS)
endif

export

all: $(TARGET)

 $(TARGET): main.cc xmodelib.cc texturelib.cpp idscroller.cc timerlib.cpp
	@echo Compiling Insolit Dust BoRnTrO
	g++ -o  $(TARGET) $(CXXFLAGS) main.cc xmodelib.cc texturelib.cpp idscroller.cc timerlib.cpp $(shell pkgconf sdl SDL_mixer --libs)
				
	@echo Build finished. Run ./borntro

clean:
	-@$(RM_F) $(TARGET)

install: $(TARGET)
	$(INSTALL_DIR) $(DESTDIR)$(bindir)
	-@$(RM_F) $(DESTDIR)$(bindir)/$(TARGET)
	$(INSTALL_PROGRAM) $(TARGET) $(DESTDIR)$(bindir)

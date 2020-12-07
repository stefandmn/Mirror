SHELL:=/bin/bash
CXXFLAGS = -Wall -std=c++11 -O3 -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM
LIBFLAGS = -lbrcmEGL -lbrcmGLESv2 -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lvncserver -lconfig++
INC_PATHS = -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
LIB_PATHS = -L/opt/vc/lib/


SYSDIR = sys
OBJDIR = obj
OUTDIR = out
TARGET = mirror
SOURCES = mirror.cpp UFile.cpp Resource.cpp Display.cpp Keyboard.cpp Mouse.cpp VNCServer.cpp
OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))


$(OBJDIR)/$(TARGET): init $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INC_PATHS) -o $@ $(OBJECTS) $(LIB_PATHS) $(LIBFLAGS)
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_PATHS) -c -o $@ $<
init:
ifneq ($(shell [[ -d ./$(OBJDIR) ]] && echo "yes"),yes)
	mkdir ./$(OBJDIR)
endif


clean:
	rm -rf $(OBJDIR)
	rm -rf $(OUTDIR)


install: $(OBJDIR)/$(TARGET)
	mkdir -p $(OUTDIR)/usr/bin
	mkdir -p $(OUTDIR)/usr/lib
	mkdir -p $(OUTDIR)/etc
	cp -rf $(OBJDIR)/$(TARGET) $(OUTDIR)/usr/bin
ifeq ($(shell [[ -d ./$(SYSDIR) ]] && echo "yes"),yes)
	cp -rf $(SYSDIR)/etc/* $(OUTDIR)/etc/
	cp -rf $(SYSDIR)/usr/lib/* $(OUTDIR)/usr/lib/
else
ifeq ($(shell [[ -d ../$(SYSDIR) ]] && echo "yes"),yes)
	cp -rf ../$(SYSDIR)/etc/* $(OUTDIR)/etc/
	cp -rf ../$(SYSDIR)/usr/lib/* $(OUTDIR)/usr/lib/
endif
endif
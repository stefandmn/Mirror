CXX = g++
CXXFLAGS = -Wall -std=c++11 -O3 -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM

INCLUDES = -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
LIB_PATHS = -L/opt/vc/lib/
LIBS = -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lvncserver -lconfig++

PREFIX=/usr
BINDIR=${PREFIX}/bin

SOURCES = mirror.cpp \
		UFile.cpp \
		Resource.cpp \
		Display.cpp \
		Keyboard.cpp \
		Mouse.cpp \
		VNCServer.cpp

OBJS = $(SOURCES:.cpp=.o)

MAIN = mirror

all:	$(MAIN)

$(MAIN): $(OBJS)
		 $(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIB_PATHS) $(LIBS)

.cpp.o:
		$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean: 
		$(RM) *.o *~ $(MAIN)

install: $(MAIN)
	mkdir -p $(BINDIR)
	cp -rf $(MAIN) $(BINDIR)/$(MAIN)


CXXFLAGS = -Wall -std=c++11 -O3 -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM
LIBFLAGS = -lbrcmEGL -lbrcmGLESv2 -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lvncserver -lconfig++
INC_PATHS = -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
LIB_PATHS = -L/opt/vc/lib/

SOURCES = mirror.cpp \
		UFile.cpp \
		Resource.cpp \
		Display.cpp \
		Keyboard.cpp \
		Mouse.cpp \
		VNCServer.cpp

OBJS = $(SOURCES:.cpp=.o)

MAIN = mirror

$(MAIN): $(OBJS)
		 $(CXX) $(CXXFLAGS) $(INC_PATHS) -o $(MAIN) $(OBJS) $(LIB_PATHS) $(LIBFLAGS)

.cpp.o:
		$(CXX) $(CXXFLAGS) $(INC_PATHS) -c $< -o $@

clean: 
		$(RM) *.o *~ $(MAIN)



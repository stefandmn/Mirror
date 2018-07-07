# Clue Mirror

**Mirror** is a particular VNC server for Raspberry Pi devices
allowing to share graphical screens described by X server, 
frame buffer and OpenGL ES technologies. **Mirror** is an
optional module of **Clue OS**, providing console sharing over
frame buffer and media player screen sharing and I/O handling 
over GLES. **Mirror** runs as a system service and a specific 
configuration might be stored and use in _/opt/clue/etc/mirror.conf_. 
VNC server instance can be started and run with the following settings:

 - `-c` or `--config-file=FILE` - use the specified configuration file
 - `-d` or `--downscale` - downscales the screen to a quarter in vnc
 - `-f` or `--fullscreen` - always runs fullscreen mode
 - `-l` or `--localhost` - only listens to local ports
 - `-m` or `--multi-threaded` - runs vnc in a separate thread
 - `-p` or `--port=PORT` - makes vnc available on the speficied port
 - `-P` or `--password=PASSWORD` - protects the session with PASSWORD
 - `-s` or `--screen=SCREEN` - opens the specified screen number
 - `-t` or `--frame-rate=RATE`- sets the target frame rate, default is 15
 - `-v` or `--vnc-params` - parameters to send to libvncserver
 - `--help` - displays help text and exit

Using any compatible VNC client installed on your workstation you can connect 
using RPi IP address and standard VNC port (or the custom one - in case it is 
specified through the configuration or command line) and then you can display 
the remote content and also you can control it using standard I/O (remote 
keyboard and mouse).

The main interest of VNC is that it allows to take control of a remote machine, 
while displaying the screen of it. So you can see in real time what is happening
on your Raspberry Pi, without having to plug it into a monitor!


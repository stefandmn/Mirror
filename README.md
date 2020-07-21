# Mirror

**Mirror** is a particular VNC server, designed especially for 
`Raspberry Pi` devices, allowing to share virtual desktop environment,
as well as **Kodi** graphical console.

**Mirror** is a fully functional linux program, being deployable on any 
linux system, dedicated for **Clue** environment when you don't have a 
monitor attached to your device, providing console sharing over 
frame buffer and I/O handling over GLES for **Kodi** media player. 
**Mirror** runs as a system service and a specific configuration might be 
stored and use in `/etc/mirror.conf` configuration file. VNC server instance 
can be started and run with the following settings:

 - `-c` or `--config-file=FILE` - use the specified configuration file
 - `-d` or `--downscale` - downscales the screen to a quarter in vnc
 - `-f` or `--fullscreen` - always runs fullscreen mode
 - `-l` or `--localhost` - only listens to local ports
 - `-m` or `--multi-threaded` - runs vnc in a separate thread
 - `-p` or `--port=PORT` - makes vnc available on the specified port
 - `-P` or `--password=PASSWORD` - protects the session with PASSWORD
 - `-s` or `--screen=SCREEN` - opens the specified screen number
 - `-t` or `--framerate=RATE`- sets the target framerate, default is 15
 - `-v` or `--vnc-params` - parameters to send to libvncserver
 - `--help` - displays help text and exit

Using any compatible VNC client installed on your workstation you can connect 
using RPi IP address and standard VNC port (or the custom one - in case it is 
specified through the configuration or command line) and then you can display 
the remote content and also you can control it using standard I/O (remote 
keyboard and mouse).

The main goal of this VNC service is to share the remote virtual screen and to 
allow to control to the remote machine while displaying the screen. With this 
tool installed on your Raspberry Pi device you can see in real time what is 
happening on your RPi system without having plug in into a monitor or having a 
live view what is happening on the remote device without to have access to the 
connected screen!


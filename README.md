# Mirror

**Mirror** is a particular VNC server, designed especially for 
`Raspberry Pi` devices and **Clue** OS, allowing to share virtual 
desktop environments.

**Mirror** is a fully functional linux program, being deployable on any 
linux system, but dedicated for **Clue** environment when want to test or 
to see the console but you don't have a monitor/TV attached to your device, 
providing console sharing over frame buffer and I/O handling through GLES. 
**Mirror** runs as a system service and a specific configuration might be 
stored and use in `/etc/mirror.conf` configuration file. VNC server instance 
can be started and run with the following settings:

 - `-c` or `--config-file=FILE` - use the specified configuration file
 - `-d` or `--downscale` - downscales the screen to a quarter in vnc
 - `-f` or `--fullscreen` - always runs fullscreen mode
 - `-r` or `--relative` - applies relative mouse movements
 - `-a` or `--absolute` - applies absolute mouse movements
 - `-l` or `--localhost` - only listens to local ports
 - `-m` or `--multi-threaded` - runs vnc in a separate thread
 - `-p` or `--port=PORT` - makes vnc available on the specified port
 - `-P` or `--password=PASSWORD` - protects the session with PASSWORD
 - `-s` or `--screen=SCREEN` - opens the specified screen number
 - `-t` or `--framerate=RATE`- sets the target framerate, default is 15
 - `-v` or `--vnc-params` - parameters to send to libvncserver
 - `--help` - displays help text and exit

As a particularity of **Clue** environment, the service environment is 
customized in order to check first into `$HOME/.cache/services/mirror.conf` 
user home location and, in case this one is not found default configuration 
file in `/etc/mirror.conf`. The service is starting only when the local 
file system is mounted and when the network services are up and running.

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


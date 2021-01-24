
/**
 * Mirror - [member of Clue Media Experience]
 * mirror.cpp - implementation of main logic of Mirror VNC server
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <sstream>

#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <getopt.h>

#include <libconfig.h++>

#include "Exception.h"
#include "UFile.h"
#include "Resource.h"
#include "Display.h"
#include "VNCServer.h"


#define BPP      2

extern bool terminate;

struct ConfigData
{
	uint32_t screen = 0;
	bool relative = false;
	std::string password;
	int port = 0;
	bool downscale = false;
	bool unsafe = false;
	bool fullscreen = false;
	bool multiThreaded = false;
	bool localhost = false;
	int frameRate = 15;
	std::string configFile;
	std::string vncParams;
};

void GetConfigData(int argc, char *argv[], ConfigData& configData);
void GetCommandLineConfigData(int argc, char *argv[], ConfigData& configData);
bool ReadConfigFile(const char *programName, const std::string& configFile, ConfigData& configData);
bool TryReadConfigFile(libconfig::Config& config, const std::string& file);
void usage(const char *programName);

void sig_handler(int signo)
{
	terminate = true;
}

int main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;

	try
	{
		if (signal(SIGINT, sig_handler) == SIG_ERR)
		{
			throw Exception( "Error setting sighandler");
		}
		
		if (signal(SIGTERM, sig_handler) == SIG_ERR) 
		{
			throw Exception("Error setting sighandler");
		}

		ConfigData configData;
		GetConfigData(argc, argv, configData);

		std::cerr <<
			"Running mirror vnc server with the following settings\n"
			"  framerate        = " << configData.frameRate << "\n"
			"  downscale        = " << (configData.downscale ? "true" : "false") << "\n"
			"  fullscreen       = " << (configData.fullscreen ? "true" : "false") << "\n"
			"  localhost        = " << (configData.localhost ? "true" : "false") << "\n"
			"  port             = " << configData.port << "\n"
			"  password         = " << (configData.password.length() ? "******" : "") << "\n"
			"  relative         = " << (configData.relative ? "true" : "false") << "\n"
			"  screen           = " << configData.screen << "\n"
			"  unsafe           = " << (configData.unsafe ? "true" : "false") << "\n"
			"  multi-threaded   = " << (configData.multiThreaded ? "true" : "false") << "\n"
			"  vnc-params       = " << configData.vncParams << "\n";

		VNCServer vncServer(BPP, configData.frameRate);
		vncServer.Run(configData.port, configData.password, configData.screen,
									configData.relative, !configData.unsafe, !configData.fullscreen,
									configData.multiThreaded, configData.downscale,
									configData.localhost,
									configData.vncParams);
	}
	catch (HelpException) {
		usage(argv[0]);
	}
	catch (ParamException) {
		std::cerr << "Try '" << argv[0] << "' --help for more information.\n";
		ret = EXIT_FAILURE;
	}
	catch (Exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
		ret = EXIT_FAILURE;
	}

	return ret;
}

void GetConfigData(int argc, char *argv[], ConfigData& configData)
{
	ConfigData configDataTemp;
	GetCommandLineConfigData(argc, argv, configDataTemp);
	if (ReadConfigFile(argv[0], configDataTemp.configFile, configData))
		GetCommandLineConfigData(argc, argv, configData);
	else
		configData = configDataTemp;
}

void GetCommandLineConfigData(int argc, char *argv[], ConfigData& configData)
{
	static struct option long_options[] = 
	{
		{ "relative", no_argument, nullptr, 'r' },
		{ "absolute", no_argument, nullptr, 'a' },
		{ "config-file", required_argument, nullptr, 'c' },
		{ "downscale", no_argument, nullptr, 'd' },
		{ "unsafe", no_argument, nullptr, 'u' },
		{ "fullscreen", no_argument, nullptr, 'f' },
		{ "localhost", no_argument, nullptr, 'l' },
		{ "multi-threaded", no_argument, nullptr, 'm' },
		{ "password", required_argument, nullptr, 'P' },
		{ "port", required_argument, nullptr, 'p' },
		{ "screen", required_argument, nullptr, 's' },
		{ "framerate", required_argument, nullptr, 't' },
		{ "vnc-params", required_argument, nullptr, 'v' },
		{ "help", no_argument, nullptr, CHAR_MIN - 2 },
		{ nullptr, 0, nullptr, 0 }
	};

	int c;
	optind = 1;
	while (-1 != (c = getopt_long(argc, argv, "abc:dflmP:p:rs:t:uv:", long_options, nullptr))) 
	{
		switch (c) 
		{
		case 'a':
			configData.relative = false;
			break;

		case 'r':
			configData.relative = true;
			break;

		case 'c':
			configData.configFile = optarg;
			break;

		case 'd':
			configData.downscale = true;
			break;

		case 'u':
			configData.unsafe = true;
			break;

		case 'f':
			configData.fullscreen = true;
			break;

		case 'l':
			configData.localhost = true;
			break;

		case 'm':
			configData.multiThreaded = true;
			break;

		case 'P':
			configData.password = optarg;
			break;

		case 'p':
			configData.port = atoi(optarg);
			break;

		case 's':
			configData.screen = atoi(optarg);
			break;

		case 't':
			configData.frameRate = atoi(optarg);
			break;

		case 'v':
			configData.vncParams = optarg;
			break;

		case CHAR_MIN - 2:
			throw HelpException();

		default:
			throw ParamException();
		}
	}

	if (optind < argc) 
	{
		std::cerr << "Unknown parameter: " << argv[optind] << '\n';
		throw ParamException();
	}
}

bool ReadConfigFile(const char *programName, const std::string& configFile, ConfigData& configData)
{
	std::string configFileTemp;
	libconfig::Config config;
	bool readConfig = false;

	if (configFile.length()) 
	{
		configFileTemp = configFile;
		readConfig = TryReadConfigFile(config, configFileTemp);
		if (!readConfig)
			throw Exception("Unable to read the specified configuration file");
	}
	else 
	{
		configFileTemp = programName;
		configFileTemp += ".conf";
		readConfig = TryReadConfigFile(config, configFileTemp);
		if (!readConfig)
		{
			const char *baseName = basename(programName);
			configFileTemp = getenv("HOME");
			configFileTemp += "/.config/";
			configFileTemp += baseName;
			configFileTemp += ".conf";
			std::cerr << "trying: " << configFileTemp << '\n';
			readConfig = TryReadConfigFile(config, configFileTemp);
			if (!readConfig)
			{
				const char *baseName = basename(programName);
				configFileTemp = "/etc/";
				configFileTemp += baseName;
				configFileTemp += ".conf";
				std::cerr << "trying: " << configFileTemp << '\n';
				readConfig = TryReadConfigFile(config, configFileTemp);
			}
		}
	}

	if (readConfig) 
	{
		std::cerr << "Read config file: " << configFileTemp << '\n';

		config.lookupValue("relative", configData.relative);
		config.lookupValue("unsafe", configData.unsafe);
		config.lookupValue("downscale", configData.downscale);
		config.lookupValue("fullscreen", configData.fullscreen);
		config.lookupValue("localhost", configData.localhost);
		config.lookupValue("multi-threaded", configData.multiThreaded);
		config.lookupValue("password", configData.password);
		config.lookupValue("port", configData.port);
		config.lookupValue("screen", configData.screen);
		config.lookupValue("framerate", configData.frameRate);
		config.lookupValue("vnc-params", configData.vncParams);
	}
	else
		std::cerr << "No configuration file found\n";

	return readConfig;
}

bool TryReadConfigFile(libconfig::Config& config, const std::string& file)
{
	try{
		config.readFile(file.c_str());
		return true;
	}
	catch (libconfig::FileIOException) {
		return false;;
	}
	catch (libconfig::ParseException &e) {
		std::stringstream ss;
		ss << "Error: " << e.getError() << " on line " << e.getLine() << " while reading configuration file: " << file;
		throw Exception(ss.str());
	}
}

void usage(const char *programName)
{
	std::cout << 
		"Usage: " << programName << " [OPTION]...\n"
		"\n"
		"  -a, --absolute               absolute mouse movements\n"
		"  -c, --config-file=FILE       use the specified configuration file\n"
		"  -d, --downscale              downscales the screen to a quarter in vnc\n"
		"  -f, --fullscreen             always runs fullscreen mode\n"
		"  -l, --localhost              only listens to local ports\n"
		"  -m, --multi-threaded         runs vnc in a separate thread\n"
		"  -p, --port=PORT              makes vnc available on the speficied port\n"
		"  -P, --password=PASSWORD      protects the session with PASSWORD\n"
		"  -r, --relative               relative mouse movements\n"
		"  -s, --screen=SCREEN          opens the specified screen number\n"
		"  -t, --framerate=RATE        sets the target frame rate, default is 15\n"
		"  -u, --unsafe                 disables more robust handling of resolution\n"
		"                               change at a small performance gain\n"
		"  -v, --vnc-params             parameters to send to libvncserver\n"
		"      --help                   displays this help and exit\n";
}

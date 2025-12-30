/*
    Copyright (C) 2009 Andrew Caudwell (acaudwell@gmail.com)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version
    3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

std::string search4TextConfig(std::string confname) {
	debugLog("Checking for text config %s in the working directory", confname.c_str());
	if(!boost::filesystem::exists(confname.c_str())) {
		if(!gGourceSettings.textConfDir.empty()) {
			std::string testforconfig = gGourceSettings.textConfDir + confname;
			debugLog("Checking for text config in %s", testforconfig.c_str());
			if(boost::filesystem::exists(testforconfig.c_str())) {
				confname = testforconfig;
			} else {
				confname = texturemanager.getDir() + confname;
				debugLog("Checking for text config in %s", confname.c_str());
				if(!boost::filesystem::exists(confname.c_str())) {
					confname.clear();
				}
			}
		} else {
			confname = texturemanager.getDir() + confname;
			debugLog("Checking for text config in %s", confname.c_str());
			if(!boost::filesystem::exists(confname.c_str())) {
				confname.clear();
			}
		}
	}
	return confname;
}
	

int main(int argc, char *argv[]) {

    std::string exepath;
#ifndef _WIN32
    if(argc > 0) {
        exepath = std::string(argv[0]);
    }
#endif

    SDLAppInit("Mal4s", "mal4s", execpath);

#ifdef _WIN32
        SDLApp::initConsole();
#endif

    ConfFile conf;
    ConfFile textConf;
    std::vector<std::string> files;
    std::string textConfFile;
    //Default path to search for the main config file in when no config is specified at the command line
    std::string defaultMainConfig = texturemanager.getDir() + "mal4s.conf";

    std::string demoFile = texturemanager.getDir() + "sample--newns.mal4s";
    //captionFile will be re-used if a file name is specified at the command prompt
    std::string captionFile = texturemanager.getDir() + "sample--newns.captions";
    std::string captionArg = "--caption-file";
    bool isDemo = false;
    int replacementArgc = argc;
    int demoindex = 4;
    char* demo[demoindex];
    demo[0] = argv[0];
    demo[1] = strdup(captionArg.c_str());
    demo[2] = strdup(captionFile.c_str());
    demo[3] = strdup(demoFile.c_str());

    //convert args to a conf file
    //read the conf file
    //apply the conf file to settings
    try {
        if(argc == 1) {
		printf("No arguments supplied, using sample file: %s.\n", demoFile.c_str());
		isDemo = true;
		gGourceSettings.parseArgs(demoindex, demo, conf, &files);
	} else {
	        gGourceSettings.parseArgs(argc, argv, conf, &files);
	}
        Logger::getDefault()->setLevel(gGourceSettings.log_level);

	if(!files.empty() && gGourceSettings.load_text_config.empty()) {
		size_t conf_marker = files[0].rfind("--");
		std::string baseconfname;
		if(conf_marker != std::string::npos) {
			conf_marker++;
			size_t conf_end_marker = files[0].find_last_of(".");
			if(conf_end_marker != std::string::npos) {
				//Stop reading config name at first ( to allow for browser duplicate file numbering
				baseconfname = files[0].substr(conf_marker + 1, conf_end_marker - conf_marker - 1);
				if(baseconfname.rfind("(") > 0) {
					textConfFile = search4TextConfig(baseconfname + ".conf");
					if(textConfFile.empty()) textConfFile = search4TextConfig(baseconfname.substr(0,baseconfname.rfind("(")) + ".conf");
				} else {
					textConfFile = search4TextConfig(textConfFile + ".conf");
				}
				if(textConfFile.empty()) {
					conf_end_marker = baseconfname.rfind("-");
					if(conf_end_marker != std::string::npos && baseconfname.find_first_not_of("0123456789", conf_end_marker+1) == std::string::npos ) {
						textConfFile = baseconfname.substr(0,conf_end_marker) + ".conf";
						textConfFile = search4TextConfig(textConfFile);
						if(textConfFile.empty()) debugLog("Failed to locate a text config file");
					}
				}	
			}
		} else debugLog("Mal4s file name is not in SOMENAME--TEXTCONF.mal4s format, unable to automatically match the text config");
	} else if(files.empty() && gGourceSettings.load_text_config.empty()) debugLog("Reading from STDIN with no text config specified");

	//Automatic captions file selection
	//Note: Matching could be done here the same way it is done for the text config above.
	//However, bad matching of mal4s file to captions file is quite possible with different
        //versions of the mal4s/captions files.
	if(!files.empty() && gGourceSettings.caption_file.empty()) {
		size_t ext_marker = files[0].find_last_of(".");
		if(ext_marker != std::string::npos) {
			captionFile = files[0].substr(0, ext_marker + 1) + "captions";
			debugLog("Checking for caption file %s", captionFile.c_str());
			if(boost::filesystem::exists(captionFile.c_str())) {
				replacementArgc += 2;
			} else {
				captionFile.clear();
				debugLog("No caption file found");
			}
		}
	}

    	char* replacementArgv[replacementArgc];
	int replacementIt = 0;
	for(int it = 0; it < replacementArgc; it++) {
		if(it == 0 && replacementArgc == argc) {
			replacementArgv[0] = argv[0];
			replacementIt = 1;
		} else if(it == 0) {
			replacementArgv[0] = argv[0];
			replacementArgv[1] = strdup(captionArg.c_str());
			replacementArgv[2] = strdup(captionFile.c_str());
			replacementIt = 3;
		} else {
			replacementArgv[replacementIt] = argv[it];
			replacementIt++;
		}
	}
	if(!gGourceSettings.load_text_config.empty()) textConfFile = gGourceSettings.load_text_config;

	//apply text formatting
	if(!textConfFile.empty()) {
		fprintf(stdout, "Using text config from: %s.\n", textConfFile.c_str());
		textConf.load(textConfFile);
		if(isDemo) {
			gGourceSettings.parseArgs(demoindex, demo, textConf);
		} else gGourceSettings.parseArgs(replacementArgc, replacementArgv, textConf);
		
	}

	//Test if defaultMainConfig file exists and no other config was specified at the command line
	if(gGourceSettings.load_config.empty()) {
		debugLog("Checking for main config file in %s", defaultMainConfig.c_str());
		if(boost::filesystem::exists(defaultMainConfig)) { 
			gGourceSettings.load_config = defaultMainConfig;
		} else debugLog("No main config file found");
	}
	
        //set log level
        Logger::getDefault()->setLevel(gGourceSettings.log_level);

#ifdef _WIN32
        // hide console if not needed
        if(gGourceSettings.log_level == LOG_LEVEL_OFF && !SDLApp::existing_console) {
            SDLApp::showConsole(false);
        }
#endif

        //load config
        if(!gGourceSettings.load_config.empty()) {
		conf.clear();
		conf.load(gGourceSettings.load_config);


                //apply args to loaded conf file
		if(isDemo) {
			gGourceSettings.parseArgs(demoindex, demo, textConf);
		} else gGourceSettings.parseArgs(replacementArgc, replacementArgv, textConf);
        }

        //set path
        if(!files.empty()) {
            std::string path = files[files.size()-1];

            ConfSectionList* sectionlist = conf.getSections("gource");

            if(sectionlist!=0) {
                for(ConfSectionList::iterator sit = sectionlist->begin(); sit != sectionlist->end(); sit++) {
                    (*sit)->setEntry("path", path);
                }
            } else {
                conf.setEntry("gource", "path", path);
            }
        }

	files.clear();
	if(isDemo) {
		gGourceSettings.parseArgs(demoindex, demo, conf, &files);
	} else {
		gGourceSettings.parseArgs(replacementArgc, replacementArgv, conf, &files);
	}

        //apply the config / see if its valid
        gGourceSettings.importDisplaySettings(conf);
        gGourceSettings.importGourceSettings(conf);
	if(!gGourceSettings.caption_file.empty()) printf("Using captions from: %s\n", gGourceSettings.caption_file.c_str());

	if(!textConfFile.empty()) {
	        gGourceSettings.importTextSettings(textConf);
	} else gGourceSettings.importTextSettings(conf);

        //save config
        if(!gGourceSettings.save_config.empty()) {
            conf.save(gGourceSettings.save_config);
            exit(0);
        }

        //write custom log file
        if(!gGourceSettings.output_custom_filename.empty() && !gGourceSettings.path.empty()) {

            Gource::writeCustomLog(gGourceSettings.path, gGourceSettings.output_custom_filename);
            exit(0);
        }

    } catch(ConfFileException& exception) {

        SDLAppQuit(exception.what());
    }

    //enable frameless
    display.enableFrameless(gGourceSettings.frameless);

    // this causes corruption on some video drivers
    if(gGourceSettings.multisample) {
        display.multiSample(4);
    }

    //background needs alpha channel
    if(gGourceSettings.transparent) {
        display.enableAlpha(true);
    }

    //enable vsync
    display.enableVsync(gGourceSettings.vsync);

    //allow resizing window if we are not recording
    if(gGourceSettings.resizable && gGourceSettings.output_ppm_filename.empty()) {
        display.enableResize(true);
    }

    // Change OS High DPI display behaviour
    // On Windows this behaves differently, it seems safe to always enable it
    bool high_dpi = true;
#ifndef _WIN32
    // Requesting High DPI on MacOS may cause the pixel resolution to be doubled.
    // If a resolution has been specified this may not be appropriate
    // E.g. if you are recording a video at a specific resolution
    // Can override by supplying --high-dpi option.
    if(gGourceSettings.viewport_specified && !gGourceSettings.high_dpi) {
        high_dpi = false;
    }
#endif
    display.enableHighDPIAwareness(high_dpi);

    try {

        display.init("Mal4s", gGourceSettings.display_width, gGourceSettings.display_height, gGourceSettings.fullscreen, gGourceSettings.screen);

#if SDL_VERSION_ATLEAST(2,0,0)
        if(!display.isFullscreen() && gGourceSettings.window_x >= 0 && gGourceSettings.window_y >= 0) {
            SDL_SetWindowPosition(display.sdl_window, gGourceSettings.window_x, gGourceSettings.window_y);
        }

        SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
#endif
        
    } catch(SDLInitException& exception) {

        char errormsg[1024];
        snprintf(errormsg, 1024, "SDL initialization failed - %s", exception.what());

        SDLAppQuit(errormsg);
    }

    //Open sound
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    Mix_Music *music = NULL;
    Mix_PlayMusic(music, 1);

    //init frame exporter
    FrameExporter* exporter = 0;

    if(gGourceSettings.output_ppm_filename.size() > 0) {

        try {

            exporter = new PPMExporter(gGourceSettings.output_ppm_filename);

        } catch(PPMExporterException& exception) {

            char errormsg[1024];
            snprintf(errormsg, 1024, "could not write to '%s'", exception.what());

            SDLAppQuit(errormsg);
        }
    }

    if(display.multiSamplingEnabled()) {
        glEnable(GL_MULTISAMPLE_ARB);
    }

    GourceShell* gourcesh = 0;

    try {
        gourcesh = gGourceShell = new GourceShell(&conf, exporter);
        gourcesh->run();

    } catch(ResourceException& exception) {

        char errormsg[1024];
        snprintf(errormsg, 1024, "failed to load resource '%s'", exception.what());

        SDLAppQuit(errormsg);

    } catch(SDLAppException& exception) {

        if(exception.showHelp()) {
            gGourceSettings.help();
        } else {
            SDLAppQuit(exception.what());
        }
    }

    gGourceShell = 0;

    if(gourcesh != 0) delete gourcesh;
    if(exporter != 0) delete exporter;

    //free resources
    display.quit();

    return 0;
}

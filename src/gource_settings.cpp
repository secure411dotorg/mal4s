/*
    Copyright (C) 2010 Andrew Caudwell (acaudwell@gmail.com)

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

#include "gource_settings.h"
#include "core/sdlapp.h"
#include "file.h"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "core/utf8/utf8.h"
#include <time.h>

/*
#include "formats/hg.h"
#include "formats/git.h"
#include "formats/bzr.h"
#include "formats/cvs-exp.h"
#include "formats/cvs2cl.h"
#include "formats/svn.h"
*/

GourceSettings gGourceSettings;

//display help message
void GourceSettings::help(bool extended_help) {

#ifdef _WIN32
    //resize window to fit help message
    SDLApp::resizeConsole(820);
    SDLApp::showConsole(true);
#endif

    printf("Mal4s v%s\n", GOURCE_VERSION);

    printf("Usage: mal4s [OPTIONS] [LOGFILE]\n");
    printf("\nOptions:\n");
    printf("  -h, --help                          Help\n\n");
    printf("  -WIDTHxHEIGHT, --viewport           Set viewport size\n");
    printf("  -f, --fullscreen                    Fullscreen\n");
    printf("      --multi-sampling                Enable multi-sampling\n");
    printf("      --no-vsync                      Disable vsync\n\n");

    printf("      --enable-exec                   Enable command execution (includes browser)\n");
    printf("      --enable-browser                Enable browser command only.\n\n");

    printf("      --disable-exec                  Disable command execution (excludes browser).\n");
    printf("      --disable-browser               Disable browser and URL opening.\n\n");

    printf("      --start-date YYYY-MM-DD         Date to start from\n");
    printf("  -p, --start-position POSITION       Begin at some position (0.0-1.0 or 'random')\n");
    printf("      --stop-position  POSITION       Stop at some position\n");
    printf("  -t, --stop-at-time SECONDS          Stop after a specified number of seconds\n");
    printf("      --stop-at-end                   Stop at end of the log\n");
    printf("      --dont-stop                     Keep running after the end of the log\n");
    printf("      --loop                          Loop at the end of the log\n\n");

    printf("  -a, --auto-skip-seconds SECONDS     Auto skip to next entry if nothing happens\n");
    printf("                                      for a number of seconds (default: 3)\n");
    printf("      --disable-auto-skip             Disable auto skip\n");
    printf("  -s, --seconds-per-day SECONDS       Speed in seconds per day (default: 10)\n");
    printf("      --realtime                      Realtime playback speed\n");
    printf("  -c, --time-scale SCALE              Change simulation time scale (default: 1.0)\n");
    printf("  -e, --elasticity FLOAT              Elasticity of nodes (default: 0.0)\n\n");

    printf("      --key-off                       Hide key\n\n");

    printf("      --plotter-image-dir DIRECTORY   Dir containing images to use as avatars\n");
    printf("      --default-plotter-image IMAGE	  Default plotter image file\n");
    printf("      --colour-images                 Colourize plotter images\n\n");

    printf("  -i, --host-idle-time SECONDS        Time hosts remain idle (default: 0)\n\n");

    printf("      --max-hosts NUMBER              Max number of hosts or 0 for no limit\n");
    printf("      --max-host-lag SECONDS          Max time hosts of a commit can take to appear\n\n");

    printf("  -C, --load-config CONF_FILE         Load a main config file\n");
    printf("  -T, --load-text-config CONF_FILE    Load a text config file\n");
    printf("  -D, --text-config-dir DIRECTORY     Path to text configs (matched with file--TEXTCONF.mal4s)\n");
    printf("      --save-config CONF_FILE         Save a config file with the current options\n\n");

    printf("      --wrap-max-lines LINES          Max number of lines a hover line can wrap before truncating\n");
    printf("      --truncate-hover-lines          Do not wrap hover lines by default\n");
    printf("      --wrap-hover-lines              Wrap hover lines by default\n");
    printf("      --hover-line-length CHARS       Max number of characters on a hover line.\n\n");

    printf("  -o, --output-ppm-stream FILE        Output PPM stream to a file ('-' for STDOUT)\n");
    printf("  -r, --output-framerate  FPS         Framerate of output (25,30,60)\n\n");

if(extended_help) {
    printf("Extended Options:\n\n");

    printf("      --output-custom-log FILE        Output a custom format log file ('-' for STDOUT).\n\n");

    printf("  -b, --background-colour FFFFFF      Background colour in hex\n");
    printf("      --background-image IMAGE        Set a background image\n\n");

    printf("      --bloom-multiplier              Adjust the amount of bloom (default: 1.0)\n");
    printf("      --bloom-intensity               Adjust the intensity of the bloom (default: 0.75)\n\n");

    printf("      --camera-mode MODE              Camera mode (overview,track)\n");
    printf("      --crop AXIS                     Crop view on an axis (vertical,horizontal)\n");
    printf("      --padding FLOAT                 Camera view padding (default: 1.1)\n\n");

    printf("      --disable-auto-rotate           Disable automatic camera rotation\n\n");

    printf("      --date-format FORMAT            Specify display date string (strftime format)\n\n");

    printf("      --font-size SIZE                Font size used by date and title\n");
    printf("      --font-colour FFFFFF            Font colour used by date and title in hex\n\n");

    printf("      --show-tld-only                 Show tld only\n\n");

    printf("      --hide DISPLAY_ELEMENT          bloom,date,branchnames,hostss,hostnames,mouse,progress,\n");
    printf("                                      root,tree,plotters,plotternames\n\n");

    printf("      --logo IMAGE                    Logo to display in the foreground\n");
    printf("      --logo-offset XxY               Offset position of the logo\n\n");

    printf("      --title TITLE                   Set a title\n\n");

    printf("      --transparent                   Make the background transparent\n\n");

    printf("      --plotter-filter REGEX          Ignore plotternames matching this regex\n");
    printf("      --host-filter REGEX             Ignore hosts matching this regex\n\n");

    printf("      --plotter-friction SECONDS      Change the rate plotters slow down (default: 0.67)\n");
    printf("      --plotter-scale SCALE           Change scale of plotters (default: 1.0)\n");
    printf("      --max-plotter-speed UNITS       Speed plotters can travel per second (default: 500)\n\n");

    printf("      --follow-plotter PLOTTER        Camera will automatically follow this plotter\n");
    printf("      --highlight-branches            Highlight the names of all branches\n");
    printf("      --highlight-plotter PLOTTER     Highlight the names of a particular plotter\n");
    printf("      --highlight-plotters            Highlight the names of all plotters\n\n");
    printf("      --highlight-colour              Font colour for highlighted plotters in hex.\n");
    printf("      --selection-colour              Font colour for selected plotters and hosts.\n");
    printf("      --branch-colour                 Font colour for branches.\n\n");

    printf("      --caption-file FILE             Caption file\n");
    printf("      --caption-size SIZE             Caption font size\n");
    printf("      --caption-colour FFFFFF         Caption colour in hex\n");
    printf("      --caption-duration SECONDS      Caption duration (default: 10.0)\n");
    printf("      --caption-offset X              Caption horizontal offset\n\n");

    printf("      --hash-seed SEED                Change the seed of hash function.\n\n");

}

    if(!extended_help) {
        printf("To see the full command line options use '-H'\n\n");
    }

#ifdef _WIN32
    if(!SDLApp::existing_console) {
        printf("Press Enter\n");
        getchar();
    }
#endif

    exit(0);
}

GourceSettings::GourceSettings() {
    repo_count = 0;
    file_graphic = 0;
    log_level = LOG_LEVEL_OFF;
    shutdown = false;

    setGourceDefaults();

    default_section_name = "gource";

    //translate args
    arg_aliases["p"]                = "start-position";
    arg_aliases["a"]                = "auto-skip-seconds";
    arg_aliases["s"]                = "seconds-per-day";
    arg_aliases["t"]                = "stop-at-time";
    arg_aliases["i"]                = "host-idle-time";
    arg_aliases["e"]                = "elasticity";
    arg_aliases["h"]                = "help";
    arg_aliases["?"]                = "help";
    arg_aliases["H"]                = "extended-help";
    arg_aliases["b"]                = "background-colour";
    arg_aliases["c"]                = "time-scale";
    arg_aliases["background"]       = "background-colour";
    arg_aliases["disable-bloom"]    = "hide-bloom";
    arg_aliases["disable-progress"] = "hide-progress";
    arg_aliases["highlight-all-plotters"] = "highlight-plotters";
    arg_aliases["C"]                = "load-config";
    arg_aliases["T"]                = "load-text-config";
    arg_aliases["D"]                = "text-config-dir";

    //command line only options
    conf_sections["help"]           = "command-line";
    conf_sections["extended-help"]  = "command-line";
    conf_sections["load-text-config"]  = "command-line";
    conf_sections["load-config"]    = "command-line";
    conf_sections["save-config"]    = "command-line";
    conf_sections["log-level"]      = "command-line";
    conf_sections["text-config-dir"] = "command-line";

    //boolean args
    arg_types["enable-exec"]        = "bool";
    arg_types["enable-browser"]     = "bool";
    arg_types["help"]               = "bool";
    arg_types["extended-help"]      = "bool";
    arg_types["stop-on-idle"]       = "bool";
    arg_types["screenshot-at-end"]  = "bool";
    arg_types["stop-at-end"]        = "bool";
    arg_types["dont-stop"]          = "bool";
    arg_types["loop"]               = "bool";
    arg_types["realtime"]           = "bool";
    arg_types["colour-images"]      = "bool";
    arg_types["hide-date"]          = "bool";
    arg_types["hide-hosts"]         = "bool";
    arg_types["hide-plotters"]      = "bool";
    arg_types["hide-tree"]          = "bool";
    arg_types["hide-plotter-names"] = "bool";
    arg_types["hide-hostnames"]     = "bool";
    arg_types["hide-branch-names"]  = "bool";
    arg_types["hide-progress"]      = "bool";
    arg_types["hide-bloom"]         = "bool";
    arg_types["hide-mouse"]         = "bool";
    arg_types["hide-root"]          = "bool";
    arg_types["highlight-plotters"] = "bool";
    arg_types["highlight-branches"] = "bool";
    arg_types["show-tld-only"]      = "bool";
    arg_types["key-off"]            = "bool";
    arg_types["ffp"]                = "bool";
    arg_types["truncate-hover-lines"] = "bool";
    arg_types["wrap-hover-lines"]   = "bool";
    arg_types["wrap-max-lines"]     = "int";
    arg_types["hover-line-length"]  = "int";

    arg_types["disable-auto-rotate"] = "bool";
    arg_types["disable-auto-skip"]  = "bool";

    arg_types["bloom-intensity"]    = "float";
    arg_types["bloom-multiplier"]   = "float";
    arg_types["elasticity"]         = "float";
    arg_types["seconds-per-day"]    = "float";
    arg_types["auto-skip-seconds"]  = "float";
    arg_types["stop-at-time"]       = "float";
    arg_types["max-plotter-speed"]  = "float";
    arg_types["plotter-friction"]   = "float";
    arg_types["padding"]            = "float";
    arg_types["time-scale"]         = "float";

    arg_types["max-hosts"]          = "int";
    arg_types["font-size"]          = "int";
    arg_types["hash-seed"]          = "int";

    arg_types["plotter-filter"]     = "multi-value";
    arg_types["host-filter"]        = "multi-value";
    arg_types["follow-plotter"]     = "multi-value";
    arg_types["highlight-plotter"]  = "multi-value";

    arg_types["log-level"]          = "string";
    arg_types["background-image"]   = "string";
    arg_types["logo"]               = "string";
    arg_types["logo-offset"]        = "string";
    arg_types["log-command"]        = "string";
    arg_types["text-config-dir"]    = "string";
    arg_types["load-text-config"]   = "string";
    arg_types["load-config"]        = "string";
    arg_types["save-config"]        = "string";
    arg_types["output-custom-log"]  = "string";
    arg_types["path"]               = "string";
    arg_types["log-command"]        = "string";
    arg_types["background-colour"]  = "string";
    arg_types["host-idle-time"]     = "string";
    arg_types["plotter-image-dir"]  = "string";
    arg_types["default-plotter-image"] = "string";
    arg_types["date-format"]        = "string";
    arg_types["log-format"]         = "string";

    arg_types["disable-browser"]    = "bool";
    arg_types["disable-exec"]       = "bool";
    arg_types["browser-command"]    = "string";
    arg_types["start-position"]     = "string";
    arg_types["start-date"]         = "string";
    arg_types["stop-position"]      = "string";
    arg_types["crop"]               = "string";
    arg_types["hide"]               = "string";
    arg_types["max-host-lag"]       = "string";
    arg_types["plotter-scale"]      = "string";
    arg_types["camera-mode"]        = "string";
    arg_types["title"]              = "string";
    arg_types["font-colour"]        = "string";
    arg_types["highlight-colour"]   = "string";
    arg_types["selection-colour"]   = "string";
    arg_types["branch-colour"]      = "string";

    arg_types["caption-file"]       = "string";
    arg_types["caption-size"]       = "int";
    arg_types["caption-duration"]   = "float";
    arg_types["caption-colour"]     = "string";
    arg_types["caption-offset"]     = "int";


}

void GourceSettings::setTextDefaults() {

    //hoverUnsetField is the field replacement in hover lines where a non-existent field is addressed.
    //blank fields are not "empty", this would be considered a replacement for unset and not blank.
    hoverUnsetField = "";
    keyFormat       = "${n1}";
    keyWidth        = 90.0f;
    show_key        = true;
    hide_date       = false;
}
void GourceSettings::setGourceDefaults() {

    path           = "-";
    default_path   = true;

    ffp            = false;

    wrap_truncate  = "wrap";
    wrap_max_lines = 3;
    wrap_truncate_chars = 50;

    hide_users     = false;
    hide_tree      = false;
    hide_files     = false;
    hide_usernames = false;
    hide_filenames = true;
    hide_dirnames  = true;
    hide_progress  = true;
    hide_bloom     = false;
    hide_mouse     = false;
    hide_root      = false;

    disable_exec   = true;
    disable_browser = true;

    start_timestamp = 0;
    start_position = 0.0f;
    stop_position  = 0.0f;
    stop_at_time   = -1.0f;
    stop_on_idle   = false;
    screenshot_at_end = false;
    stop_at_end    = false;
    dont_stop      = false;


    disable_auto_rotate = false;

    auto_skip_seconds = 1.0f;
    days_per_second   = 0.1f; // TODO: check this is right
    file_idle_time    = 0.0f;
    time_scale        = 1.0f;

    loop = false;

    logo        = "";
    logo_offset = vec2(20.0f,20.0f);

    colour_user_images = false;
    default_user_image = "";
    user_image_dir     = "";
    user_image_map.clear();

    camera_zoom_min     = 50.0f;
    camera_zoom_default = 100.0f;
    camera_zoom_max     = 10000.0f;

    camera_mode     = "overview";
    padding         = 1.1f;

    crop_vertical   = false;
    crop_horizontal = false;

    bloom_multiplier = 1.0f;
    bloom_intensity  = 0.025f;

    background_colour = vec3(0.1f, 0.1f, 0.1f);

    //relocated default background_image to the top of importGourceSettings to fix Windows build.
    background_image  = "";

    title             = "Get tools and data at https://DissectCyber.com";

    font_size = 16;
    dir_colour       = vec3(1.0f);
    font_colour      = vec3(1.0f);
    highlight_colour = vec3(1.0f);
    selection_colour = vec3(1.0, 1.0, 0.3f);

    elasticity = 0.0f;

    git_branch = "";

    log_format  = "mal4s";
//    date_format = "%A, %d %B, %Y %X";
    date_format = "";

    max_files      = 0;
    max_user_speed = 500.0f;
    max_file_lag   = 5.0f;

    user_idle_time = 3.0f;
    user_friction  = 1.0f;
    user_scale     = 1.0f;

    follow_users.clear();
    highlight_users.clear();
    highlight_all_users = false;
    highlight_dirs = false;

    caption_file     = "";
    caption_duration = 10.0f;
    caption_size     = 16;
    caption_offset   = 0;
    caption_colour   = vec3(1.0f, 1.0f, 1.0f);

    gStringHashSeed = 31;

    //delete file filters
    for(std::vector<Regex*>::iterator it = file_filters.begin(); it != file_filters.end(); it++) {
        delete (*it);
    }
    file_filters.clear();
    file_extensions = false;

    //delete user filters
    for(std::vector<Regex*>::iterator it = user_filters.begin(); it != user_filters.end(); it++) {
        delete (*it);
    }
    user_filters.clear();
}

void GourceSettings::commandLineOption(const std::string& name, const std::string& value) {

    if(name == "help") {
        help();
    }

    if(name == "extended-help") {
        help(true);
    }

    if(name == "load-text-config" && value.size() > 0) {
        load_text_config = value;
        return;
    }

    if(name == "load-config" && value.size() > 0) {
        load_config = value;
        return;
    }

    if(name == "text-config-dir" && value.size() > 0) {
#ifdef _WIN32
	std::string dir_delim = "\\";
#else
	std::string dir_delim = "/";
#endif
	if(value.substr(value.size() - 1) == dir_delim) {
		textConfDir = value;
	} else textConfDir = value + dir_delim;
	return;
    }

    if(name == "save-config" && value.size() > 0) {
        save_config = value;
        return;
    }

    if(name == "log-level") {
        if(value == "warn") {
            log_level = LOG_LEVEL_WARN;
        } else if(value == "debug") {
            log_level = LOG_LEVEL_DEBUG;
        } else if(value == "info") {
            log_level = LOG_LEVEL_INFO;
        } else if(value == "error") {
            log_level = LOG_LEVEL_ERROR;
        } else if(value == "pedantic") {
            log_level = LOG_LEVEL_PEDANTIC;
        }
        return;
    }

    std::string invalid_error = std::string("invalid ") + name + std::string(" value");
    throw ConfFileException(invalid_error, "", 0);
}

void GourceSettings::importTextSettings(ConfFile& conffile, ConfSection* text_settings) {
    setTextDefaults();

    if(text_settings == 0) text_settings = conffile.getSection("text");

    if(text_settings == 0) {
        text_settings = conffile.addSection("text");
    }

    ConfEntry* entry = 0;

   if((entry = text_settings->getEntry("f5-action")) != 0) {
		f5_action = entry->getString();
    }

   if((entry = text_settings->getEntry("f7-action")) != 0) {
		f7_action = entry->getString();
    }

   if((entry = text_settings->getEntry("f9-action")) != 0) {
		f9_action = entry->getString();
    }

   if((entry = text_settings->getEntry("mouseclick-action")) != 0) {
		mouseclick_action = entry->getString();
    }

   if((entry = text_settings->getEntry("browser-url")) != 0) {
		browser_url = entry->getString();
    }

   if((entry = text_settings->getEntry("key-format")) != 0) {
		keyFormat.clear();
		keyFormat = entry->getString();
    }

    if(text_settings->getBool("key-off")) {
        show_key = false;
    }

    if((entry = text_settings->getEntry("key-width")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify key-width (float)");

        keyWidth = entry->getFloat();

        if(keyWidth<=0.0f) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = text_settings->getEntry("hover-replace-unset")) != 0) {
		hoverUnsetField = entry->getString();
    }

    if((entry = text_settings->getEntry("hover-line-1")) != 0) {
	if(hoverLines.size() == 0) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-2")) != 0) {
	if(hoverLines.size() == 1) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-3")) != 0) {
	if(hoverLines.size() == 2) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-4")) != 0) {
	if(hoverLines.size() == 3) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-5")) != 0) {
	if(hoverLines.size() == 4) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-6")) != 0) {
	if(hoverLines.size() == 5) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-7")) != 0) {
	if(hoverLines.size() == 6) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-8")) != 0) {
	if(hoverLines.size() == 7) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-9")) != 0) {
	if(hoverLines.size() == 8) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-10")) != 0) {
	if(hoverLines.size() == 9) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-11")) != 0) {
	if(hoverLines.size() == 10) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-12")) != 0) {
	if(hoverLines.size() == 11) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-13")) != 0) {
	if(hoverLines.size() == 12) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-14")) != 0) {
	if(hoverLines.size() == 13) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-15")) != 0) {
	if(hoverLines.size() == 14) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-16")) != 0) {
	if(hoverLines.size() == 15) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-17")) != 0) {
	if(hoverLines.size() == 16) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-18")) != 0) {
	if(hoverLines.size() == 17) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-19")) != 0) {
	if(hoverLines.size() == 18) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-20")) != 0) {
	if(hoverLines.size() == 19) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-21")) != 0) {
	if(hoverLines.size() == 20) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-22")) != 0) {
	if(hoverLines.size() == 21) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-23")) != 0) {
	if(hoverLines.size() == 22) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-24")) != 0) {
	if(hoverLines.size() == 23) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-25")) != 0) {
	if(hoverLines.size() == 24) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-26")) != 0) {
	if(hoverLines.size() == 25) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-27")) != 0) {
	if(hoverLines.size() == 26) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-28")) != 0) {
	if(hoverLines.size() == 27) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-29")) != 0) {
	if(hoverLines.size() == 28) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-30")) != 0) {
	if(hoverLines.size() == 29) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-31")) != 0) {
	if(hoverLines.size() == 30) {
		hoverLines.push_back(entry->getString());
	} 
    }

    if((entry = text_settings->getEntry("hover-line-32")) != 0) {
	if(hoverLines.size() == 31) {
		hoverLines.push_back(entry->getString());
	} 
    }

   //If there is no formatted hover lines, mal4s crashes on hover.
   if(hoverLines.size() == 0) {
	hoverLines.push_back("hover-line-1 not configured");
   }

}

/*
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif
*/
void GourceSettings::importGourceSettings(ConfFile& conffile, ConfSection* gource_settings) {

    setGourceDefaults();

    //This default can not be set for windows builds in setGourceDefaults with the texturemanger.getDir() function
    if(background_image.empty()) {
	    background_image  = texturemanager.getDir() + "dissectcyber.png";
    }

    if(gource_settings == 0) gource_settings = conffile.getSection(default_section_name);

    if(gource_settings == 0) {
        gource_settings = conffile.addSection("gource");
    }

    ConfEntry* entry = 0;

    //hide flags

    std::vector<std::string> hide_fields;

    if((entry = gource_settings->getEntry("hide")) != 0) {

        if(!entry->hasValue()) conffile.missingValueException(entry);

        std::string hide_string = entry->getString();

        size_t sep;
        while((sep = hide_string.find(",")) != std::string::npos) {

            if(sep == 0 && hide_string.size()==1) break;

            if(sep == 0) {
                hide_string = hide_string.substr(sep+1, hide_string.size()-1);
                continue;
            }

            std::string hide_field  = hide_string.substr(0, sep);
            hide_fields.push_back(hide_field);
            hide_string = hide_string.substr(sep+1, hide_string.size()-1);
        }

        if(hide_string.size() > 0 && hide_string != ",") hide_fields.push_back(hide_string);

        //validate field list

        for(std::vector<std::string>::iterator it = hide_fields.begin(); it != hide_fields.end(); it++) {
            std::string hide_field = (*it);

            if(   hide_field != "date"
               && hide_field != "plotters"
               && hide_field != "tree"
               && hide_field != "hosts"
               && hide_field != "plotternames"
               && hide_field != "hostnames"
               && hide_field != "branchnames"
               && hide_field != "bloom"
               && hide_field != "progress"
               && hide_field != "mouse"
               && hide_field != "root") {
                std::string unknown_hide_option = std::string("unknown option hide ") + hide_field;
                conffile.entryException(entry, unknown_hide_option);
            }
        }
    }

    //check hide booleans
    for(std::map<std::string,std::string>::iterator it = arg_types.begin(); it != arg_types.end(); it++) {
        if(it->first.find("hide-") == 0 && it->second == "bool") {

            if(gource_settings->getBool(it->first)) {
                std::string hide_field = it->first.substr(5, it->first.size()-5);
                hide_fields.push_back(hide_field);
            }
        }
    }

    if(hide_fields.size()>0) {

        for(std::vector<std::string>::iterator it = hide_fields.begin(); it != hide_fields.end(); it++) {
            std::string hidestr = (*it);

                if(hidestr == "date")       hide_date      = true;
            else if(hidestr == "plotters")     hide_users     = true;
            else if(hidestr == "tree")      hide_tree      = true;
            else if(hidestr == "hosts")     hide_files     = true;
            else if(hidestr == "plotternames") hide_usernames = true;
            else if(hidestr == "hostnames") hide_filenames = true;
            else if(hidestr == "branchnames")  hide_dirnames  = true;
            else if(hidestr == "bloom")     hide_bloom     = true;
            else if(hidestr == "progress")  hide_progress  = true;
            else if(hidestr == "root")      hide_root      = true;
            else if(hidestr == "mouse")     {
                hide_mouse     = true;
                hide_progress  = true;
            }
        }
    }

    if((entry = gource_settings->getEntry("browser-command")) != 0) {
	browser_command = entry->getString();
    }

    if((entry = gource_settings->getEntry("hover-replace-unset")) != 0) {
	hoverUnsetField = entry->getString();
    }

    if((entry = gource_settings->getEntry("date-format")) != 0) {

        if(!entry->hasValue()) conffile.missingValueException(entry);

        date_format = entry->getString();
    }

    if(gource_settings->getBool("truncate-hover-lines")) {
        wrap_truncate = "truncate";
    }

    if(gource_settings->getBool("wrap-hover-lines")) {
        wrap_truncate = "wrap";
    }

    if((entry = gource_settings->getEntry("hover-line-length")) != 0) {
        if(!entry->hasValue()) conffile.entryException(entry, "specify max hover line length");

    	wrap_truncate_chars = entry->getInt();
    }

    if((entry = gource_settings->getEntry("wrap-max-lines")) != 0) {
        if(!entry->hasValue()) conffile.entryException(entry, "specify max number of lines to wrap on hover");

        wrap_max_lines = entry->getInt();
    }

    if(gource_settings->getBool("enable-exec")) {
	disable_browser=false;
	disable_exec=false;
    }

    if(gource_settings->getBool("enable-browser")) {
	disable_browser=false;
    }

    if(gource_settings->getBool("disable-browser")) {
        disable_browser=true;
    }

    if(gource_settings->getBool("disable-exec")) {
        disable_exec=true;
    }

    if(gource_settings->getBool("disable-auto-rotate")) {
        disable_auto_rotate=true;
    }

    if(gource_settings->getBool("disable-auto-skip")) {
        auto_skip_seconds = -1.0;
    }

    if(gource_settings->getBool("loop")) {
        loop = true;
    }

    if((entry = gource_settings->getEntry("git-branch")) != 0) {

        if(!entry->hasValue()) conffile.missingValueException(entry);

        git_branch = entry->getString();
    }

    if(gource_settings->getBool("colour-images")) {
        colour_user_images = true;
    }

    if((entry = gource_settings->getEntry("crop")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify crop (vertical,horizontal)");

        std::string crop = entry->getString();

        if(crop == "vertical") {
            crop_vertical = true;
        } else if (crop == "horizontal") {
            crop_horizontal = true;
        } else {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("log-format")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify log-format (format)");

        log_format = entry->getString();

        if(  log_format != "mal4s"
           && log_format != "custom") {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("default-plotter-image")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify default-plotter-image (image path)");

        default_user_image = entry->getString();
    }

    if((entry = gource_settings->getEntry("plotter-image-dir")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify plotter-image-dir (directory)");

        user_image_dir = entry->getString();

        //append slash
        if(user_image_dir[user_image_dir.size()-1] != '/') {
            user_image_dir += std::string("/");
        }

        user_image_map.clear();

        boost::filesystem::path image_dir_path(user_image_dir);

        if(!is_directory(image_dir_path)) {
             conffile.entryException(entry, "specified plotter-image-dir is not a directory");
        }

        std::vector<boost::filesystem::path> image_dir_files;

        try {
            copy(boost::filesystem::directory_iterator(image_dir_path), boost::filesystem::directory_iterator(), back_inserter(image_dir_files));
        } catch(const boost::filesystem::filesystem_error& exception) {
             conffile.entryException(entry, "error reading specified plotter-image-dir");
        }

        for(boost::filesystem::path& p : image_dir_files) {

            std::string dirfile;

#ifdef _WIN32
            std::wstring dirfile_16 = p.filename().wstring();
            utf8::utf16to8(dirfile_16.begin(), dirfile_16.end(), back_inserter(dirfile));
#else
            dirfile = p.filename().string();
#endif
            std::string file_ext = extension(p);
            boost::algorithm::to_lower(file_ext);

            if(file_ext != ".jpg" && file_ext != ".jpeg" && file_ext != ".png") continue;

            std::string image_path = gGourceSettings.user_image_dir + dirfile;
            std::string name       = dirfile.substr(0,dirfile.size() - file_ext.size());
/*
#ifdef __APPLE__
                CFMutableStringRef help = CFStringCreateMutable(kCFAllocatorDefault, 0);
                CFStringAppendCString(help, name.c_str(), kCFStringEncodingUTF8);
                CFStringNormalize(help, kCFStringNormalizationFormC);
                char data[4096];
                CFStringGetCString(help,
                                   data,
                                   sizeof(data),
                                   kCFStringEncodingUTF8);
                name = data;
#endif
*/
            debugLog("%s => %s", name.c_str(), image_path.c_str());

            user_image_map[name] = image_path;
        }
    }

    if((entry = gource_settings->getEntry("caption-file")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify caption file (filename)");

        caption_file = entry->getString();
    }

    if((entry = gource_settings->getEntry("caption-duration")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify caption duration (seconds)");

        caption_duration = entry->getFloat();

        if(caption_duration <= 0.0f) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("caption-size")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify caption size");

        caption_size = entry->getInt();

        if(caption_size<1 || caption_size>100) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("caption-offset")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify caption offset");

        caption_offset = entry->getInt();
    }

    if((entry = gource_settings->getEntry("caption-colour")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify caption colour (FFFFFF)");

        int r,g,b;

        std::string colstring = entry->getString();

        if(entry->isVec3()) {
            caption_colour = entry->getVec3();
        } else if(colstring.size()==6 && sscanf(colstring.c_str(), "%02x%02x%02x", &r, &g, &b) == 3) {
            caption_colour = vec3(r,g,b);
            caption_colour /= 255.0f;
        } else {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("bloom-intensity")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify bloom-intensity (float)");

        bloom_intensity = entry->getFloat();

        if(bloom_intensity<=0.0f) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("bloom-multiplier")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify bloom-multiplier (float)");

        bloom_multiplier = entry->getFloat();

        if(bloom_multiplier<=0.0f) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("elasticity")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify elasticity (float)");

        elasticity = entry->getFloat();

        if(elasticity<=0.0f) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("font-size")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify font size");

        font_size = entry->getInt();

        if(font_size<1 || font_size>100) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("hash-seed")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify hash seed (integer)");

        gStringHashSeed = entry->getInt();
    }

    if((entry = gource_settings->getEntry("font-colour")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify font colour (FFFFFF)");

        int r,g,b;

        std::string colstring = entry->getString();

        if(entry->isVec3()) {
            font_colour = entry->getVec3();
        } else if(colstring.size()==6 && sscanf(colstring.c_str(), "%02x%02x%02x", &r, &g, &b) == 3) {
            font_colour = vec3(r,g,b);
            font_colour /= 255.0f;
        } else {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("background-colour")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify background colour (FFFFFF)");

        int r,g,b;

        std::string colstring = entry->getString();

        if(entry->isVec3()) {
            background_colour = entry->getVec3();
        } else if(colstring.size()==6 && sscanf(colstring.c_str(), "%02x%02x%02x", &r, &g, &b) == 3) {
            background_colour = vec3(r,g,b);
            background_colour /= 255.0f;
        } else {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("highlight-colour")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify highlight colour (FFFFFF)");

        int r,g,b;

        std::string colstring = entry->getString();

        if(entry->isVec3()) {
            highlight_colour = entry->getVec3();
        } else if(colstring.size()==6 && sscanf(colstring.c_str(), "%02x%02x%02x", &r, &g, &b) == 3) {
            highlight_colour = vec3(r,g,b);
            highlight_colour /= 255.0f;
        } else {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("selection-colour")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify selection colour (FFFFFF)");

        int r,g,b;

        std::string colstring = entry->getString();

        if(entry->isVec3()) {
            selection_colour = entry->getVec3();
        } else if(colstring.size()==6 && sscanf(colstring.c_str(), "%02x%02x%02x", &r, &g, &b) == 3) {
            selection_colour = vec3(r,g,b);
            selection_colour /= 255.0f;
        } else {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("branch-colour")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify dir colour (FFFFFF)");

        int r,g,b;

        std::string colstring = entry->getString();

        if(entry->isVec3()) {
            dir_colour = entry->getVec3();
        } else if(colstring.size()==6 && sscanf(colstring.c_str(), "%02x%02x%02x", &r, &g, &b) == 3) {
            dir_colour = vec3(r,g,b);
            dir_colour /= 255.0f;
        } else {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("background-image")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify background image (image path)");

        background_image = entry->getString();
    }

    if((entry = gource_settings->getEntry("title")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify title");

        title = entry->getString();
    }

    if((entry = gource_settings->getEntry("logo")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify logo (image path)");

        logo = entry->getString();
    }

    if((entry = gource_settings->getEntry("logo-offset")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify logo-offset (XxY)");

        std::string logo_offset_str = entry->getString();

        int posx = 0;
        int posy = 0;

        if(parseRectangle(logo_offset_str, posx, posy)) {
            logo_offset = vec2(posx, posy);
        } else {
            conffile.invalidValueException(entry);
        }

    }

    if((entry = gource_settings->getEntry("seconds-per-day")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify seconds-per-day (seconds)");

        float seconds_per_day = entry->getFloat();

        if(seconds_per_day<=0.0f) {
            conffile.invalidValueException(entry);
        }

        // convert seconds-per-day to days-per-second
        days_per_second = 1.0 / seconds_per_day;
    }

    if((entry = gource_settings->getEntry("auto-skip-seconds")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify auto-skip-seconds (seconds)");

        auto_skip_seconds = entry->getFloat();

        if(auto_skip_seconds <= 0.0) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("host-idle-time")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify host-idle-time (seconds)");

        std::string file_idle_str = entry->getString();

        file_idle_time = (float) atoi(file_idle_str.c_str());

        if(file_idle_time<0.0f || (file_idle_time == 0.0f && file_idle_str[0] != '0') ) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("plotter-idle-time")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify plotter-idle-time (seconds)");

        user_idle_time = entry->getFloat();

        if(user_idle_time < 0.0f) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("time-scale")) != 0) {

        if(!entry->hasValue())
            conffile.entryException(entry, "specify time-scale (scale)");

        time_scale = entry->getFloat();

        if(time_scale <= 0.0f || time_scale > 4.0f) {
            conffile.entryException(entry, "time-scale outside of range 0.0 - 4.0");
        }
    }

    if((entry = gource_settings->getEntry("start-date")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify start-date (YYYY-MM-DD)");

        std::string start_date_string = entry->getString();

        int year;
        int month;
        int day;

        if(start_date_string.size()==10 && sscanf(start_date_string.c_str(), "%04d-%02d-%02d", &year, &month, &day) == 3) {

            struct tm time_str;
            time_str.tm_year  = year - 1900;
            time_str.tm_mon   = month - 1;
            time_str.tm_mday  = day;
            time_str.tm_hour  = 0;
            time_str.tm_min   = 0;
            time_str.tm_sec   = 0;
            time_str.tm_isdst = -1;

            time_t timestamp = mktime(&time_str);

            if(timestamp == -1) {
                conffile.invalidValueException(entry);
            }

            this->start_timestamp = timestamp;
            this->start_date      = start_date_string;
        } else {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("start-position")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify start-position (float,random)");

        if(entry->getString() == "random") {
            srand(time(0));
            start_position = (rand() % 1000) / 1000.0f;
        } else {
            start_position = entry->getFloat();

            if(start_position<=0.0 || start_position>=1.0) {
                conffile.entryException(entry, "start-position outside of range 0.0 - 1.0 (non-inclusive)");
            }
        }
    }

    if((entry = gource_settings->getEntry("stop-position")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify stop-position (float)");

        stop_position = entry->getFloat();

        if(stop_position<=0.0 || stop_position>1.0) {
            conffile.entryException(entry, "stop-position outside of range 0.0 - 1.0 (inclusive)");
        }
    }

    if((entry = gource_settings->getEntry("stop-at-time")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify stop-at-time (seconds)");

        stop_at_time = entry->getFloat();

        if(stop_at_time <= 0.0) {
            conffile.invalidValueException(entry);
        }
    }

    if(gource_settings->getBool("ffp")) {
        ffp = true;
    }

    if(gource_settings->getBool("realtime")) {
        days_per_second = 1.0 / 86400.0;
    }

    if(gource_settings->getBool("dont-stop")) {
        dont_stop = true;
    }

    if(gource_settings->getBool("screenshot-at-end")) {
        screenshot_at_end = true;
	stop_at_end = true;
    }

    if(gource_settings->getBool("stop-at-end")) {
        stop_at_end = true;
    }

    //NOTE: this no longer does anything
    if(gource_settings->getBool("stop-on-idle")) {
        stop_on_idle = true;
    }

    if((entry = gource_settings->getEntry("max-hosts")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify max-hosts (number)");

        max_files = entry->getInt();

        if( max_files<0 || (max_files == 0 && entry->getString() != "0") ) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("max-host-lag")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify max-host-lag (seconds)");

        max_file_lag = entry->getFloat();

        if(max_file_lag==0.0) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("plotter-friction")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify plotter-friction (seconds)");

        user_friction = entry->getFloat();

        if(user_friction<=0.0) {
            conffile.invalidValueException(entry);
        }

        user_friction = 1.0 / user_friction;
    }

    if((entry = gource_settings->getEntry("plotter-scale")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify plotter-scale (scale)");

        user_scale = entry->getFloat();

        if(user_scale<=0.0 || user_scale>100.0) {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("max-plotter-speed")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify max-plotter-speed (units)");

        max_user_speed = entry->getFloat();

        if(max_user_speed<=0) {
            conffile.invalidValueException(entry);
        }
    }

    if(   gource_settings->getBool("highlight-plotters")
       || gource_settings->getBool("highlight-all-plotters")) {
        highlight_all_users = true;
    }

    if(gource_settings->getBool("highlight-branches")) {
        highlight_dirs = true;
    }

    if((entry = gource_settings->getEntry("camera-mode")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify camera-mode (overview,track)");

        camera_mode = entry->getString();

        if(camera_mode != "overview" && camera_mode != "track") {
            conffile.invalidValueException(entry);
        }
    }

    if((entry = gource_settings->getEntry("padding")) != 0) {

        if(!entry->hasValue()) conffile.entryException(entry, "specify padding (float)");

        padding = entry->getFloat();

        if(padding <= 0.0f || padding >= 2.0f) {
            conffile.invalidValueException(entry);
        }
    }

    // multi-value entries

    if((entry = gource_settings->getEntry("highlight-plotter")) != 0) {

        ConfEntryList* highlight_user_entries = gource_settings->getEntries("highlight-plotter");

        for(ConfEntryList::iterator it = highlight_user_entries->begin(); it != highlight_user_entries->end(); it++) {

            entry = *it;

            if(!entry->hasValue()) conffile.entryException(entry, "specify highlight-plotter (user)");

            highlight_users.push_back(entry->getString());
        }
    }

    if((entry = gource_settings->getEntry("follow-plotter")) != 0) {

        ConfEntryList* follow_user_entries = gource_settings->getEntries("follow-plotter");

        for(ConfEntryList::iterator it = follow_user_entries->begin(); it != follow_user_entries->end(); it++) {

            entry = *it;

            if(!entry->hasValue()) conffile.entryException(entry, "specify follow-plotter (user)");

            follow_users.push_back(entry->getString());
        }
    }

    if(gource_settings->getBool("show-tld-only")) {
        file_extensions=true;
    }

    if((entry = gource_settings->getEntry("host-filter")) != 0) {

        ConfEntryList* filters = gource_settings->getEntries("host-filter");

        for(ConfEntryList::iterator it = filters->begin(); it != filters->end(); it++) {

            entry = *it;

            if(!entry->hasValue()) conffile.entryException(entry, "specify host-filter (regex)");

            std::string filter_string = entry->getString();

            Regex* r = new Regex(filter_string, 1);

            if(!r->isValid()) {
                delete r;
                conffile.entryException(entry, "invalid host-filter regular expression");
            }

            file_filters.push_back(r);
        }
    }

    if((entry = gource_settings->getEntry("plotter-filter")) != 0) {

        ConfEntryList* filters = gource_settings->getEntries("plotter-filter");

        for(ConfEntryList::iterator it = filters->begin(); it != filters->end(); it++) {

            entry = *it;

            if(!entry->hasValue()) conffile.entryException(entry, "specify plotter-filter (regex)");

            std::string filter_string = entry->getString();

            Regex* r = new Regex(filter_string, 1);

            if(!r->isValid()) {
                delete r;
                conffile.entryException(entry, "invalid plotter-filter regular expression");
            }

            user_filters.push_back(r);
        }
    }

    //validate path
    if(gource_settings->hasValue("path")) {
        path = gource_settings->getString("path");
        default_path = false;
    }

    if(path == "-") {

        if(log_format.size() == 0) {
            throw ConfFileException("log-format required when reading from STDIN", "", 0);
        }

#ifdef _WIN32
        DWORD available_bytes;
        HANDLE stdin_handle = GetStdHandle(STD_INPUT_HANDLE);

        while(PeekNamedPipe(stdin_handle, 0, 0, 0,
            &available_bytes, 0) && available_bytes==0 && !std::cin.fail()) {
            SDL_Delay(100);
        }
#else
        while(std::cin.peek() == EOF && !std::cin.fail()) SDL_Delay(100);
#endif

        std::cin.clear();
       
    } else if(!path.empty() && path != ".") {

        //remove trailing slash
        if(path[path.size()-1] == '\\' || path[path.size()-1] == '/') {
            path.resize(path.size()-1);
        }

        // check path exists
        if(!boost::filesystem::exists(path)) {
            throw ConfFileException(str(boost::format("'%s' does not appear to be a valid file or directory") % path), "", 0);
        }
    }
}

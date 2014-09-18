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

#include "mal4s.h"

Regex mal4s_regex("^(?:\\xEF\\xBB\\xBF)?(-?[0-9]+)\\|([^|]*)\\|([ADM]?)\\|([^|]+)\\|([A-Fa-f0-9]{6})\\|([^|\\n]*)(?:\\|#?([^\\n]*))?");

std::vector<std::string> &Mal4sLog::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> Mal4sLog::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


Mal4sLog::Mal4sLog(const std::string& logfile) : RCommitLog(logfile) {
}

vec3 Mal4sLog::parseColour(const std::string& cstr) {

    vec3 colour;
    int r,g,b;

    if(sscanf(cstr.c_str(), "%02x%02x%02x", &r, &g, &b) == 3) {
        colour = vec3( r, g, b );
        colour /= 255.0f;
    }

    return colour;
}

// parse modified cvs format log entries

bool Mal4sLog::parseCommit(RCommit& commit) {

    while(parseCommitEntry(commit));

    return !commit.files.empty();
}

bool Mal4sLog::parseCommitEntry(RCommit& commit) {

    std::string line;
    std::vector<std::string> entries;

    if(!getNextLine(line)) return false;

    //custom line
    if(!mal4s_regex.match(line, &entries)) return false;

    long timestamp       = atol(entries[0].c_str());

    std::string username = (entries[1].size()>0) ? entries[1] : "Unknown";
    std::string action   = (entries[2].size()>0) ? entries[2] : "A";

    //if this file is for the same person and timestamp
    //we add to the commit, else we save the lastline
    //and return false
    if(commit.files.empty()) {
        commit.timestamp = timestamp;
        commit.username  = username;
    } else {
        if(commit.timestamp != timestamp || commit.username  != username) {
            lastline = line;
            return false;
        }
    }

    if(entries.size()>=6 && entries[5].size()>0) {
	commit.userimagename = entries[5];
    } else commit.userimagename = commit.username;

    std::vector<std::string> displayData;

    //Extra fields for display on hover.
    if(entries.size()>=7 ) {
	displayData = split(entries[6], '|');
    }

    bool has_colour = false;
    vec3 colour;

    if(entries.size()>=5 && entries[4].size()>0) {
        has_colour = true;
        colour = parseColour(entries[4]);
    }

    std::string imageName;
    //Select the image name to display on hover from only one field
    if(gGourceSettings.host_image_dir.empty()) {
	imageName = "";
    } else if(gGourceSettings.hostimage_field == "plotter") {
	imageName = entries[1];
    } else if(gGourceSettings.hostimage_field == "plotter-image") {
	imageName = commit.userimagename;
    } else if(gGourceSettings.hostimage_field == "host") {
	size_t hbegin = entries[3].find_last_of("/");
	if(hbegin != std::string::npos) {
		imageName = entries[3].substr(hbegin+1);
	}
    } else if(gGourceSettings.hostimage_field == "tld") {
	size_t tbegin = entries[3].find_last_of(".");
	if(tbegin != std::string::npos  && tbegin+1 < entries[3].size()) {
		imageName = entries[3].substr(tbegin+1);
	}
    } else if(gGourceSettings.hostimage_field.size() > 1 && gGourceSettings.hostimage_field.compare(0,1,"b") == 0 &&  gGourceSettings.hostimage_field.substr(1).find_first_not_of("0123456789") == std::string::npos) {
	std::vector<std::string> branches = split(entries[3], '/');
#ifndef _GLIBCXX_HAVE_BROKEN_VSWPRINTF
	unsigned long fnum = std::stoul(gGourceSettings.hostimage_field.substr(1));
#else
	unsigned long fnum;
	std::stringstream ss(gGourceSettings.hostimage_field.substr(1).c_str());
	ss >> fnum;
#endif

	if(fnum == 0) {
		imageName = "";
	} else if(branches.size() >= 3 && fnum < branches.size() - 3) {
		imageName = branches[fnum - 1];
	} else imageName = "";
    } else if(gGourceSettings.hostimage_field.size() > 1 && gGourceSettings.hostimage_field.compare(0,1,"n") == 0 &&  gGourceSettings.hostimage_field.substr(1).find_first_not_of("0123456789") == std::string::npos) {
#ifndef _GLIBCXX_HAVE_BROKEN_VSWPRINTF
	unsigned long fnum = std::stoul(gGourceSettings.hostimage_field.substr(1));
#else
	unsigned long fnum;
	std::stringstream ss(gGourceSettings.hostimage_field.substr(1).c_str());
	ss >> fnum;
#endif
	if(fnum == 0) {
		imageName = "";
	} else if(displayData.size() >= 3 && fnum < displayData.size() - 3) {
		imageName = displayData[fnum - 1];
	} else imageName = "";
    } else {
	imageName = "";
    }
    if(has_colour) {
        commit.addFile(entries[3], action, colour, commit.username, imageName, displayData);
    } else {
        commit.addFile(entries[3], action, commit.username, imageName, displayData);
    }

    return true;
}

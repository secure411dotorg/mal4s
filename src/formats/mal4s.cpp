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

//Regex mal4s_regex("^(?:\\xEF\\xBB\\xBF)?(-?[0-9]+)\\|([^|]*)\\|([ADM]?)\\|([^|]+)(?:\\|([A-F0-9]{6})\\|([^|]*)\\|([^|]*)\\|([^|]*)\\|([^|]*)\\|([^|]*))?");
Regex mal4s_regex("^(?:\\xEF\\xBB\\xBF)?(-?[0-9]+)\\|([^|]*)\\|([ADM]?)\\|([^|]+)(?:\\|([A-F0-9]{6})\\|#?([^|\\n]*)\\|#?([^|\\n]*)\\|#?([^|\\n]*))?");

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

//    displayData.push_back("Super Duper!");

//fprintf(stdout, "%s\n", displayData[0].c_str());
    
    //Extra fields for display on hover.
    if(entries.size()>=7 ) {
	for(unsigned int it = 6;it < entries.size(); it++) {
		displayData.push_back(entries[it]);
	}
    } 

    bool has_colour = false;
    vec3 colour;

    if(entries.size()>=5 && entries[4].size()>0) {
        has_colour = true;
        colour = parseColour(entries[4]);
    }

    if(has_colour) {
        commit.addFile(entries[3], action, colour, commit.username, displayData);
    } else {
        commit.addFile(entries[3], action, commit.username, displayData);
    }

    return true;
}

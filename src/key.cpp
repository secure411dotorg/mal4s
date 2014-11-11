#include "key.h"


// File Key Entry
// a string for the file ext and a colour

FileKeyEntry::FileKeyEntry(const FXFont& font, const std::string& ext, const vec3& colour) {
    this->ext    = ext;
    this->colour = colour;
    this->pos_y  = -1.0f;

    this->font = font;
    this->font.dropShadow(false);

    shadow      = vec2(3.0, 3.0);

    width       = gGourceSettings.keyWidth;
    height      = 18.0f;
    left_margin = 20.0f;
    count       = 0;
    brightness  = 1.0f;
    alpha       = 0.0f;

    move_elapsed = 1.0f;
    src_y        = -1.0f;
    dest_y       = -1.0f;

    show = true;

    display_ext = ext;

    bool truncated = false;

    while(font.getWidth(display_ext) > width - 15.0f) {
        display_ext.resize(display_ext.size()-1);
        truncated = true;
    }

    if(truncated) {
        display_ext += std::string("...");
    }
}

const vec3& FileKeyEntry::getColour() const {
    return colour;
}

const std::string& FileKeyEntry::getExt() const {
    return ext;
}

void FileKeyEntry::setShow(bool show) {
    this->show = show;
}

bool FileKeyEntry::isFinished() const {
    return (count<=0 && alpha <= 0.0f);
}

void FileKeyEntry::colourize() {
    colour = ext.empty() ? vec3(1.0f, 1.0f, 1.0f) : colourHash(ext);
}

void FileKeyEntry::inc() {
    count++;
}

void FileKeyEntry::dec() {
    count--;
}

int FileKeyEntry::getCount() const {
    return count;
}

void FileKeyEntry::setCount(int count) {
    this->count = count;
}

void FileKeyEntry::setDestY(float dest_y) {
    if(dest_y == this->dest_y) return;

    this->dest_y = dest_y;

    src_y = pos_y;
    move_elapsed = 0.0f;
}


void FileKeyEntry::logic(float dt) {

    if(count<=0 || !show) {
        alpha = std::max(0.0f, alpha - dt);
    } else if(alpha < 1.0f) {
        alpha = std::min(1.0f, alpha + dt);
    }

    //move towards dest
    if(pos_y != dest_y) {
        //initialize pos from dest if new
        if(pos_y < 0.0f) pos_y = dest_y;
        else {
            move_elapsed += dt;

            if(move_elapsed >= 1.0f) pos_y = dest_y;
            else pos_y = src_y + (dest_y - src_y) * move_elapsed;
        }
    }

    pos = vec2(alpha * left_margin, pos_y);
}

void FileKeyEntry::draw() {
    if(isFinished()) return;
    //label.draw();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, alpha * 0.333f);

    glPushMatrix();
        glTranslatef(shadow.x, shadow.y, 0.0f);

        glBegin(GL_QUADS);
            glVertex2f(pos.x,       pos.y);
            glVertex2f(pos.x,       pos.y + height);
            glVertex2f(pos.x+width, pos.y + height);
            glVertex2f(pos.x+width, pos.y);
        glEnd();
    glPopMatrix();

    glBegin(GL_QUADS);
        glColor4f(colour.x * 0.5f, colour.y * 0.5f, colour.z * 0.5f, alpha);
        glVertex2f(pos.x,         pos.y);
        glVertex2f(pos.x,         pos.y + height);
        glColor4f(colour.x, colour.y, colour.z, alpha);
        glVertex2f(pos.x + width, pos.y + height);
        glVertex2f(pos.x + width, pos.y);
    glEnd();

    glEnable(GL_TEXTURE_2D);

    font.setColour(vec4(1.0f, 1.0f, 1.0f, alpha));

    font.dropShadow(false);
    font.draw((int)pos.x+2, (int)pos.y+3,  display_ext.c_str());

    font.dropShadow(true);
    font.print((int)pos.x+width+4, (int)pos.y+3, "%d", count);
}

// Key

//maintain a key of all the current file types, updated periodically.
//new entries slide in and out / fade in fade out

FileKey::FileKey() {

}

FileKey::FileKey(float update_interval) {
    this->update_interval = update_interval;
    interval_remaining = 1.0f;
    font = fontmanager.grab("FreeSans.ttf", 16);
    font.dropShadow(false);
    font.roundCoordinates(false);
    show = true;
}

FileKey::~FileKey() {
    active_keys.clear();

    for(std::map<std::string, FileKeyEntry*>::iterator it = keymap.begin(); it != keymap.end(); it++) {
        FileKeyEntry* entry = it->second;
        delete entry;
    }
    keymap.clear();

}

void FileKey::setShow(bool show) {
    this->show = show;

    for(std::vector<FileKeyEntry*>::iterator it = active_keys.begin(); it != active_keys.end(); it++) {

        FileKeyEntry* entry = *it;

        entry->setShow(show);
    }
    interval_remaining = 0.0f;
}

void FileKey::colourize() {
    for(std::vector<FileKeyEntry*>::iterator it = active_keys.begin(); it != active_keys.end(); it++) {
        FileKeyEntry* entry = *it;
        entry->colourize();
    }
}

void FileKey::clear() {

    for(std::vector<FileKeyEntry*>::iterator it = active_keys.begin(); it != active_keys.end(); it++) {
        FileKeyEntry* entry = *it;
        entry->setCount(0);
    }

    interval_remaining = 0.0f;
}

std::vector<std::string> &FileKey::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> FileKey::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string FileKey::parseKey(RFile* file) {
	std::string fieldIdentifier;
	std::string keyFormat = gGourceSettings.keyFormat;
	std::string parsedKeyEntry;
/*
These fields are stored in a vector beginning at 0, so subtract 1
plotter = ${plotter}
host.tld = ${host}
tld = ${tld}
Non-branching field = ${nNUM}
Branching field = ${bNUM}
*/

	std::string path = file->path;
	// Erase leading slash from the path
        path.erase(0,1);

	// Convert "path" to separate fields
	std::vector<std::string> branching = split(path, '/');

	std::vector<std::string> nonBranching = file->displayData;

   //Loop though line and replace all of the ${FIELDNUM}s with the corresponding field
   for(std::size_t last = 0; last != std::string::npos;) {
	std::size_t open = keyFormat.find("${", last);
	if(open != std::string::npos) {
	   std::size_t close = keyFormat.find("}", open);
	   //Make sure it is only number enclosed in ${FIELDNUM}
	   if(close != std::string::npos) {
		fieldIdentifier = keyFormat.substr(open + 2, close - open - 2);
		if(fieldIdentifier.compare("plotter-image") == 0) {
		   parsedKeyEntry += keyFormat.substr(last, open - last) + file->imageName;
		   last = close + 1;
		} else if(fieldIdentifier.compare("plotter") == 0) {
		   parsedKeyEntry += keyFormat.substr(last, open - last) + file->fileUser;
		   last = close + 1;
		} else if(fieldIdentifier.compare("host") == 0) {
		   parsedKeyEntry += keyFormat.substr(last, open - last) + file->getName();
		   last = close + 1;
		} else if(fieldIdentifier.compare("tld") ==0) {
		   parsedKeyEntry += keyFormat.substr(last, open - last) + file->ext;
		   last = close + 1;
		} else if(fieldIdentifier.compare(0,1,"n") == 0 && fieldIdentifier.substr(1).find_first_not_of("0123456789") == std::string::npos) {
		   //Convert the string to an unigned int
#ifndef _GLIBCXX_HAVE_BROKEN_VSWPRINTF
		   unsigned long fieldnum = std::stoul(fieldIdentifier.substr(1)) - 1;
#else
		   unsigned long fieldnum;
		   std::stringstream ss(fieldIdentifier.substr(1).c_str());
		   ss >> fieldnum;
		   fieldnum = fieldnum - 1;
#endif
		   //Test if the field is in range
		   if(nonBranching.size() > fieldnum) {
			   //Yes, append from last up to ${, and the replacement field
			   parsedKeyEntry += keyFormat.substr(last, open - last) + nonBranching[fieldnum];
			   last = close + 1;
		   } else if(gGourceSettings.hoverUnsetField.size() != 0) {
			   //No, append from last up to ${ and the replacement for an unset field
			   parsedKeyEntry += keyFormat.substr(last, open - last) + gGourceSettings.hoverUnsetField;
			   last = close + 1;
		   } else {
			   //No, unset field is blank, so append up to ${
			   parsedKeyEntry += keyFormat.substr(last, open - last);
			   last = close + 1;
		   }
		} else if(fieldIdentifier.compare(0,1,"b") == 0 && fieldIdentifier.substr(1).find_first_not_of("0123456789") == std::string::npos) {
		   //Convert the string to an unigned int
#ifndef _GLIBCXX_HAVE_BROKEN_VSWPRINTF
		   unsigned long fieldnum = std::stoul(fieldIdentifier.substr(1)) - 1;
#else
		   unsigned long fieldnum;
		   std::stringstream ss(fieldIdentifier.substr(1).c_str());
		   ss >> fieldnum;
		   fieldnum = fieldnum - 1;
#endif
		   //Test if the field is in range
		   if(branching.size() > fieldnum) {
			   //Yes, append from last up to ${, and the replacement field
			   parsedKeyEntry += keyFormat.substr(last, open - last) + branching[fieldnum];
			   last = close + 1;
		   } else if(gGourceSettings.hoverUnsetField.size() != 0) {
			   //No, append from last up to ${ and the replacement for an unset field
			   parsedKeyEntry += keyFormat.substr(last, open - last) + gGourceSettings.hoverUnsetField;
			   last = close + 1;
		   } else {
			   //No, unset field is blank, so append up to ${
			   parsedKeyEntry += keyFormat.substr(last, open - last);
			   last = close + 1;
		   }
		} else {
			//The formatting does not point to a field, copy raw formatting
			parsedKeyEntry += keyFormat.substr(last, close - last);
			last = close + 1;
		}
	   }
	} else {
		//${ was not found copy the rest of the formatting
		parsedKeyEntry += keyFormat.substr(last);
		last = std::string::npos;
	}
   }

	return parsedKeyEntry;
}

void FileKey::inc(RFile* file) {

    FileKeyEntry* entry = 0;

    //This is the label displayed in the key
    std::string key_display = parseKey(file);

//    std::map<std::string, FileKeyEntry*>::iterator result = keymap.find(file->ext);
    std::map<std::string, FileKeyEntry*>::iterator result = keymap.find(key_display);

    if(result != keymap.end()) {
        entry = result->second;
    } else {
//        entry = new FileKeyEntry(font, file->ext, file->getFileColour());
        entry = new FileKeyEntry(font, key_display, file->getFileColour());
//        keymap[file->ext] = entry;
        keymap[key_display] = entry;
    }

    entry->inc();
}


//decrement count of extension. if drops to zero, mark it for removal
void FileKey::dec(RFile* file) {
    //This is what is the label displayed in the key
    std::string key_display = parseKey(file);

    std::map<std::string, FileKeyEntry*>::iterator result = keymap.find(key_display);

    if(result == keymap.end()) return;

    FileKeyEntry* entry = result->second;

    entry->dec();
}

bool file_key_entry_sort (const FileKeyEntry* a, const FileKeyEntry* b) {

    //sort by count
    if(a->getCount() != b->getCount())
        return (a->getCount() > b->getCount());

    //then by name (tie breaker)
    return a->getExt().compare(b->getExt()) < 0;
}

void FileKey::logic(float dt) {

    interval_remaining -= dt;

    //recalculate active_keys
    if(interval_remaining <= 0.0f) {

        if(show) {
            active_keys.clear();
            std::vector<FileKeyEntry*> finished_keys;

            for(std::map<std::string, FileKeyEntry*>::iterator it = keymap.begin(); it != keymap.end(); it++) {
                FileKeyEntry* entry = it->second;

                if(!entry->isFinished()) {
                    active_keys.push_back(entry);
                } else {
                    finished_keys.push_back(entry);
                }
            }

            //sort
            std::sort(active_keys.begin(), active_keys.end(), file_key_entry_sort);

            //limit to entries we can put onto the screen
            int max_visible_entries = std::max(0, (int)((display.height - 150.0f) / 20.0f));

            if (active_keys.size() > max_visible_entries) {
                active_keys.resize(max_visible_entries);
            }

            //set position

            float key_y = 20.0f;

            for(std::vector<FileKeyEntry*>::iterator it = active_keys.begin(); it != active_keys.end(); it++) {
                FileKeyEntry* entry = *it;
                if(entry->getCount()>0) {
                    entry->setDestY(key_y);
                }
                key_y += 20.0f;
            }

            //remove and delete finished entries
            for(std::vector<FileKeyEntry*>::iterator it = finished_keys.begin(); it != finished_keys.end(); it++) {
                FileKeyEntry* entry = *it;
                keymap.erase(entry->getExt());
                delete entry;
            }
        }

        interval_remaining = update_interval;
    }

    for(std::vector<FileKeyEntry*>::iterator it = active_keys.begin(); it != active_keys.end(); it++) {

        FileKeyEntry* entry = *it;

        entry->logic(dt);
    }
}

void FileKey::draw() {

    for(std::vector<FileKeyEntry*>::iterator it = active_keys.begin(); it != active_keys.end(); it++) {
        FileKeyEntry* entry = *it;

        entry->draw();
    }

}

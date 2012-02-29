#include "access.h"


FileAccess::FileAccess(std::string fileName) {
    mediaPath = fileName;
    mediaFile = NULL;
}

FileAccess::~FileAccess() {
    if ( mediaFile != NULL)
        fclose(mediaFile);
}

bool FileAccess::Open() {
    return false;
}

bool FileAccess::Start() {
    return false;
}

bool FileAccess::Stop() {
    return false;
}





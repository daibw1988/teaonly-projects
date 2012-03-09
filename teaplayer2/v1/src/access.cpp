#include "access.h"


FileAccess::FileAccess(std::string fileName) {
    mediaPath = fileName;
    mediaFile = NULL;

    thread = new talk_base::Thread();
    thread->Start();
}

FileAccess::~FileAccess() {
    if ( mediaFile != NULL)
        fclose(mediaFile);
}

bool FileAccess::Open() {
    thread->PostDelayed(10, this, MSG_ACCESS_START);
    return true;
}

void FileAccess::Close() {
    thread->Clear(this);    
    mediaFile = NULL;
}

void FileAccess::OnMessage(talk_base::Message *msg) {
    switch( msg->message_id ) {
        case MSG_ACCESS_START:
            mediaFile = fopen( mediaPath.c_str() , "rb"); 
            if ( mediaFile == NULL) {
                signalBeginofStream(false);
                return;
            }
            signalBeginofStream(true);
            thread->PostDelayed(80, this, MSG_ACCESS_TIMER);
            break;
        case MSG_ACCESS_TIMER:
            doAccess();
            break;
    }        
}

void FileAccess::doAccess() {
    static unsigned char buffer[1024*6];        
    
    if ( mediaFile == NULL)
        return;

    if ( feof(mediaFile) ) {
        mediaFile = NULL;
        signalEndOfStream();
        return;
    }  
    
    int ret = fread(buffer, 1, sizeof(buffer), mediaFile);
    if ( ret > 0) {
        signalData(buffer, ret); 
        thread->PostDelayed(80, this, MSG_ACCESS_TIMER);
    }
}



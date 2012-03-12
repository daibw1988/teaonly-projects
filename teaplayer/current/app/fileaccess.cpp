#include "fileaccess.h"

FileAccess::FileAccess(std::string fileName) {
    mediaPath = fileName;
    mediaFile = NULL;

    thread = new talk_base::Thread();
    thread->Start();
}

FileAccess::~FileAccess() {
    delete thread;
    if ( mediaFile != NULL)
        fclose(mediaFile);
}

bool FileAccess::Open() {
    thread->PostDelayed(10, this, MSG_ACCESS_START);
    return true;
}

void FileAccess::Close() {
    FILE *temp = mediaFile;
    mediaFile = NULL;
    thread->Stop();

    if ( temp != NULL) {
        fclose(temp);
        mediaFile = NULL;
    }
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
        case MSG_ACCESS_STOP:
            thread->Clear(this);
            break;
    }        
}

void FileAccess::doAccess() {
    static unsigned char buffer[1024*6];        
    FILE *target = mediaFile; 

    if ( target == NULL)
        return;

    if ( feof(target) ) {
        mediaFile = NULL;
        signalEndOfStream();
        return;
    }  
    
    int ret = fread(buffer, 1, sizeof(buffer), target);
    if ( ret > 0) {
        signalData(buffer, ret); 
        thread->PostDelayed(80, this, MSG_ACCESS_TIMER);
    }
}


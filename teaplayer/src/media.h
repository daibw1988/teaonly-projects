#include <string>

class TeaMedia {
public:    
    TeaMedia();
    bool open(std::string &mrl);

public:    
    inline bool isParsed() {
        return parsed;
    }
    
private:
    bool parsed; 
    std::string mrl;
};

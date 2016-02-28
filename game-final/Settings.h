// Settings.h
//
//
#ifndef Settings_h
#define Settings_h

#include <string>

class Settings
{
    private:
        void host();
        void client();
    public:
        std::string port;
        std::string ipAddress;
        int type;
        Settings(int argc, char** argv);
};
#endif //Settings_h

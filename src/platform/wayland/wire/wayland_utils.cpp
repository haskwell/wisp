#include "wayland_utils.hpp"
#include <string>
#include <cstdlib>
#include <stdexcept>
std::string get_path() {
    const char* display = std::getenv("WAYLAND_DISPLAY");

    if (!display || display[0] == '\0') display = "wayland-0";
    
    std::string path{};

    if (display[0] == '/'){
        path = display;
    }
    else{
        const char* runtime = std::getenv("XDG_RUNTIME_DIR");
        if(!runtime){
            throw std::runtime_error("'XDG_RUNTIME_DIR' not set");
        }
        path = runtime;
        path += '/';
        path += display;
    }
    return path;
}

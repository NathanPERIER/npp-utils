
#pragma once

#include <exception>


namespace npp {

class unreachable : public std::exception {
public:
    const char* what() {
        return "Entered theoretically unreachable section";
    }
};

} // namespace npp

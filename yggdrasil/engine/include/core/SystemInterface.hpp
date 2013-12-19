#ifndef _YGG_SYSTEMINTERFACE_HPP
#define _YGG_SYSTEMINTERFACE_HPP

#include "assets/Image.hpp"
#include "HumanInterfaceDevice.hpp"

namespace ygg {

class SystemInterface {
public:
    virtual HumanInterfaceDevice& getHID() = 0;

    virtual bool loadImage(const char* path, Image& image) = 0;

    virtual std::string getApplicationPath() const = 0;

    virtual void getWindowSize(size_t& w, size_t& h) const = 0;
};

}

#endif // _YGG_SYSTEMINTERFACE_HPP

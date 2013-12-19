#ifndef _YGG_IMAGE_HPP
#define _YGG_IMAGE_HPP

#include <cstdint>
#include <vector>

namespace ygg {

struct RGBA {
    uint8_t r, g, b, a;

    RGBA() {
    }

    RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
        r(r), g(g), b(b), a(a) {
    }

    RGBA(const uint8_t* ptr):
        r(ptr[0]), g(ptr[1]), b(ptr[2]), a(ptr[3]) {
    }
};

class Image {
public:
    Image():
        m_nWidth(0), m_nHeight(0) {
    }

    Image(uint32_t width, uint32_t height, const uint8_t* pixels):
        m_nWidth(width), m_nHeight(height), m_Pixels(pixels, pixels + width * height * 4) {
    }

    const RGBA operator ()(uint32_t x, uint32_t y) const {
        return RGBA(&m_Pixels[4 * (x + y * m_nWidth)]);
    }

    uint32_t getWidth() const {
        return m_nWidth;
    }

    uint32_t getHeight() const {
        return m_nHeight;
    }

    const uint8_t* getPixels() const {
        return m_Pixels.data();
    }

    void setTransparentColor(uint8_t r, uint8_t g, uint8_t b) {
        for(auto it = begin(m_Pixels); it != end(m_Pixels); it += 4) {
            if(it[0] == r && it[1] == g && it[2] == b) {
                it[3] = 0;
            } else {
                it[3] = 255;
            }
        }
    }

private:
    uint32_t m_nWidth, m_nHeight;
    std::vector<uint8_t> m_Pixels;
};

}

#endif

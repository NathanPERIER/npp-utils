#include "npp/colour/conv.hh"

#include <algorithm>
#include <cmath>


namespace {

struct rgb_hue_base {
    double min;
    double max;
    double chroma;
    double hue;
};

rgb_hue_base get_rgb_hue_base(const npp::rgb& col) {
    const uint8_t min = std::min(std::min(col.r, col.g), col.b);
    const uint8_t max = std::max(std::max(col.r, col.g), col.b);
    const double chroma = static_cast<double>(max - min);
    double hue = 0;
    if(chroma != 0) {
        if(max == col.r) {
            hue = std::fmod(static_cast<double>(col.g - col.b) / chroma, 6);
        } else if(max == col.g) {
            hue = (static_cast<double>(col.b - col.r) / chroma) + 2.0;
        } else {
            hue = (static_cast<double>(col.r - col.g) / chroma) + 4.0;
        }
    }
    return {
        .min = static_cast<double>(min) / 255.0,
        .max = static_cast<double>(max) / 255.0,
        .chroma = chroma,
        .hue = hue * 60.0
    };
}


struct uncorrected_rgb_hue_chroma {
    double r;
    double g;
    double b;
};

uncorrected_rgb_hue_chroma partial_rgb_from_hue_chroma(double hue, double chroma) {
    const double h = hue / 60;
    const double x = chroma * (1 - std::abs(std::fmod(h, 2) - 1));
    if(h < 1) { return { .r = chroma, .g = x,      .b = 0      }; }
    if(h < 2) { return { .r = x,      .g = chroma, .b = 0      }; }
    if(h < 3) { return { .r = 0,      .g = chroma, .b = x      }; }
    if(h < 4) { return { .r = 0,      .g = x,      .b = chroma }; }
    if(h < 5) { return { .r = x,      .g = 0,      .b = chroma }; }
                return { .r = chroma, .g = 0,      .b = x      };
}

npp::rgb rgb_from_cylindrical_params(double hue, double chroma, double min) {
    const uncorrected_rgb_hue_chroma uncorrected_col = partial_rgb_from_hue_chroma(hue, chroma);
    const auto correct_channel = [min](double uncorrected_channel) -> uint8_t {
        return static_cast<uint8_t>(std::round((uncorrected_channel + min) * 255.0));
    };
    return {
        .r = correct_channel(uncorrected_col.r),
        .g = correct_channel(uncorrected_col.g),
        .b = correct_channel(uncorrected_col.b),
    };
}



} // anonymous namespace


namespace npp {

hsl conversion<rgb, hsl>::operator()(const rgb& col) const {
    const ::rgb_hue_base base = ::get_rgb_hue_base(col);
    const double lightness = (base.min + base.max) / 2.0;
    const double saturation = base.chroma / (1 - std::abs(2.0 * lightness - 1.0));
    return {
        .h = base.hue,
        .s = saturation,
        .l = lightness
    };
}

hsv conversion<rgb, hsv>::operator()(const rgb& col) const {
    const ::rgb_hue_base base = ::get_rgb_hue_base(col);
    return {
        .h = base.hue,
        .s = base.chroma / base.max, // = chroma / value
        .v = base.max
    };
}


rgb conversion<hsl, rgb>::operator()(const hsl& col) const {
    const double chroma = (1.0 - std::abs(2 * col.l - 1)) * col.s;
    const double min = col.l - chroma / 2;
    return ::rgb_from_cylindrical_params(col.h, chroma, min);
}

rgb conversion<hsv, rgb>::operator()(const hsv& col) const {
    const double chroma = col.v * col.s;
    const double min = col.v - chroma;
    return ::rgb_from_cylindrical_params(col.h, chroma, min);
}


hsv conversion<hsl, hsv>::operator()(const hsl& col) const {
    const double v = col.l + col.s * std::min(col.l, 1.0 - col.l);
    return {
        .h = col.h,
        .s = (v == 0) ? 0 : 2.0 * (1.0 - col.l / v),
        .v = v
    };
}

hsl conversion<hsv, hsl>::operator()(const hsv& col) const {
    const double l = col.v * (1.0 - col.s / 2.0);
    return {
        .h = col.h,
        .s = (l == 0 || l == 1) ? 0 : ((col.v - l) / std::min(l, 1.0 - l)),
        .l = l
    };
}

} // namespace npp;

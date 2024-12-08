
#pragma once

#include "npp/conv/core.hh"
#include "npp/colour/hsl.hh"
#include "npp/colour/hsv.hh"
#include "npp/colour/rgb.hh"


namespace npp {

template <>
struct conversion<rgb, rgba> {
    rgba operator()(const rgb& col) const {
        return {
            .r = col.r,
            .g = col.g,
            .b = col.b,
            .a = 255
        };
    }
};

template <>
struct conversion<rgba, rgb> {
    rgb operator()(const rgba& col) const {
        return {
            .r = col.r,
            .g = col.g,
            .b = col.b
        };
    }
};



template <>
struct conversion<rgb, hsl> {
    hsl operator()(const rgb& col) const;
};

template <>
struct conversion<rgb, hsv> {
    hsv operator()(const rgb& col) const;
};


template <>
struct conversion<hsl, rgb> {
    rgb operator()(const hsl& col) const;
};

template <>
struct conversion<hsv, rgb> {
    rgb operator()(const hsv& col) const;
};


template <>
struct conversion<hsl, hsv> {
    hsv operator()(const hsl& col) const;
};

template <>
struct conversion<hsv, hsl> {
    hsl operator()(const hsv& col) const;
};

} // namespace npp

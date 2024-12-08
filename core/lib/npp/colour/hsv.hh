
#pragma once


namespace npp {

struct hsv {
    double h;
    double s;
    double v;

    bool operator==(const hsv& col) const {
        return (h == col.h) && (s == col.s) && (v == col.v);
    }
};

} // namespace npp

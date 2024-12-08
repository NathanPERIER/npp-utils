
#pragma once


namespace npp {

struct hsl {
    double h;
    double s;
    double l;

    bool operator==(const hsl& col) const {
        return (h == col.h) && (s == col.s) && (l == col.l);
    }
};

} // namespace npp

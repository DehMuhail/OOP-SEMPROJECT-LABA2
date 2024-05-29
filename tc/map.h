#ifndef MAP_H
#define MAP_H

#include <cstdlib>

struct Map {
    size_t w, h; // overall map dimensions
    Map(int level, int w, int h);
    int get(const size_t i, const size_t j) const;
    bool is_empty(const size_t i, const size_t j) const;
    int level; 
};

#endif // MAP_H


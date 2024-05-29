#include <cassert>
#include "map.h"
#include <string.h>

static const char* map[2] =
{
"0000222222220000"\
"1              5"\
"1          0   5"\
"1          0   5"\
"000030000000   5"\
"0              5"\
"0   1  0       5"\
"0   3  0       5"\
"5   4  100011  4"\
"5   4   1      4"\
"0       1      4"\
"2       1      4"\
"0     000      4"\
"0              4"\
"0              4"\
"0002222244444444",
"000022222222000005"\
"1                5"\
"1              5 5"\
"1     01111    5 3"\
"0     0        5 2"\
"0     3     1155 1"\
"0   1000       5 0"\
"0   3  0       5 0"\
"5   4  100011  4 0"\
"5   4   1      4 0"\
"0       1      4 4"\
"2       1  44444 3"\
"0     000      4 2"\
"0 111          4 1"\
"0              4 0"\
"0                0"\
"0                0"\
"000000000000000000"\
};
                    
/// <summary>
/// // Map constructor initializes level, width, and height, and checks the integrity of the map string
/// </summary>
/// <param name="level"></param>
/// <param name="w"></param>
/// <param name="h"></param>
Map::Map(int level, int w, int h)
{   
    this->level = level;
    this -> w = w; this->h = h;
        //w = 16, h = 16;
        assert(strlen(map[level-1]) == w * h); // +1 for the null terminated string
 
}

/// <summary>
/// Retrieves the value at the specified map coordinates
/// </summary>
/// <param name="i"></param>
/// <param name="j"></param>
/// <returns></returns>
int Map::get(const size_t i, const size_t j) const {
    assert(i<w && j<h && strlen(map[level-1]) == w * h);
    return map[level-1][i + j * w] - '0';
  
}
/// <summary>
/// // Checks if the specified map coordinates are empty
/// </summary>
/// <param name="i"></param>
/// <param name="j"></param>
/// <returns></returns>
bool Map::is_empty(const size_t i, const size_t j) const {
        assert(i < w&& j < h && strlen(map[level-1]) == w * h);
        return map[level-1] [i + j * w] == ' ';
 
}


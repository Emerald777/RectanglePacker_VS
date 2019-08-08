#ifndef USERDEFINES_H
#define USERDEFINES_H

#include <map>
#include <list>
#include <string>
#include <functional>

#include "rectangle.h"


using InputContainer = std::list<Geometry::Rectangle>;
using OutputContainer = std::multimap<int, Geometry::Rectangle>; /// key is the number of basket

// #define DEBUG
#define RAW_OUT_DATA

#endif // USERDEFINES_H

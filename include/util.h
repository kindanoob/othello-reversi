#ifndef UTIL_H_
#define UTIL_H_

#include <array>

#include "config.h"

namespace Util {
	int BitboardPopcount(u64 b);
	int BitboardPopcountHashTable(u64 b, const std::array<int, 65536>& h);

}

#endif //UTIL_H_
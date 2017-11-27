#include  <iostream>

#include "util.h"



namespace Util {
	int BitboardPopcount(u64 bitboard) {
	    int cnt = 0;
	    while(bitboard) {
	        bitboard &= (bitboard - 1);
	        ++cnt;
	    }
	    return cnt;
	}

    //This function calculates popcount using precomputed hash table of popcounts for 16-bit words
	int BitboardPopcountHashTable(u64 b, const std::array<int, 65536>& h) {
		const int kWordSize = 16;
		const int kBitMask = 0xFFFF; //mask is used to extract the lowest kWordSize bits of the bitboard
		return h[b & kBitMask] + 
		    h[(b >> kWordSize) & kBitMask] + 
		    h[(b >> (2 * kWordSize)) & kBitMask] + 
		    h[(b >> (3 * kWordSize)) & kBitMask];
	}
}
#ifndef INCLUDED_USER
#define INCLUDED_USER

//******************************************************************************
//
//
//      ���[�U�[�i���[�e�B���e�B�[�j
//
//
//******************************************************************************

//------< �C���N���[�h >--------------------------------------------------------
#include <sstream>
#include <bitset>
#include <bitset>
#include <assert.h>
#include <queue>


inline std::string strBit16(const int n)
{
    std::stringstream ss;
    ss << static_cast<std::bitset<16>>(n);
    return ss.str();
}


inline const float& clamp(const float& v, const float& lo, const float& hi) {
    assert(lo <= hi);

    return  (std::max)((std::min)(v, hi), lo);

}

#endif // !INCLUDED_USER  
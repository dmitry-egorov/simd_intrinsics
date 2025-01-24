#pragma once

#ifdef DEBUG
    #include <cassert>
    #define lu_assert(expr) assert(expr)
#else
    #define lu_assert(expr)
#endif
#ifndef PIRANHA_MEMORY_MANAGEMENT_H
#define PIRANHA_MEMORY_MANAGEMENT_H

#include <assert.h>

#if __APPLE__
#define __int64 long long
#endif

namespace piranha {

    using mem_size = size_t;

    constexpr mem_size KB = 1000;
    constexpr mem_size MB = 1000 * KB;
    constexpr mem_size GB = 1000 * MB;

#define CHECK_ALIGNMENT(pointer, required) assert((((mem_size)((char *)(pointer))) % (required)) == 0)

} /* namespace piranha */

#endif /* PIRANHA_MEMORY_MANAGEMENT_H */

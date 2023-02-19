#include "daScript/misc/platform.h"

#include "daScript/misc/smart_ptr.h"

#include <signal.h>

#if DAS_SMART_PTR_ID
    uint64_t das::ptr_ref_count::ref_count_total = 0;
    uint64_t das::ptr_ref_count::ref_count_track = 0;
#endif

#if DAS_SMART_PTR_TRACKER
    std::atomic<uint64_t> das::g_smart_ptr_total {0};
#endif

void os_debug_break()
{
#ifdef _MSC_VER
    __debugbreak();
#else
    raise(SIGTRAP);
#endif
}

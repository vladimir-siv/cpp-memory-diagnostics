#pragma once

#ifdef __MEMORY_DIAGNOSTICS__

#include <cinttypes>

extern bool __check_false_usages__();
extern uint64_t __total_memory_usage__();
extern uint64_t __total_object_usage__();
extern uint64_t __total_array_usage__();

#define __FALSE_USAGES__ __check_false_usages__()
#define __MEMORY_USAGE__ __total_memory_usage__()
#define __OBJECT_USAGE__ __total_object_usage__()
#define __ARRAY_USAGE__ __total_array_usage__()

#else

#define __FALSE_USAGES__ false
#define __MEMORY_USAGE__ 0ull
#define __OBJECT_USAGE__ 0ull
#define __ARRAY_USAGE__ 0ull

#endif

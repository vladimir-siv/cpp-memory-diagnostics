#pragma once

extern bool __check_false_usages__();
#define __FALSE_USAGES__ __check_false_usages__()

extern uint64_t __total_memory_usage__();
#define __MEMORY_USAGE__ __total_memory_usage__()

extern uint64_t __total_object_usage__();
#define __OBJECT_USAGE__ __total_object_usage__()

extern uint64_t __total_array_usage__();
#define __ARRAY_USAGE__ __total_array_usage__()

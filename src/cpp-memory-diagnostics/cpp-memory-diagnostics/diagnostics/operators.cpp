#ifdef __MEMORY_DIAGNOSTICS__

#include <new>
#include <cstdint>

#include "hash_map.h"
using namespace collections;

#define __ALLOCATE__(__VARNAME__, __SIZE__)\
	void* __VARNAME__ = std::malloc(__SIZE__);\
	if (__VARNAME__ == nullptr) throw std::bad_alloc();

struct
{
	uint64_t nothrow_single_new = 0;
	uint64_t nothrow_array_new = 0;
	uint64_t invalid_deletes = 0;
	uint64_t total_memory_usage = 0;
	hash_map<void*, uint64_t> objects;
	hash_map<void*, uint64_t> arrays;
} __tracker;

bool __check_false_usages__()
{
	return
		__tracker.nothrow_single_new
		||
		__tracker.nothrow_array_new
		||
		__tracker.invalid_deletes
	;
}

uint64_t __total_memory_usage__()
{
	return __tracker.total_memory_usage;
}

uint64_t __total_object_usage__()
{
	return __tracker.objects.count();
}

uint64_t __total_array_usage__()
{
	return __tracker.arrays.count();
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t _Size)
{
	__ALLOCATE__(_Block, _Size);
	__tracker.total_memory_usage += _Size;
	__tracker.objects.insert(_Block, _Size);
	return _Block;
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t _Size, std::nothrow_t const&) noexcept
{
	++__tracker.nothrow_single_new;
	return std::malloc(_Size);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t _Size)
{
	__ALLOCATE__(_Block, _Size);
	__tracker.total_memory_usage += _Size;
	__tracker.arrays.insert(_Block, _Size);
	return _Block;
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t _Size, std::nothrow_t const&) noexcept
{
	++__tracker.nothrow_array_new;
	return std::malloc(_Size);
}

void __CRTDECL operator delete(void* _Block) noexcept
{
	try
	{
		uint64_t size = __tracker.objects.remove(_Block);
		__tracker.total_memory_usage -= size;
		std::free(_Block);
	}
	catch (const invalidkeyexception&) { }
	catch (const keynotfoundexception&) { }
}

void __CRTDECL operator delete(void* _Block, std::nothrow_t const&) noexcept
{
	try
	{
		uint64_t size = __tracker.objects.remove(_Block);
		__tracker.total_memory_usage -= size;
		std::free(_Block);
	}
	catch (const invalidkeyexception&) { }
	catch (const keynotfoundexception&) { }
}

void __CRTDECL operator delete[](void* _Block) noexcept
{
	try
	{
		uint64_t size = __tracker.arrays.remove(_Block);
		__tracker.total_memory_usage -= size;
		std::free(_Block);
	}
	catch (const invalidkeyexception&) { }
	catch (const keynotfoundexception&) { }
}

void __CRTDECL operator delete[](void* _Block, std::nothrow_t const&) noexcept
{
	try
	{
		uint64_t size = __tracker.arrays.remove(_Block);
		__tracker.total_memory_usage -= size;
		std::free(_Block);
	}
	catch (const invalidkeyexception&) { }
	catch (const keynotfoundexception&) { }
}

void __CRTDECL operator delete(void* _Block, size_t _Size) noexcept
{
	try
	{
		uint64_t size = __tracker.objects.remove(_Block);
		
		if (size != _Size)
		{
			++__tracker.invalid_deletes;
			return;
		}

		__tracker.total_memory_usage -= size;
		std::free(_Block);
	}
	catch (const invalidkeyexception&) { }
	catch (const keynotfoundexception&) { }
}

void __CRTDECL operator delete[](void* _Block, size_t _Size) noexcept
{
	try
	{
		uint64_t size = __tracker.arrays.remove(_Block);
		
		if (size != _Size)
		{
			++__tracker.invalid_deletes;
			return;
		}

		__tracker.total_memory_usage -= size;
		std::free(_Block);
	}
	catch (const invalidkeyexception&) { }
	catch (const keynotfoundexception&) { }
}

/*#ifdef __cpp_aligned_new
_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t _Size, std::align_val_t _Al)
{

}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t _Size, std::align_val_t _Al, std::nothrow_t const&) noexcept
{

}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t _Size, std::align_val_t _Al)
{

}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t _Size, std::align_val_t _Al, std::nothrow_t const&) noexcept
{

}

void __CRTDECL operator delete(void* _Block, std::align_val_t _Al) noexcept
{

}

void __CRTDECL operator delete(void* _Block, std::align_val_t _Al, std::nothrow_t const&) noexcept
{

}

void __CRTDECL operator delete[](void* _Block, std::align_val_t _Al) noexcept
{

}

void __CRTDECL operator delete[](void* _Block, std::align_val_t _Al, std::nothrow_t const&) noexcept
{

}

void __CRTDECL operator delete(void* _Block, size_t _Size, std::align_val_t _Al) noexcept
{

}

void __CRTDECL operator delete[](void* _Block, size_t _Size, std::align_val_t _Al) noexcept
{

}
//#endif // __cpp_aligned_new*/

#endif

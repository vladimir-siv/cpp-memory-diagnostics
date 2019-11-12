#include <iostream>
#include <memory_diagnostics.h>
using namespace std;

namespace assert
{
	class assertexception : public std::exception
	{
		public: virtual const char* what() const override
		{
			return "Assertion failed.";
		}
	};

	template <typename T1, typename T2> void equal(T1&& v1, T2&& v2)
	{
		if (v1 == v2) return;
		throw assertexception();
	}
	
	template <typename T> void istrue(T&& v)
	{
		if (v) return;
		throw assertexception();
	}

	template <typename T> void isfalse(T&& v)
	{
		if (!v) return;
		throw assertexception();
	}
};

inline void a(uint64_t memory_u = 0, uint64_t object_u = 0, uint64_t array_u = 0)
{
	assert::equal(__MEMORY_USAGE__, memory_u);
	assert::equal(__OBJECT_USAGE__, object_u);
	assert::equal(__ARRAY_USAGE__, array_u);
	assert::isfalse(__FALSE_USAGES__);
}

struct test
{
	char c;
	int i;
	float f;

	test() : c(0), i(0), f(0.0f) { }
	test(char _c, int _i, float _f) : c(_c), i(_i), f(_f) { }
};

int main(void)
{
	try
	{
		uint64_t iref = sizeof(int);
		uint64_t fref = sizeof(float);
		uint64_t tref = sizeof(test);

		int* p;
		float* q;
		test* t1;
		test* t2;

		a();

		p = new int(6);
		q = new float(8);

		a(iref + fref, 2, 0);

		delete p;
		delete q;

		a();

		p = new int[10];
		q = new float(5);

		a(iref * 10 + fref, 1, 1);

		delete[] p;
		delete q;

		a();

		p = new int[20];
		q = new float[30];
		t1 = new test;

		a(iref * 20 + fref * 30 + tref, 1, 2);

		delete[] p;
		delete[] q;
		delete t1;

		a();

		p = new int(5);
		q = new float(8);
		t1 = new test;
		t2 = new test[10];

		a(iref + fref + tref + tref * 10, 3, 1);

		delete p;
		delete q;
		delete t1;
		delete[] t2;

		a();

		cout << "Success!" << endl;
	}
	catch (const exception& ex) { cout << "Failure: " << ex.what() << endl; }

	return 0;
}

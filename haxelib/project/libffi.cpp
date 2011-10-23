#ifndef IPHONE
	#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif
#include <hx/CFFI.h>

#ifdef __cplusplus
extern "C" 
{
#endif
#include <ffi.h>
#ifdef __cplusplus
}
#endif

static int floating(int a, float b, double c, long double d)
{
  int i;

  i = (int) ((float)a/b + ((float)c/(float)d));

  return i;
}

value test_ffi()
{
	ffi_cif cif;
	ffi_type *args[4];
	void *values[4];
	ffi_arg rint;

	float f;
	signed int si1;
	double d;
	long double ld;

	args[0] = &ffi_type_sint;
	values[0] = &si1;
	args[1] = &ffi_type_float;
	values[1] = &f;
	args[2] = &ffi_type_double;
	values[2] = &d;
	args[3] = &ffi_type_longdouble;
	values[3] = &ld;

	/* Initialize the cif */
	if(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 4,
	     &ffi_type_sint, args) != FFI_OK)
	{
		return NULL;
	}

	si1 = 6;
	f = 3.14159;
	d = (double)1.0/(double)3.0;
	ld = 2.71828182846L;

	floating (si1, f, d, ld);

	ffi_call(&cif, FFI_FN(floating), &rint, values);

	printf ("%d vs %d\n", (int)rint, floating (si1, f, d, ld));

	return alloc_int((int)rint == floating(si1, f, d, ld));
}

DEFINE_PRIM(test_ffi, 0);

// Reference this to bring in all the symbols for the static library
extern "C" int hxffi_register_prims() { return 0; }


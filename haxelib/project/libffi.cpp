#ifndef STATIC_LINK
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
#include <ffi_common.h>

#ifndef HX_WINDOWS
#	include <dlfcn.h>
#else
#	undef ERROR
#	include <windows.h>
#	define dlopen(l,p)		(void*)LoadLibrary(l)
#	define dlsym(h,n)		GetProcAddress((HMODULE)h,n)
#	define dlclose(h)		FreeLibrary((HMODULE)h)
#endif

#if defined HX_WINDOWS
	#define HXFFI_EXPORT __declspec(dllexport)
#else
  #define HXFFI_EXPORT __attribute__ ((__visibility__("default")))
#endif

typedef enum
{
	hxffi_zero, //zero
	hxffi_type_void,
	//The type void. This cannot be used for argument types, only for return values. 
	hxffi_type_uint8,
	//An unsigned, 8-bit integer type. 
	hxffi_type_sint8,
	//A signed, 8-bit integer type. 
	hxffi_type_uint16,
	//An unsigned, 16-bit integer type. 
	hxffi_type_sint16,
	//A signed, 16-bit integer type. 
	hxffi_type_uint32,
	//An unsigned, 32-bit integer type. 
	hxffi_type_sint32,
	//A signed, 32-bit integer type. 
	hxffi_type_uint64,
	//An unsigned, 64-bit integer type. 
	hxffi_type_sint64,
	//A signed, 64-bit integer type. 
	hxffi_type_float,
	//The C float type. 
	hxffi_type_double,
	//The C double type. 
	hxffi_type_uchar,
	//The C unsigned char type. 
	hxffi_type_schar,
	//The C signed char type. (Note that there is not an exact equivalent to the C char type in libffi; ordinarily you should either use hxffi_type_schar or hxffi_type_uchar depending on whether char is signed.) 
	hxffi_type_ushort,
	//The C unsigned short type. 
	hxffi_type_sshort,
	//The C short type. 
	hxffi_type_uint,
	//The C unsigned int type. 
	hxffi_type_sint,
	//The C int type. 
	hxffi_type_ulong,
	//The C unsigned long type. 
	hxffi_type_slong,
	//The C long type. 
	hxffi_type_pointer,
	//A generic void * pointer. You should use this for all pointers, regardless of their real type.
	hxffi_type_longdouble //won't be supported
	//On platforms that have a C long double type, this is defined. On other platforms, it is not.
} hxffi_types;

typedef struct {
        int size;
        ffi_type **arr;
		  hxffi_types *native_types;
} hxffi_type_array;

typedef struct {
	ffi_cif *cif;
	hxffi_type_array *type_array;
} hxffi_cif;

DECLARE_KIND(k_hxffi_pointer);
DEFINE_KIND(k_hxffi_pointer);

DECLARE_KIND(k_hxffi_dlib);
DEFINE_KIND(k_hxffi_dlib);

/////////////////////////  DYLD IMPLEMENTATION   ////////////////////////////

static value hxffi_dlopen(value str)
{
	val_check(str, string);
	
	void *lib = dlopen(val_string(str), RTLD_LAZY);
	if (NULL == lib)
	{
		return alloc_int(-1); //hxcpp bug with alloc_null
	}
	
	return alloc_abstract(k_hxffi_dlib, lib);
}

DEFINE_PRIM(hxffi_dlopen, 1);

static value hxffi_cur_dlopen()
{
	void *h;
#ifdef HX_WINDOWS
	h = (void*)GetModuleHandle(NULL);
#else
	h = dlopen(NULL,RTLD_LAZY);
#endif
	
	return alloc_abstract(k_hxffi_dlib, h);
}

DEFINE_PRIM(hxffi_cur_dlopen, 0);

static value hxffi_dlsym(value handle, value name)
{
	val_check_kind(handle, k_hxffi_dlib);
	val_check(name, string);
	
	void *h = val_data(handle);
	void *ret = dlsym(h, val_string(name));
	if (NULL == ret)
		return alloc_int(-1); //hxcpp bug with alloc_null
	
	return alloc_abstract(k_hxffi_pointer, ret);
}

DEFINE_PRIM(hxffi_dlsym, 2);

static value hxffi_dlclose(value handle)
{
	val_check_kind(handle, k_hxffi_dlib);
	
	void *h = val_data(handle);
	if (NULL != h)
		dlclose(h);
	
	return alloc_int(-1); //hxcpp bug with alloc_null
}

DEFINE_PRIM(hxffi_dlclose, 1);

///////////////////////// LIBFFI IMPLEMENTATION ////////////////////////////

DECLARE_KIND(k_hxffi_type_array);
DEFINE_KIND(k_hxffi_type_array);

DECLARE_KIND(k_hxffi_type);
DEFINE_KIND(k_hxffi_type);

DECLARE_KIND(k_hxffi_cif);
DEFINE_KIND(k_hxffi_cif);

static value hxffi_get_addressof(value dynamic)
{
	void *addr = &dynamic;
	return alloc_abstract(k_hxffi_pointer, addr);
}

DEFINE_PRIM(hxffi_get_addressof, 1);

static ffi_type *_hxffi_get_native_type(hxffi_types t)
{
	ffi_type *ret = NULL;
	
	switch(t)
	{
		case hxffi_type_void:
			ret = &ffi_type_void;
			break;
		case hxffi_type_uint8:
			ret = &ffi_type_uint8;
			break;
		case hxffi_type_sint8:
			ret = &ffi_type_sint8;
			break;
		case hxffi_type_uint16:
			ret = &ffi_type_uint16;
			break;
		case hxffi_type_sint16:
			ret = &ffi_type_sint16;
			break;
		case hxffi_type_uint32:
			ret = &ffi_type_uint32;
			break;
		case hxffi_type_sint32:
			ret = &ffi_type_sint32;
			break;
		case hxffi_type_uint64:
			ret = &ffi_type_uint64;
			break;
		case hxffi_type_sint64:
			ret = &ffi_type_sint64;
			break;
		case hxffi_type_float:
			ret = &ffi_type_float;
			break;
		case hxffi_type_double:
			ret = &ffi_type_double;
			break;
		case hxffi_type_uchar:
			ret = &ffi_type_uchar;
			break;
		case hxffi_type_schar:
			ret = &ffi_type_schar;
			break;
		case hxffi_type_ushort:
			ret = &ffi_type_ushort;
			break;
		case hxffi_type_sshort:
			ret = &ffi_type_sshort;
			break;
		case hxffi_type_uint:
			ret = &ffi_type_uint;
			break;
		case hxffi_type_sint:
			ret = &ffi_type_sint;
			break;
		case hxffi_type_ulong:
			ret = &ffi_type_ulong;
			break;
		case hxffi_type_slong:
			ret = &ffi_type_slong;
			break;
		case hxffi_type_pointer:
			ret = &ffi_type_pointer;
			break;
		default:
			neko_error();
	}
	
	ffi_type *retval = new ffi_type;
	memcpy(retval, ret, sizeof(ffi_type));
	
	return retval;
}

static value hxffi_get_native_type(value ntype)
{
	val_check(ntype, int);
	hxffi_types t = ((hxffi_types)(val_int(ntype) + 1));
	
	return alloc_abstract(k_hxffi_type, _hxffi_get_native_type(t));
}

DEFINE_PRIM(hxffi_get_native_type, 1);

static value hxffi_create_struct_type(value _hxffi_types_array)
{
	val_check(_hxffi_types_array, array);
	int size = val_array_size(_hxffi_types_array);
	
	ffi_type *tm_type = new ffi_type;
	ffi_type **tm_type_elements = new ffi_type *[size + 1];
	tm_type_elements[size] = NULL;
	
	tm_type->size = tm_type->alignment = 0;
	tm_type_elements = tm_type_elements;
	
	int i = 0;
	for (i = 0; i < size; i++)
	{
		value current = val_array_i(_hxffi_types_array, i);
		if (val_is_int(current))
			tm_type_elements[i] = _hxffi_get_native_type(((hxffi_types)(val_int(current) + 1)));
		else if (val_is_kind(current, k_hxffi_type))
			tm_type_elements[i] = ((ffi_type *) val_data(current));
		else
			neko_error(); //TODO cleanup if fail
	}
	
	return alloc_abstract(k_hxffi_type, tm_type);
}

DEFINE_PRIM(hxffi_create_struct_type, 1)

static value hxffi_create_type_descriptor_array(value num_args)
{
	val_check(num_args, int);
	int nargs = val_int(num_args);
	
	hxffi_type_array *ret = new hxffi_type_array;
	ret->size = nargs;
	ret->arr = new ffi_type *[nargs + 1];
	ret->native_types = new hxffi_types[nargs + 1];
	
	return alloc_abstract(k_hxffi_type_array, ret);
}

DEFINE_PRIM(hxffi_create_type_descriptor_array, 1);

static value hxffi_set_descriptor_value(value _type_arr, value _index_pos, value _type_val)
{
	val_check_kind(_type_arr, k_hxffi_type_array);
	val_check(_index_pos, int);
	val_check_kind(_type_val, k_hxffi_type);
	
	int index_pos = val_int(_index_pos);
	hxffi_type_array *type_arr = ((hxffi_type_array *) val_data(_type_arr));
	ffi_type *type_val = ((ffi_type *) val_data(_type_arr));
	
	//bounds check
	if (type_arr->size <= ((unsigned int) index_pos))
	{
		neko_error();
	}
	
	type_arr->arr[index_pos] = type_val;
	type_arr->native_types[index_pos] = hxffi_zero;
	
	return alloc_int(-1); //hxcpp bug with alloc_null
}

DEFINE_PRIM(hxffi_set_descriptor_value, 3);

static value hxffi_set_descriptor_native_value(value _type_arr, value _index_pos, value _ntype)
{
	val_check_kind(_type_arr, k_hxffi_type_array);
	val_check(_index_pos, int);
	val_check(_ntype, int);
	
	int index_pos = val_int(_index_pos);
	hxffi_type_array *type_arr = ((hxffi_type_array *) val_data(_type_arr));
	hxffi_types ntype = ((hxffi_types) (val_int(_ntype) + 1));
	
	//bounds check
	if (type_arr->size <= ((unsigned int) index_pos))
		neko_error();
	
	type_arr->native_types[index_pos] = ntype;
	switch(ntype)
	{
		case hxffi_type_void:
			type_arr->arr[index_pos] = &ffi_type_void;
			break;
		case hxffi_type_uint8:
			type_arr->arr[index_pos] = &ffi_type_uint8;
			break;
		case hxffi_type_sint8:
			type_arr->arr[index_pos] = &ffi_type_sint8;
			break;
		case hxffi_type_uint16:
			type_arr->arr[index_pos] = &ffi_type_uint16;
			break;
		case hxffi_type_sint16:
			type_arr->arr[index_pos] = &ffi_type_sint16;
			break;
		case hxffi_type_uint32:
			type_arr->arr[index_pos] = &ffi_type_uint32;
			break;
		case hxffi_type_sint32:
			type_arr->arr[index_pos] = &ffi_type_sint32;
			break;
		case hxffi_type_uint64:
			type_arr->arr[index_pos] = &ffi_type_uint64;
			break;
		case hxffi_type_sint64:
			type_arr->arr[index_pos] = &ffi_type_sint64;
			break;
		case hxffi_type_float:
			type_arr->arr[index_pos] = &ffi_type_float;
			break;
		case hxffi_type_double:
			type_arr->arr[index_pos] = &ffi_type_double;
			break;
		case hxffi_type_uchar:
			type_arr->arr[index_pos] = &ffi_type_uchar;
			break;
		case hxffi_type_schar:
			type_arr->arr[index_pos] = &ffi_type_schar;
			break;
		case hxffi_type_ushort:
			type_arr->arr[index_pos] = &ffi_type_ushort;
			break;
		case hxffi_type_sshort:
			type_arr->arr[index_pos] = &ffi_type_sshort;
			break;
		case hxffi_type_uint:
			type_arr->arr[index_pos] = &ffi_type_uint;
			break;
		case hxffi_type_sint:
			type_arr->arr[index_pos] = &ffi_type_sint;
			break;
		case hxffi_type_ulong:
			type_arr->arr[index_pos] = &ffi_type_ulong;
			break;
		case hxffi_type_slong:
			type_arr->arr[index_pos] = &ffi_type_slong;
			break;
		case hxffi_type_pointer:
			type_arr->arr[index_pos] = &ffi_type_pointer;
			break;
		default:
			neko_error();
	}
	
	return alloc_int(-1); //hxcpp bug with alloc_null
}

DEFINE_PRIM(hxffi_set_descriptor_native_value, 3);

static value hxffi_create_call_interface(value _abi, value _nargs, value _r_type, value _type_arr)
{
	ffi_abi abi = FFI_DEFAULT_ABI;
	if (!val_is_null(_abi))
	{
		val_check(_abi, string);
		const char *val = val_string(_abi);
		
#ifdef X86_WIN32
		if (strcmp(val, "sysv") == 0)
			abi = FFI_SYSV;
		else if (strcmp(val, "stdcall") == 0)
			abi = FFI_STDCALL;
		else val_throw(alloc_string("Unknown ABI"));
#elif defined(X86_WIN64)
		if (strcmp(val, "win64") == 0)
			abi = FFI_WIN64;
		else val_throw(alloc_string("Unknown ABI"));
#else
		if (strcmp(val, "sysv") == 0)
			abi = FFI_SYSV;
		else if (strcmp(val, "unix64") == 0)
			abi = FFI_UNIX64;
		else val_throw(alloc_string("Unknown ABI"));
#endif
	}
	
	val_check(_nargs, int);
	int nargs = val_int(_nargs);
	val_check_kind(_r_type, k_hxffi_type);
	ffi_type *r_type = ((ffi_type *) val_data(_r_type));
	val_check_kind(_type_arr, k_hxffi_type_array);
	hxffi_type_array *type_arr = ((hxffi_type_array *) val_data(_type_arr));
	
	ffi_cif *ret = new ffi_cif;
	ffi_status stat = ffi_prep_cif(ret, abi, nargs, r_type, type_arr->arr);
	switch(stat)
	{
		case FFI_BAD_ABI: val_throw(alloc_string("FFI_BAD_ABI"));
		case FFI_BAD_TYPEDEF: val_throw(alloc_string("FFI_BAD_TYPEDEF"));
	}
	
	hxffi_cif *_ret = new hxffi_cif;
	_ret->cif = ret;
	_ret->type_array = type_arr;
	
	return alloc_abstract(k_hxffi_cif, _ret);
}

DEFINE_PRIM(hxffi_create_call_interface, 4)

static char *get_buffer(value buf)
{
	if (val_is_string(buf))
	{
		return (char *) val_string(buf);
	} else {
		buffer b = val_to_buffer(buf);
		if (b == 0)
			val_throw(alloc_string("bad buffer"));
		
		return (char *) buffer_data(b);
	}
}

static value hxffi_call_cif(value *args, int _nargs)
{
	//ffi_call (ffi_cif *cif, void *fn, void *rvalue, void **avalues)
	int z = 0;
	unsigned int nargs = (unsigned int) _nargs;
	
	if (nargs < 3)
		val_throw(alloc_string("Too few arguments"));
	
	val_check_kind(args[0], k_hxffi_cif);
	hxffi_cif *hxcif = ((hxffi_cif *) val_data(args[0]));
	
	val_check_kind(args[1], k_hxffi_pointer);
	void *fun = ((void *) val_data(args[1]));
	
	void *retval = NULL;
	if (!val_is_null(args[2]))
	{
		retval = (void *)get_buffer(args[2]);
	}
	
	if (hxcif->type_array->size < (nargs - 3))
		nargs = hxcif->type_array->size + 3;
	
	void **values = new void *[nargs - 2];
	values[nargs - 1] = NULL;
	
	UINT64 *real_vals = new UINT64[nargs - 3]; // alloc an array big enough for all references
	
	int i = 3;
	for (i = 3; i < nargs; i++)
	{

		int ival = 0;
		double dval = 0.0;
		
		value myval = args[i];
		
		if (val_is_int(myval)) 
		{
			ival = val_int(myval);
			dval = (double) ival;
		} else if (val_is_float(myval))
		{
			dval = val_float(myval);
			ival = (int) dval;
		} else if (val_is_buffer(myval) || val_is_string(myval)) {
			char *val = get_buffer(myval);
			values[i - 3] = (void *) val;
			continue;
		} else if (val_is_kind(myval, k_hxffi_pointer)) {
			values[i - 3] = (void *) val_data(myval);
			continue;
		} else {
			val_throw(alloc_string("Unrecognized call type."));
		}
		
		values[i - 3] = real_vals;
		switch(hxcif->type_array->native_types[i - 3])
		{
			case hxffi_type_void:
				neko_error();
			case hxffi_type_uint8:
			{
				UINT8 *vals_ui8 = (UINT8 *) real_vals;
				vals_ui8[0] = (UINT8) ival;
			}
				break;
			case hxffi_type_sint8:
			{
				SINT8 *vals_si8 = (SINT8 *) real_vals;
				vals_si8[0] = (SINT8) ival;
			}
				break;
			case hxffi_type_uint16:
			{
				UINT16 *vals_ui16 = (UINT16 *) real_vals;
				vals_ui16[0] = (UINT16) ival;
			}
				break;
			case hxffi_type_sint16:
			{
				SINT16 *vals_si16 = (SINT16 *) real_vals;
				vals_si16[0] = (SINT16) ival;
			}
				break;
			case hxffi_type_uint32:
			{
				UINT32 *vals_ui32 = (UINT32 *) real_vals;
				vals_ui32[0] = (UINT32) ival;
			}
				break;
			case hxffi_type_sint32:
			{
				SINT32 *vals_si32 = (SINT32 *) real_vals;
				vals_si32[0] = (SINT32) ival;
			}
				break;			
			case hxffi_type_float:
			{
				float *vals_float = (float *) real_vals;
				vals_float[0] = (float) dval;
			}
				break;
			case hxffi_type_double:
			{
				double *vals_double = (double *) real_vals;
				vals_double[0] = (double) dval;
			}
				break;
			case hxffi_type_uchar:
			{
				unsigned char *vals_uchar = (unsigned char *) real_vals;
				vals_uchar[0] = (unsigned char) ival;
			}
				break;
			case hxffi_type_schar:
			{
				char *vals_schar = (char *) real_vals;
				vals_schar[0] = (char) ival;
			}
				break;
			case hxffi_type_ushort:
			{
				unsigned short *vals_ushort = (unsigned short *) real_vals;
				vals_ushort[0] = (unsigned short) ival;
			}
				break;
			case hxffi_type_sshort:
			{
				short *vals_sshort = (short *) real_vals;
				vals_sshort[0] = (short) ival;
			}
				break;
			case hxffi_type_uint:
			{
				unsigned int *vals_uint = (unsigned int *) real_vals;
				vals_uint[0] = (unsigned int) ival;
			}
				break;
			case hxffi_type_sint:
			{
				int *vals_sint = (int *) real_vals;
				vals_sint[0] = (int) ival;
			}
				break;
			case hxffi_type_ulong:
			{
				unsigned long *vals_ulong = (unsigned long *) real_vals;
				vals_ulong[0] = (unsigned long) ival;
			}
				break;
			case hxffi_type_slong:
			{
				long *vals_slong = (long *) real_vals;
				vals_slong[0] = (long) ival;
			}
				break;
			default:
				neko_error();
		}

		real_vals++;
	}
	
	printf("calling now...\n");
	ffi_call(hxcif->cif, FFI_FN(fun), retval, values);
	
	return alloc_int(-1); //hxcpp bug with alloc_null
}

DEFINE_PRIM_MULT(hxffi_call_cif);

HXFFI_EXPORT int checking(int a, short b, signed char c)
{
  return (a < 0 && b < 0 && c < 0);
}

static value hxffi_get_address_checking()
{
	void *addr = (void *) &checking;
	return alloc_abstract(k_hxffi_pointer, addr);
}

DEFINE_PRIM(hxffi_get_address_checking, 0);

value hxffi_test_ffi()
{
	ffi_cif cif;
	ffi_type *args[10];
	void *values[10];
	ffi_arg rint;

	signed int si;
	signed short ss;
	signed char sc;

	args[0] = &ffi_type_sint;
	values[0] = &si;
	args[1] = &ffi_type_sshort;
	values[1] = &ss;
	args[2] = &ffi_type_schar;
	values[2] = &sc;

	/* Initialize the cif */
	if(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 3,
	     &ffi_type_sint, args) != FFI_OK)
		neko_error();

	si = -6;
	ss = -12;
	sc = -1;

	checking (si, ss, sc);

	ffi_call(&cif, FFI_FN(checking), &rint, values);

	printf ("%d vs %d\n", (int)rint, checking (si, ss, sc));

	if (!rint) neko_error();
	
	return alloc_int(0);
}

DEFINE_PRIM(hxffi_test_ffi, 0);

// Reference this to bring in all the symbols for the static library
int hxffi_register_prims() { return 0; }

#ifdef __cplusplus
}
#endif
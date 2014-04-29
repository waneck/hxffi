package hxffi;

enum NativeType
{
	TVoid;
	//The type void. This cannot be used for argument types, only for return values. 
	TUInt8;
	//An unsigned, 8-bit integer type. 
	TInt8;
	//A signed, 8-bit integer type. 
	TUInt16;
	//An unsigned, 16-bit integer type. 
	TInt16;
	//A signed, 16-bit integer type. 
	TUInt32;
	//An unsigned, 32-bit integer type. 
	TInt32;
	//A signed, 32-bit integer type. 
	TUInt64;
	//An unsigned, 64-bit integer type. 
	TInt64;
	//A signed, 64-bit integer type. 
	TSingle;
	//The C float type. 
	TDouble;
	//The C double type. 
	TUChar;
	//The C unsigned char type. 
	TChar;
	//The C signed char type. (Note that there is not an exact equivalent to the C char type in libffi; ordinarily you should either use hxffi_type_schar or hxffi_type_uchar depending on whether char is signed.) 
	TUShort;
	//The C unsigned short type. 
	TShort;
	//The C short type. 
	TUInt;
	//The C unsigned int type. 
	TInt;
	//The C int type. 
	TULong;
	//The C unsigned long type. 
	TLong;
	//The C long type. 
	TPointer;
	//A generic void * pointer. You should use this for all pointers, regardless of their real type.
}
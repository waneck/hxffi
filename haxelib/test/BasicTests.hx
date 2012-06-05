package ;
import utest.Assert;
import hxffi.FunctionPointer;
import hxffi.HxffiTypeArray;
import hxffi.NativeType;
import hxffi.CallInterface;
import hxffi.dlib.DynamicLib;
import haxe.io.Bytes;
#if cpp
import cpp.Lib;
import cpp.Sys;
#else
import neko.Lib;
import neko.Sys;
#end


class BasicTests
{
	
	public function new():Void
	{
		
	}
	
	public function setup():Void
	{
		
	}
	
	public function test_InvalidLoad():Void
	{
		var lib = DynamicLib.getLib("INVALIDLOCATION");
		Assert.isNull(lib);
	}
	
	public function test_DynamicLoad():Void
	{
		var lib = DynamicLib.getLib(Sys.getCwd() + "hxffi.ndll");
		Assert.notNull(lib);
		var pointer = lib.getSymbol("checking");
		Assert.notNull(pointer);
		
		var cif = CallInterface.ofNativeTypes([TInt, TShort, TChar], TInt);
		var ret = Bytes.alloc(4); //just to be safe
		
		cif.call(pointer, ret.getData(), -6,-12,-1);
		
		var b = new haxe.io.BytesInput(ret);
		var ret = b.readInt31();
		Assert.notEquals(0, ret);
		Assert.equals(1, ret); //should be 1; but there can be endianness issues here that we won't take care right now (structs will take care)
	}
	
	public function test_CallOnC():Void
	{
		var lib = Lib.load("hxffi", "hxffi_test_ffi", 0);
		Assert.equals(0, lib()); //just doesn't fail it's good
	}
	
	public function test_CallInternal():Void
	{
		/*
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

		/* Initialize the cif *
		if(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 3,
		     &ffi_type_sint, args) != FFI_OK)
			neko_error();

		si = -6;
		ss = -12;
		sc = -1;

		checking (si, ss, sc);

		ffi_call(&cif, FFI_FN(checking), &rint, values);

		printf ("%d vs %d\n", (int)rint, checking (si, ss, sc));

		if (rint != 0)
			neko_error();
		*/
		
		var cif = CallInterface.ofNativeTypes([TInt, TShort, TChar], TInt);
		var pointer = FunctionPointer.withHandle(checking_addr());
		var ret = Bytes.alloc(4); //just to be safe
		
		//testing call dynamic
		cif.callDynamic(pointer, ret.getData(), [-6,-12,-1]);
		
		var b = new haxe.io.BytesInput(ret);
		var ret = b.readInt31();
		Assert.notEquals(0, ret);
		Assert.equals(1, ret); //should be 1; but there can be endianness issues here that we won't take care right now (structs will take care)
		
		trace(ret);
	}
	
	static var checking_addr = Lib.load("hxffi", "hxffi_get_address_checking", 0);
}
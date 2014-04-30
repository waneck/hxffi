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
#else
import neko.Lib;
#end


class BasicTests
{

	public var lib:DynamicLib;

	public function new():Void
	{
		lib = DynamicLib.getLib('bin/testlib.so');
		Assert.notNull(lib);
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
		var ret = b.readInt32();
		Assert.notEquals(0, ret);
		Assert.equals(1, ret); //should be 1; but there can be endianness issues here that we won't take care right now (structs will take care)
	}

	public function test_CallOnC():Void
	{
		var lib = Lib.load("hxffi", "hxffi_test_ffi", 0);
		Assert.equals(0, lib()); //just doesn't fail it's good
	}
}

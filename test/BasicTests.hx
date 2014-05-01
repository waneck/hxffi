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
		if (sys.FileSystem.exists('bin/testlib.so'))
			lib = DynamicLib.getLib('bin/testlib.so');
		else
			lib = DynamicLib.getLib(Sys.executablePath() + '/testlib.so');
	}

	public function setup():Void
	{
		Assert.notNull(lib);
	}

	public function test_InvalidLoad():Void
	{
		var lib = DynamicLib.getLib("INVALIDLOCATION");
		Assert.isNull(lib);
	}
}

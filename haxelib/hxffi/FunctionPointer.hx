package hxffi;
#if cpp
import cpp.Lib;
#else
import neko.Lib;
#end

class FunctionPointer
{
	private var _handle:Dynamic;
	
	private function new(hnd)
	{
		this._handle = hnd;
	}
	
	public static function withHandle(hnd:Dynamic):FunctionPointer
	{
		return new FunctionPointer(hnd);
	}
	
	public static function addressOf(dyn:Dynamic):FunctionPointer
	{
		return new FunctionPointer(_addressOf(dyn));
	}
	
	private static var _addressOf = Lib.load("hxffi", "hxffi_get_addressof", 1);
}
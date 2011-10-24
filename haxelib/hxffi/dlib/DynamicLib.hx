package hxffi.dlib;
#if cpp
import cpp.Lib;
#else
import neko.Lib;
#end

import hxffi.FunctionPointer;

class DynamicLib
{
	private var _handle:Dynamic;
	
	private function new(hnd)
	{
		this._handle = hnd;
	}
	
	public static function getLib(path:String):Null<DynamicLib>
	{
		var lib:Dynamic = _dlopen(path);
		if (lib != -1 && lib != null)
			return new DynamicLib(lib);
		
		return null;
	}
	
	public static function current():DynamicLib
	{
		return new DynamicLib(_cur_dlopen());
	}
	
	public function getSymbol(name:String):Null<FunctionPointer>
	{
		var sym:Dynamic = _dlsym(this._handle, name);
		if (sym != -1 && sym != null)
			return FunctionPointer.withHandle(sym);
		
		return null;
	}
	
	public function close():Void
	{
		_dlclose(this._handle);
	}
	
	private static var _dlopen:Dynamic = Lib.load("hxffi", "hxffi_dlopen", 1);
	private static var _dlsym:Dynamic = Lib.load("hxffi", "hxffi_dlsym", 2);
	private static var _cur_dlopen:Dynamic = Lib.load("hxffi", "hxffi_cur_dlopen", 0);
	private static var _dlclose:Dynamic = Lib.load("hxffi", "hxffi_dlclose", 1);
}
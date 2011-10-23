package hxffi;
#if cpp
import cpp.Lib;
#else
import neko.Lib;
#end

class CallInterface
{
	private var _handle:Dynamic;
	private var typeArray:HxffiTypeArray;
	private var retType:HxffiType;
	
	public function new(nargs:Int, typeArray:HxffiTypeArray, retType:HxffiType, ?abi:Null<String>):Void
	{
		this.typeArray = typeArray;
		this.retType = retType;
		this._handle = _create(abi, nargs, untyped retType._handle, untyped typeArray._handle);
	}
	
	public static function ofTypes(types:Array<HxffiType>, ret:HxffiType, ?abi:String):CallInterface
	{
		var arr = new HxffiTypeArray(types.length);
		for (i in 0...types.length)
			arr.setValue(i, types[i]);
		
		return new CallInterface(types.length, arr, ret, abi);
	}
	
	public static function ofNativeTypes(types:Array<NativeType>, ret:NativeType, ?abi:String):CallInterface
	{
		var arr = new HxffiTypeArray(types.length);
		for (i in 0...types.length)
			arr.setNativeValue(i, types[i]);
		
		return new CallInterface(types.length, arr, HxffiType.ofNative(ret), abi);
	}
	
	public inline function call(func:FunctionPointer, retval:Null<haxe.io.BytesData>, ?arg1, ?arg2, ?arg3, ?arg4, ?arg5, ?arg6, ?arg7):Void
	{
		_call(this._handle, untyped func._handle, retval, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
	}
	
	public inline function call0(func:FunctionPointer, retval:Null<haxe.io.BytesData>):Void
	{
		_call(this._handle, untyped func._handle, retval);
	}
	
	public inline function call1(func:FunctionPointer, retval:Null<haxe.io.BytesData>, ?arg1):Void
	{
		_call(this._handle, untyped func._handle, retval, arg1);
	}
	
	public inline function call2(func:FunctionPointer, retval:Null<haxe.io.BytesData>, ?arg1, ?arg2):Void
	{
		_call(this._handle, untyped func._handle, retval, arg1, arg2);
	}
	
	public inline function call3(func:FunctionPointer, retval:Null<haxe.io.BytesData>, ?arg1, ?arg2, ?arg3):Void
	{
		_call(this._handle, untyped func._handle, retval, arg1, arg2, arg3);
	}
	
	public inline function call4(func:FunctionPointer, retval:Null<haxe.io.BytesData>, ?arg1, ?arg2, ?arg3, ?arg4):Void
	{
		_call(this._handle, untyped func._handle, retval, arg1, arg2, arg3, arg4);
	}
	
	public inline function call5(func:FunctionPointer, retval:Null<haxe.io.BytesData>, ?arg1, ?arg2, ?arg3, ?arg4, ?arg5):Void
	{
		_call(this._handle, untyped func._handle, retval, arg1, arg2, arg3, arg4, arg5);
	}
	
	public inline function call6(func:FunctionPointer, retval:Null<haxe.io.BytesData>, ?arg1, ?arg2, ?arg3, ?arg4, ?arg5, ?arg6):Void
	{
		_call(this._handle, untyped func._handle, retval, arg1, arg2, arg3, arg4, arg5, arg6);
	}
	
	private static var _call:Dynamic = Lib.load("hxffi", "hxffi_call_cif", -1);
	private static var _create:Dynamic = Lib.load("hxffi", "hxffi_create_call_interface", 4);
}
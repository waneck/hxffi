package hxffi;

#if cpp
import cpp.Lib;
#else
import neko.Lib;
#end

class HxffiTypeArray
{
	private var _handle:Dynamic;
	
	public function new(size:Int):Void
	{
		this._handle = _create(size);
	}
	
	public inline function setValue(index:Int, type:HxffiType):Void
	{
		_set_value(this._handle, index, untyped type._handle);
	}
	
	public inline function setNativeValue(index:Int, type:NativeType):Void
	{
		_set_native(this._handle, index, Type.enumIndex(type));
	}
	
	private static var _set_native = Lib.load("hxffi", "hxffi_set_descriptor_native_value", 3);
	private static var _set_value = Lib.load("hxffi", "hxffi_set_descriptor_value", 3);
	private static var _create = Lib.load("hxffi", "hxffi_create_type_descriptor_array", 1);
}
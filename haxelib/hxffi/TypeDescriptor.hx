package hxffi;

#if cpp
import cpp.Lib;
#else
import neko.Lib;
#end

class TypeDescriptor
{
#if HXFFI_MAX_ARGS
	private static var MAX_ARGS = 12;
#end
	
	private var _handle:Dynamic;
	
	public function new(size:Int)
	{
#if HXFFI_MAX_ARGS
		if (size > MAX_ARGS) throw "Type descriptor too big.";
#end
		_handle = _create_type(size);
	}
	
	public inline function setTypeAt(idx:Int, type:HxffiType):Void
	{
		if (untyped type.native != null)
			_setnativeval(_handle, idx, Type.enumIndex(type.native));
		else
			_setval(_handle, idx, untyped type._handle);
	}
	
	public inline function setNativeTypeAt(idx:Int, nativeType:NativeType):Void
	{
		_setnativeval(_handle, idx, Type.enumIndex(nativeType));
	}
	
	private static var _create_type = Lib.load("hxffi", "hxffi_create_type_descriptor_array", 1);
	private static var _setval = Lib.load("hxffi", "hxffi_set_descriptor_value", 3);
	private static var _setnativeval = Lib.load("hxffi", "hxffi_set_descriptor_native_value", 3);
}
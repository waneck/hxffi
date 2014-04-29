package hxffi;

#if cpp
import cpp.Lib;
#else
import neko.Lib;
#end

class HxffiType
{
	private var _handle:Dynamic;
	private var native:Null<NativeType>;
	
	private function new(hnd:Dynamic)
	{
		this._handle = hnd;
	}
	
	
	public static function ofNative(native:NativeType):HxffiType
	{
		var ret = new HxffiType(_create_from_native(Type.enumIndex(native)));
		ret.native = native;
		
		return ret;
	}
	
	public static function fromDefinition(def:Array<HxffiType>):HxffiType
	{
		return new HxffiType(_create_from_definition(def));
	}
	
	public static function fromNative(def:Array<NativeType>):HxffiType
	{
		var d = [];
		for (def in def) d.push(Type.enumIndex(def));
		return new HxffiType(_create_from_definition(d));
	}
	
	private static var _create_from_native:Dynamic = Lib.load("hxffi", "hxffi_get_native_type", 1);
	private static var _create_from_definition:Dynamic = Lib.load("hxffi", "hxffi_create_struct_type", 1);
}
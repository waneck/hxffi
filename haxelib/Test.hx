class Test
{
	private static function main():Void
	{
		var lib = neko.Lib.load("hxffi", "test_ffi", 0);
		trace(lib());
	}
}
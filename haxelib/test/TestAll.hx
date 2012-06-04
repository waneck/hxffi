import utest.ui.Report;
class TestAll
{
	public static function main()
	{
		#if neko loadNekoAPI(); #end
		
		runTests();
#if cpp
		//test finalizers
		cpp.vm.Gc.run(true);
		cpp.vm.Gc.run(true);
#end
	}
	
	public static function runTests()
	{
		var runner = new utest.Runner();

		runner.addCase(new BasicTests());
		
		var report = Report.create(runner);
		runner.run();
	}
	
#if neko
     public static function loadNekoAPI()
     {
        var init = neko.Lib.load("hxffi", "neko_init", 5);
        if (init != null)
        {
           init(function(s) return new String(s),
                function(len:Int) { var r = []; if (len > 0) r[len - 1] = null; return r; },
                null, true, false);
        }
        else
           throw("Could not find NekoAPI interface.");
     }
#end
}
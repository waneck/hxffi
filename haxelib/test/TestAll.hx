import utest.ui.Report;
class TestAll
{
	public static function main()
	{
		var runner = new utest.Runner();

		runner.addCase(new BasicTests());
		
		var report = Report.create(runner);
		runner.run();
	}
}
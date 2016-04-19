package tests;
import junit.framework.TestCase;
import AST.*;
import utility.AnalysisType;

public class ScalaAETest extends TestCase {

	public void testScalaAETest()
	{
		ScalaCompiler compiler = new ScalaCompiler("input/t8.txt");
		Program p = compiler.run();
		runPreErrorCheck(p);
	}
	
	public void runPreErrorCheck(Program p) {
		p.printAnalysisResult(AnalysisType.AE);
	}
}

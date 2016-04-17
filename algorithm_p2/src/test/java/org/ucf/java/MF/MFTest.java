package org.ucf.java.MF;


import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class MFTest extends TestCase{
	  /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public MFTest( String testName )
    {
        super( testName );
    }
    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( MFTest.class );
    }
    public void testMaxFlow() {
    	FordFulkerson test=new FordFulkerson();
		test.getMaxStream("s", "t");
		test.printStreamInfo();
    }
}

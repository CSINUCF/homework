package org.ucf.java.ED;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Unit test for simple App.
 */
public class EDTest extends TestCase
{
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public EDTest( String testName )
    {
        super( testName );
    }
    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( EDTest.class );
    }
    
    public void testDistanceAssert() {
    	DynamicED d = new DynamicED(1,1);
        assertEquals(d.distance("dog","cat"),3);
        assertEquals(d.distance("this","that"),2);
        assertEquals(d.distance("somebody","something else"),8);
        assertEquals(d.distance("test","test"),0);
        assertEquals(d.distance("this is a long string that should take a while","this is another long string"),27);
    }
    public void testDistancePrint(){
    	DynamicED d = new DynamicED(1,2);
    	String a = "AGGCTATCACCTGACCTCCAGGCCGATGCCC";
    	String b = "TAGCTATCACGACCGCGGTCGATTTGCCCGAC";
    	int ret = d.distance(a,b);
    	System.out.println("*******************************************");
    	System.out.println("String a: "+a);
    	System.out.println("String b: "+b);
    	System.out.print("Un Match Penalty is: "+d.SUB_COST);
    	System.out.println("\tGap Penalty is: "+d.GAP_COST);
    	System.out.println("Final penalty cost of the alignment: "+ ret);
    	System.out.println("*******************************************");
    }
}

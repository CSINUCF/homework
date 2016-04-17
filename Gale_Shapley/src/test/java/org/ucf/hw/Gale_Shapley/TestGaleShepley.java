package org.ucf.hw.Gale_Shapley;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Unit test for simple App.
 */
public class TestGaleShepley 
    extends TestCase
{
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public TestGaleShepley( String testName )
    {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( TestGaleShepley.class );
    }

    /**
     * Rigourous Test :-)
     */
    public void testApp()
    {
        assertTrue( true );
    }
    public void testGaleShepley(){
    	
    	System.out.println("Gale Shapley Marriage Algorithm\n");
    	/** list of men **/
    	String[] men = {"M1", "M2", "M3", "M4", "M5"};
    	/** list of women **/
    	String[] women = {"W1", "W2", "W3", "W4", "W5"};
    	
    	/** men preference **/
    	String[][] menpref = {{"W5", "W2", "W3", "W4", "W1"}, 
                		 	  {"W3", "W5", "W1", "W2", "W4"}, 
                		 	  {"W4", "W3", "W2", "W1", "W5"}, 
                		 	  {"W1", "W2", "W3", "W4", "W5"},
                		 	  {"W1", "W2", "W5", "W4", "W3"}};
    	/** women preference **/                      
    	String[][] womenpref = {{"M5", "M3", "M4", "M1", "M2"}, 
                		 		{"M1", "M2", "M3", "M5", "M4"}, 
                		 		{"M4", "M5", "M3", "M2", "M1"},
                		 		{"M5", "M2", "M1", "M4", "M3"}, 
                		 		{"M2", "M1", "M4", "M3", "M5"}};
    	
    	GaleShepley gs = new GaleShepley(men,women,menpref,womenpref);
    	gs.stable_match();
    	gs.printCouples();
    }
}
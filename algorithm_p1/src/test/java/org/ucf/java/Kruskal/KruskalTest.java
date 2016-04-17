package org.ucf.java.Kruskal;

import org.ucf.java.common.GraphBuilder;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Unit test for simple App.
 */
public class KruskalTest 
    extends TestCase
{
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public KruskalTest( String testName )
    {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( KruskalTest.class );
    }

    /**
     * Rigourous Test :-)
     */
    public void testApp()
    {
        assertTrue( true );
    }
    public void testKruskal(){
    	System.out.println("######################################################");
    	String input = "input/adjacency-list.txt";
    	System.out.println("The input file:"+input);
        GraphBuilder builder = new GraphBuilder(input);

        /*Kruskal Algorithm test*/
        Kruskal test = new Kruskal(builder.Vertix_Set,builder.edgeList);
        test.KruskalMst();
        test.printgraph();
        System.out.println("######################################################n\n\n");
    
    	
    }
}

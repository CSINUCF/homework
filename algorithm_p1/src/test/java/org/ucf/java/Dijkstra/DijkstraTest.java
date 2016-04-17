package org.ucf.java.Dijkstra;

import org.ucf.java.Dijkstra.Dijkstra;
import org.ucf.java.common.GraphBuilder;


import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Unit test for simple App.
 */
public class DijkstraTest extends TestCase
{
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public DijkstraTest( String testName )
    {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( DijkstraTest.class );
    }

    /**
     * Rigourous Test :-)
     */
    public void testApp()
    {
        assertTrue( true );
    }
    public void testDijkstraTest(){
    	System.out.println("\n\n\n######################################################");
    	String input = "input/adjacency-list.txt";
    	System.out.println("The input file:"+input);
    	GraphBuilder builder = new GraphBuilder(input);
    	/**/
        Dijkstra mst = new Dijkstra(builder.Vertix_Set,builder.Vertix.get(1));
        mst.computePath();
        for(int i = 1;i<= 8;i++){
            mst.printpath(builder.Vertix.get(i));
        }
        System.out.println("######################################################\n\n\n");	
    }
}

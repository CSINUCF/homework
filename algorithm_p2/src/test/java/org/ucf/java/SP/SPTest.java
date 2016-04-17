package org.ucf.java.SP;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class SPTest extends TestCase{
	  /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public SPTest( String testName )
    {
        super( testName );
    }
    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( SPTest.class );
    }
    public void testMaxFlow() {
        BufferedReader bufReader = null; 
        File file = new File("input/1.txt");
        try { 
            bufReader = new BufferedReader(new FileReader(file));
        } catch (Exception e) { 
            e.printStackTrace(); 
            return; 
        }
        DynamicSP DyMF = new DynamicSP();
        try {
            System.out.println("************** Start Graph " 
                    + "******************************");
            DyMF.buildGraph(bufReader);
            DyMF.bellmanFordSPExec();
            DyMF.printSPFromSource(); 

            System.out.println("************** End Graph "
                    + "******************************");
        } catch (Exception e) { 
            e.printStackTrace(); 
            System.err.println("Exiting : " + e); 
        } finally { 
            try { 
                bufReader.close(); 
            } catch (Exception f) { 
            	f.printStackTrace();
            } 
        }
    }
}

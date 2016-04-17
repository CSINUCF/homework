package org.ucf.java.RNA;

import java.util.*;
import org.ucf.java.RNA.DynamicRNA;



import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Unit test for simple RNATest.
 */
public class RNATest extends TestCase
{
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public RNATest( String testName )
    {
        super( testName );
    }
    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( RNATest.class );
    }

    public void testDynamicRNAPrint(){
    	 String input = "AUGGCUACCGGUCGAUUGAGCGCCAAUGUAAUCAUU";
    	 DynamicRNA r = new DynamicRNA();
    	 TreeMap<Integer,Integer> ret =  r.secondaryStructure(input);
    	 System.out.println("***********************************");
    	 System.out.println("The Number of base pairs is: "+ret.size());
    	 System.out.println("The base pairs info:");
    	 for(Map.Entry<Integer,Integer> entry : ret.entrySet()) {
    		  System.out.println(input.charAt(entry.getKey())+"("+entry.getKey() +")"+ " => " 
    				  		   + input.charAt(entry.getValue())+"("+entry.getValue()+")");
    		}
    	 System.out.println("***********************************");
    }
    public void testDynamicRNAAssert() {
        DynamicRNA r = new DynamicRNA();
        
        TreeMap<Integer,Integer> expected = new TreeMap<Integer,Integer>();
        expected.put(0,16);
        expected.put(1,15);
        expected.put(2,11);
        expected.put(3,9);
        assertEquals(expected,r.secondaryStructure("ACCAGUCUGUCGUACGUCA"));
        
        expected = new TreeMap<Integer,Integer>();
        expected.put(0,14);
        expected.put(1,13);
        expected.put(4,12);
        expected.put(5,11);
        assertEquals(expected,r.secondaryStructure("ACCGUGUCUUGCAGUCCCA"));
        
        expected = new TreeMap<Integer,Integer>();
        expected.put(0,9);
        expected.put(1,7);
        assertEquals(expected,r.secondaryStructure("ACCCCGGGGU"));
        
        expected = new TreeMap<Integer,Integer>();
        expected.put(0,19);
        expected.put(2,17);
        expected.put(3,16);
        expected.put(4,14);
        expected.put(5,13);       
        assertEquals(expected,r.secondaryStructure("ACGCGACGUCGGCUCAGCCUUGGAAA"));
        
        expected = new TreeMap<Integer,Integer>();
        expected.put(0,14);
        expected.put(1,13);
        expected.put(3,12);
        expected.put(5,11);
        assertEquals(expected,r.secondaryStructure("ACGUCGUCCGUCAGUCUGA"));
        
        expected = new TreeMap<Integer,Integer>();
        expected.put(0,13);
        expected.put(1,11);
        expected.put(2,10);
        expected.put(3,9);
        assertEquals(expected,r.secondaryStructure("AAAAAAUUUUUUAU"));
    }
}

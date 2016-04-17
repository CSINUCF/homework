package org.ucf.java.HeapSort;

import org.ucf.java.common.GraphBuilder;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Unit test for simple App.
 */
public class HeapSortTest 
    extends TestCase
{
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public HeapSortTest( String testName )
    {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( HeapSortTest.class );
    }

    /**
     * Rigourous Test :-)
     */
    public void testApp()
    {
        assertTrue( true );
    }
    public void testHeapSortTest(){
    	
    	System.out.println("######################################################");
    	String input = "input/adjacency-list.txt";
    	System.out.println("The input file:"+input);
        GraphBuilder builder = new GraphBuilder(input);
        int [] array = new int[builder.edgeList.size()];
        
        for(int i = 0;i<builder.edgeList.size();i++) {
            array[i] = builder.edgeList.get(i).getWeight();
        }
        
        HeapSort heap = new HeapSort(array);
        System.out.println("Before the heapify, the tree is(not a heap tree):");
        heap.printHeapTree();
        /*sift up nodes to make a min heap tree*/
        heap.BuildMinHeap();
        
        System.out.println("After the heapify, the min heap tree:");
        heap.printHeapTree();

        
        /*heapsort the array and get the results*/
        heap.HeapSortTree();
        
        /*print the heapsort resutls*/
        System.out.println("After HeapSort, print the final results(ascending order):");
        heap.printHeap();
        System.out.println("######################################################\n\n\n");

    }
}

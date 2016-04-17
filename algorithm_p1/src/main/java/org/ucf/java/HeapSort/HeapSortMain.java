package org.ucf.java.HeapSort;
import org.ucf.java.common.*;

public class HeapSortMain {
    public static void main(String[] args) {
    	
    	System.out.println("\n\n\n######################################################");
    	if(args[0] == null){
    		System.out.println("There is no input file,please confirm that.");
    		return;
    	}
    	System.out.println("The input file:"+args[0]);
        GraphBuilder builder = new GraphBuilder(args[0]);
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

package org.ucf.java.Kruskal;
import org.ucf.java.common.*;


public class KruskalMain {
    public static void main(String[] args) {
    	System.out.println("n\n\n######################################################");
    	if(args[0] == null){
    		System.out.println("There is no input file,please confirm that.");
    		return;
    	}
    	System.out.println("The input file:"+args[0]);
        GraphBuilder builder = new GraphBuilder(args[0]);

        /*Kruskal Algorithm test*/
        Kruskal test = new Kruskal(builder.Vertix_Set,builder.edgeList);
        test.KruskalMst();
        test.printgraph();
        System.out.println("######################################################n\n\n");
    }
}

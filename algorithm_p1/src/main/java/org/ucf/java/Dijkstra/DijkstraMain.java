package org.ucf.java.Dijkstra;
import org.ucf.java.common.*;

public class DijkstraMain {
    public static void main(String[] args) {
    	System.out.println("\n\n\n######################################################");
    	if(args[0] == null){
    		System.out.println("There is no input file,please confirm that.");
    		return;
    	}
    	System.out.println("The input file:"+args[0]);
    	GraphBuilder builder = new GraphBuilder(args[0]);
    	/**/
        Dijkstra mst = new Dijkstra(builder.Vertix_Set,builder.Vertix.get(1));
        mst.computePath();
        for(int i = 1;i<= 8;i++){
            mst.printpath(builder.Vertix.get(i));
        }
        System.out.println("######################################################\n\n\n");
    }
}

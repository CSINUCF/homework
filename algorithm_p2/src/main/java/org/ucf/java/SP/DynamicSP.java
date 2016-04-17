package org.ucf.java.SP;

import java.io.BufferedReader;
import java.util.ArrayList; 
import java.util.Arrays; 

/**
 * 
 * @author Bing
 * 
 */
public class DynamicSP {

    int source = 0; 
    Graph graph; 
    int[] distInPath; 
    ArrayList<Edge> predecessorEdgeArr; 
    /**
     * Parse the input file and build a graph
     * @param bufReader
     * @throws Exception
     */
    void buildGraph(BufferedReader bufReader) throws Exception {
        try {
        	int nodecnt = Integer.parseInt(bufReader.readLine());
        	int edgecnt = Integer.parseInt(bufReader.readLine());
            
        	graph = new Graph(nodecnt,edgecnt);
            for (int k = 0; k < graph.getEdgesCnt(); k++) {
                String[] strArr = bufReader.readLine().split(" "); 
                int from = Integer.parseInt(strArr[0])-1;
                int to = Integer.parseInt(strArr[1])-1;
                int distance = Integer.parseInt(strArr[2]);
                Edge e = new Edge(from,to,distance);
                graph.addEdge(e); 
            }
            /*Parse the file to extract the source node*/
            source = (new Integer(bufReader.readLine())) - 1;
            System.out.println("Original information about edges:"); 
            printGraphEdges(graph); 
            System.out.println("---------------------------------"); 
        } catch (Exception e) { 
            e.printStackTrace(); 
            throw e; 
        } 
    }
    /**
     * Bellman Ford Algorithm
     * @return
     */
    boolean bellmanFordSPExec() { 
        boolean distanceChanged = false;
        /***
         * Initial weight array and predecessorEdgeinfo
         */
        distInPath = initWeightArray(graph.getNodesCnt(), source); 
        predecessorEdgeArr = new ArrayList<Edge>();
        for (int i = 0; i < graph.getNodesCnt(); i++) {
            predecessorEdgeArr.add(null); 
        }
        
        for (int k = 0; k < graph.getNodesCnt(); k++) {
            distanceChanged = false; 
            for (Edge e : graph) { 

                if (distInPath[e.getFrom()] == Integer.MAX_VALUE) {
                    continue; 
                } 
                if (e.getDistance() == Integer.MAX_VALUE) { 
                    continue; 
                } 
                if ((distInPath[e.getFrom()] + e.getDistance()) < distInPath[e
                        .getTo()]) { 
                    distanceChanged = true; 
                    distInPath[e.getTo()] = e.getDistance() + distInPath[e.getFrom()]; 
                    predecessorEdgeArr.set(e.getTo(),e); 
                } 
            }
            if (!distanceChanged) { 
                break; 
            } 
        } 
        if (!distanceChanged) { 
            return true; 
        } 
        boolean foundCycle = false;
        for (Edge e : graph) { 

            if (distInPath[e.getFrom()] == Integer.MAX_VALUE) { 
                continue; 
            } 
            if ((e.getDistance() + distInPath[e.getFrom()]) < distInPath[e.getTo()]) {
                foundCycle = true; 
                break; 
            } 
        } 

        if (foundCycle) { 
            System.out.print("Found Negative Cycle "); 
            return false;
        } 
        return true; 
    } 

    int[] initWeightArray(int totalNodes, int src) {
        int[] wtArr = new int[totalNodes]; 
        Arrays.fill(wtArr, Integer.MAX_VALUE); 
        wtArr[src] = 0; 
        return wtArr; 
    } 
    /**
     * Print shortest path info for all nodes from the source node 
     */
    void printSPFromSource() { 

        StringBuilder strBuild = new StringBuilder(); 
        System.out.println("All Shortest Paths from source " + (source+1) + " are"); 

        for (int i = 0; i < graph.getNodesCnt(); i++) {
            if (i == source) { 
                continue; 
            } 
            Edge edge = predecessorEdgeArr.get(i); 
            System.out.print("From " + (source+1) + " to " + (i+1) + ",\tdistance = " 
                    + distInPath[i] + ",\tPath Is : "); 
            strBuild.append("" + (i+1) + " >-- " + (edge.getFrom()+1) + " >-- "); 
            while ((edge = predecessorEdgeArr.get(edge.getFrom())) != null) {
                strBuild.append("" + (edge.getFrom()+1) + " >-- "); 
            } 

            System.out.println(strBuild.reverse().substring(4));
            strBuild.setLength(0); 
        } 
    }
    /**
     * Print the edge info of the given graph
     * @param graph
     */
    void printGraphEdges(Graph graph) { 
        for (Edge e : graph) { 
            System.out.println(e.toString()); 
        } 
    }
}
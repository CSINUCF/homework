package org.ucf.java.SP;
/**
 * 
 * @author Bing
 * 
 */
public class Edge {

    private int from; //the source node
    private int to; //the destination node
    private int distance; // the distance of edge
    public Edge(int from, int to, int distance) {
        super(); 
        this.from = from; 
        this.to = to; 
        this.distance = distance; 
    }
    /*get the source node index*/
    public int getFrom() { 
        return from; 
    }
    /*get the destination node index*/
    public int getTo() { 
        return to; 
    } 
    /*get the distance of the current edge*/
    public int getDistance() {
        return distance; 
    }
    public String toString() { 
        return "" + (from+1) + " -> " + (to+1) + " = " + distance;
    }
} 

package org.ucf.java.SP;

import java.util.ArrayList; 
import java.util.Iterator; 

public class Graph implements Iterable<Edge> {

    private int nodesCnt;/*The number of nodes*/
    private int edgesCnt;/*The number of edges*/
    private ArrayList<ArrayList<Edge>> edges = new ArrayList<ArrayList<Edge>>(); 

    public Graph(int nodesCnt, int edgesCnt) {
        super(); 
        this.nodesCnt = nodesCnt; 
        this.edgesCnt = edgesCnt;
        /*initial the global array list to record the edge info*/
        for (int i = 0; i < nodesCnt; i++) {
            edges.add(new ArrayList<Edge>()); 
        } 
    }
    public int getNodesCnt() {
        return nodesCnt; 
    }
    public int getEdgesCnt() {
        return edgesCnt; 
    }
    void addEdge(Edge e) { 
        edges.get(e.getFrom()).add(e); 
    }
    public ArrayList<ArrayList<Edge>> getEdges() {
        return edges; 
    } 
    /*override the iteration function to  implement the iterator of graph*/
    class EdgeIterator implements Iterator<Edge> {

        Edge next = null; 
        int currBucket = 0;

        EdgeIterator() { 
            next = getNextEdgeWithDist(); 
        } 
        public boolean hasNext() {
            if (next != null) {
                return true;
            } 
            return false;
        } 

        public Edge next() { 
            Edge prevNext = next; 
            next = getNextEdgeWithDist(); 
            return prevNext; 
        } 
        public void remove() {
            // do nothing 
        }
        private Edge getNextEdgeWithDist() {
            ArrayList<Edge> edgeBuck = edges.get(currBucket);
            int idx = -1;
            if (next != null) {
                idx = edgeBuck.indexOf(next); 
                if (idx == edgeBuck.size()) { 
                    currBucket++; 
                    edgeBuck = edges.get(currBucket); 
                    idx = -1; 
                } 
            } 
            if (currBucket >= nodesCnt) { 
                return null;
            } 
            while (true) { 
                Edge e; 
                for (int i = idx + 1; i < edgeBuck.size(); i++) {
                    e = edgeBuck.get(i); 
                    if (e.getDistance() > 0) {
                        return e; 
                    } 
                } 
                currBucket++; 
                idx = -1; 
                if (currBucket == nodesCnt) { 
                    return null;
                } 
                edgeBuck = edges.get(currBucket); 
            } 
        } 
    }
    public Iterator<Edge> iterator() { 
        return new EdgeIterator();
    } 
}

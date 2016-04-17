package org.ucf.java.Dijkstra;
import org.ucf.java.common.*;
import java.util.*;

/**
 * Created by Bing on 3/17/2016.
 */

public class Dijkstra {
    public Set<Node> open;     // to store nodes which does not deal with
    public Set<Node> close;    // to store sort results
    public Set<Edge> path;     // to store the edge information
    public Node start;

    public Dijkstra(Set<Node> vertex,Node root) {
        this.open = new HashSet<Node>(vertex);
        this.close = new HashSet<Node>();
        this.path = new HashSet<Edge>();
        this.start = root;
    }
    public void computePath() {
        start.setDistance(0);
        close.add(start);
        open.remove(start);
        System.out.println("start node is:"+start.getName());
        while (!open.isEmpty()) {
            Iterator<Node> it = close.iterator();
            Edge edge = new Edge(null, null, Integer.MAX_VALUE);
            while (it.hasNext()) {
                Node node = it.next();
                node.getShortestPath(open,edge);
            }
            path.add(edge);
            close.add(edge.getDst());
            edge.getDst().setDistance(edge.getWeight());
            edge.getDst().getChild().remove(edge.getSrc());

            edge.getSrc().getChild().remove(edge.getDst());
            open.remove(edge.getDst());
        }
    }
    public void findpath(Node e,List<Edge> link) {
        Iterator<Edge> it = path.iterator();
        while(it.hasNext()) {
            Edge edg = it.next();
            if(edg.getDst().equals(e) == true){
                link.add(edg);
                findpath(edg.getSrc(),link);
            }
        }
        if(e.equals(start) == true) {
            if(link.isEmpty() == true)
                link.add(new Edge(e,e,e.getDistance()));
            return;
        }
    }
    public void printedges(){
        Iterator<Edge> it = path.iterator();
        while (it.hasNext()){
            Edge link = it.next();
            link.printinfo();
        }
    }
    public void printpath(Node node){
        List<Edge> path = new ArrayList<Edge>();
        this.findpath(node,path);
       for(int i=path.size()-1;i>=0;i--) {
           Edge it = path.get(i);
           if(i == path.size() -1){
        	   System.out.print("The Destination Node: "+path.get(0).getDst().getName()+"\t");
        	   System.out.print("\tPath value ="+path.get(0).getWeight()+"\t Path is: ");
               System.out.print(it.getSrc().getName()+"->"+it.getDst().getName());
            } else{
               System.out.print("->"+it.getDst().getName());
           }
           if(i == 0)
        	   System.out.println();
       }
    }
}

package org.ucf.java.common;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

/**
 * Created by Bing on 3/17/2016.
 */
public class Node {
    private String name;
    private int index;
    private int distance;
    private Map<Node,Integer> child = new HashMap<Node,Integer>();
    public Node(String name,Integer idx) {
        this.name=name;
        this.index = idx;
        this.distance = Integer.MAX_VALUE;
    }
    public Integer getIndex(){
        return this.index;
    }
    public String getName() {
        return this.name;
    }
    public void addChild(Node children, Integer key) {
        this.child.put(children,key);
    }
    public Map<Node,Integer> getChild() {
        return this.child;
    }
    public void printEdgeinfo() {
        Iterator<Map.Entry<Node,Integer>> it = this.child.entrySet().iterator();
        while (it.hasNext())
        {
            Map.Entry<Node,Integer> pair = (Map.Entry<Node,Integer>)it.next();
            System.out.println(this.name+"->"+pair.getKey().getName()+":"+pair.getValue());
        }
    }
    public void setDistance (Integer dv){
        this.distance = dv;
    }
    public Integer getDistance(){
        return this.distance;
    }
    public void getShortestPath(Set<Node> open,Edge e){
        Map<Node,Integer> childs=this.getChild();
        for(Node child:childs.keySet()){
            if(open.contains(child))
            {
                int distance=childs.get(child)+ this.getDistance();
                if(distance<e.getWeight())
                {
                    e.setSrc(this);
                    e.setDst(child);
                    e.setWeight(distance);
                }
            }
        }
    }
}

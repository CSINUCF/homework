package org.ucf.java.common;

/**
 * Created by Bing on 3/17/2016.
 */
public class Edge {
    private Node src;
    private Node dst;
    private Integer weight;

    public Edge(Node s,Node d,Integer w)
    {
        this.src = s;
        this.dst = d;
        this.weight = w;
    }
    public void setSrc(Node s){
        this.src = s;
    }
    public void setDst(Node d){
        this.dst = d;
    }
    public void setWeight(Integer w){
        this.weight = w;
    }
    public Node getSrc(){
        return  this.src;
    }
    public Node getDst(){
        return  this.dst;
    }
    public Integer getWeight(){
        return  this.weight;
    }
    public void printinfo() {
        String src_name;
        String dst_name;
        if(this.getSrc() == null)
            src_name = "null";
        else
            src_name = this.getSrc().getName();
        if(this.getDst() == null)
            dst_name = "null";
        else
            dst_name = this.getDst().getName();
        System.out.println(src_name+"->"+dst_name+": "+this.getWeight());
    }
}

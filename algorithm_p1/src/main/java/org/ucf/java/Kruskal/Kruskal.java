package org.ucf.java.Kruskal;

import java.util.*;
import org.ucf.java.common.*;
import org.ucf.java.HeapSort.*;

/**
 * Created by Bing on 3/18/2016.
 */
public class Kruskal {

    private Set<Node> vertex;
    private List<Edge> edges;
    private List<Edge> edges_mst;
    public Kruskal(Set<Node> v,List<Edge> e) {
        this.vertex = v;
        this.edges = e;
        this.edges_mst = new ArrayList<Edge>();
    }
    private int[] HeapsortEdgesWithArray() {
        int [] array = new int[edges.size()];
        for(int i = 0;i<edges.size();i++) {
            array[i] = edges.get(i).getWeight();
        }
        HeapSort heap = new HeapSort(array);
        heap.BuildMinHeap();
        heap.HeapSortTree();
        int [] heap_result = heap.getHeap();
        return  heap_result;
    }
    private Edge getEdgewWithWeight(int key) {
        Edge res;
        for(int i=0;i<edges.size();i++){
            res = edges.get(i);
            if(key == res.getWeight()){
                edges.remove(i);
                return res;
            }
        }
        return null;
    }

    private ArrayList<Node> getsubroot(ArrayList<ArrayList<Node>> root, Node e){

        ArrayList<Node> tmp;
        for(int i=0;i<root.size();i++)
        {
            tmp = root.get(i);
            if(tmp.contains(e) == true)
                return tmp;
        }
        return null;
    }

    private ArrayList<ArrayList<Node>> getrootnode(){
        ArrayList<ArrayList<Node>> res = new ArrayList<ArrayList<Node>>();
        Iterator<Node> it = vertex.iterator();
        while (it.hasNext()) {
            ArrayList<Node> root = new ArrayList<Node>();
            root.add(it.next());
            res.add(root);
        }
        return res;
    }
    public void KruskalMst() {
        int [] heap_res = HeapsortEdgesWithArray();
        ArrayList<ArrayList<Node>> root = getrootnode();
        Edge e;
        Node src,dst;
        ArrayList<Node> src_root,dst_root;
        for(int i=0;i<heap_res.length;i++)
        {
            e = getEdgewWithWeight(heap_res[i]);
            src = e.getSrc();
            dst = e.getDst();
            src_root = getsubroot(root,src);
            dst_root = getsubroot(root,dst);
            if(src_root.contains(dst) == false)
            {
                src_root.addAll(dst_root);
                dst_root.clear();
                edges_mst.add(e);
            }
        }
    }
    public void printgraph(){
    	int t_wgt = 0;
    	System.out.print("Minimum Spanning Tree:\nThe node set is :");
    	Iterator<Node> it = vertex.iterator();
    	while(it.hasNext())
    	{
    		Node e = it.next();
    		System.out.print(e.getName()+" ");
    		
    	}
    	System.out.println("\nThe Edg Set is :");
        for(int i = 0;i<edges_mst.size();i++)
        {
            edges_mst.get(i).printinfo();
            t_wgt += edges_mst.get(i).getWeight();
        }
        System.out.println("The Total Weight on MST edges is:"+t_wgt);
    }
}
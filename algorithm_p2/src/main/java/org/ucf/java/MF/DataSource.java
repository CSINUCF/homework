package org.ucf.java.MF;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@SuppressWarnings("resource")
public class DataSource {
	private static Map<String,Edge> edges=new HashMap<String,Edge>();
	static{
	     try {
	    	 File file = new File("input/2.txt");
		     BufferedReader buffReader = new BufferedReader(new FileReader(file));
		     String lineTxt = null;
             while((lineTxt = buffReader.readLine()) != null){
	                String[] strArr = lineTxt.split(" "); 
	                String from = strArr[0];
	                String to = strArr[1];
	                int distance = Integer.parseInt(strArr[2]);
	                Edge e1 = new Edge(from,to,0,distance,distance);
	                Edge e2 = new Edge(from,to,0,0,0);
	                edges.put(from+"->"+to, e1);
	                edges.put(to+"->"+from, e2);
			 }
		} catch (NumberFormatException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}   
	}
	public static List<Edge> nextEdge(String start){
		List<Edge> res=new ArrayList<Edge>();
		for(String edge:edges.keySet()){
			if(edge.startsWith(start)){
				res.add(edges.get(edge));
			}
		}
		return res;
	}
	public static Edge getEdge(String path){
		return edges.get(path);
	}
	public static Collection<Edge> getAllEdge(){
		return edges.values();
	}
}

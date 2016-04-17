package org.ucf.java.MF;
/**
 * @author Bing
 */
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class FordFulkerson {
	private Set<Edge> path=new HashSet<Edge>();
	public FordFulkerson(){
		super();
	}
	public int getMaxStream(String start,String end){
		while(hasAugmentPath(start,end)){
			int minRemain=getMinRemain(path);
			for(Edge edge:path){
				Edge reverseEdge=DataSource.getEdge(edge.getReversePath());//get the return path
				//calculate the current flow value
				if(edge.getRemain()>edge.getCapacity()){//the residural value is bigger than capacity 
					if(edge.getRemain()>=reverseEdge.getCapacity()){
						reverseEdge.setValue(reverseEdge.getCapacity());
						edge.setValue(edge.getRemain()-reverseEdge.getCapacity());
					}else{
						reverseEdge.setValue(edge.getRemain());
						edge.setValue(0);
					}
				}else{
					edge.setValue(edge.getValue()+minRemain);
				}
				
				edge.setRemain(edge.getRemain()-minRemain);
				reverseEdge.setRemain(reverseEdge.getRemain()+minRemain);
			}
			path.clear();
		}
		return 0;
	}
	public void printStreamInfo(){
		List<Edge> edges=DataSource.nextEdge("s");
		int maxStream=0;
		for(Edge edge:edges){
			if(edge.getCapacity()!=0){
				maxStream+=edge.getValue();
			}
		}
		System.out.println("MaxFlow value is："+maxStream);
		System.out.println("Flow info is：");
		for(Edge edge:DataSource.getAllEdge()){
			if(edge.getCapacity()!=0){
				System.out.println(edge.getStart()+"->"+edge.getEnd()+"\t:\t"+edge.getValue());
			}
		}
	}
	private boolean hasAugmentPath(String start, String end) {
		for(Edge edge:DataSource.nextEdge(start)){
			if(path.contains(edge) || edge.getRemain()==0){
				continue;
			}
			path.add(edge);
			if(edge.getEnd().equals(end)){
				return true;
			}else{
				if(hasAugmentPath(edge.getEnd(),end)){
					return true;
				}else{
					path.remove(edge);
					continue;
				}
			}
		}
		return false;
	}
	private int getMinRemain(Set<Edge> edges){
		int res=Integer.MAX_VALUE;
		for(Edge edge:edges){
			if(edge.getRemain()<res){
				res=edge.getRemain();
			}
		}
		return res;
	}
}
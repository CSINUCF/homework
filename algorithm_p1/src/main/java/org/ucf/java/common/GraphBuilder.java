package org.ucf.java.common;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.*;

/**
 * Created by Bing on 3/16/2016.
 */
public class GraphBuilder {
    public Vector<Node> Vertix = new Vector<Node>(); // the set of Vertex as the based constructor
    public List<Edge> edgeList = new ArrayList<Edge>(); // the set of edges
    public Set<Node> Vertix_Set  = new HashSet<Node>(); // the set of vertex which are used as open set in the Dijkstra Algorithm
    private  String [] nodeName = {"A","B","C","D","E","F","G","H","I"};

    private void buildVertex() {
        Vertix.add(null);
        for(int i = 0;i<nodeName.length;i++) {
            Node tmp = new Node(nodeName[i],i+1);
            this.Vertix.add(tmp);
            this.Vertix_Set.add(tmp);
        }
    }
    private static String[] getTokens(String input) {
        int i = 0;
        StringTokenizer st = new StringTokenizer(input);
        int numTokens = st.countTokens();
        String[] tokenList = new String[numTokens];
        while (st.hasMoreTokens()){
            tokenList[i] = st.nextToken();
            i++;
        }
        return(tokenList);
    }
    public GraphBuilder(String filepath) {
        buildVertex();
        try{
            String encoding = "GBK";
            File file = new File(filepath);
            if(file.isFile() && file.exists())
            {
                InputStreamReader read = new InputStreamReader(new FileInputStream(file),encoding);
                BufferedReader readbuffer = new BufferedReader(read);
                String lineTxt = null;
                while((lineTxt = readbuffer.readLine()) != null)
                {
                    String[] test = getTokens(lineTxt.replaceAll(",", " "));
                    Integer src_idx = Integer.parseInt(test[0]);
                    for(int i = 1;i<test.length;i+=2)
                    {
                        Integer dst_idx = Integer.parseInt(test[i]);
                        Integer edge_wgt = Integer.parseInt(test[i+1]);
                        Node src = Vertix.get(src_idx);
                        Node dst = Vertix.get(dst_idx);
                        src.addChild(dst,edge_wgt);
                        if(src_idx<dst_idx)
                            edgeList.add(new Edge(src,dst,edge_wgt));
                    }
                }
                readbuffer.close();
            }
            else
            {
                System.out.println("The file is not exit, please double check:"+filepath);
            }

        }catch (Exception e){
            e.printStackTrace();
        }
    }
    public void printNodeInfo() {
        System.out.println("Name\tIndex");
        for(int i = 0;i<nodeName.length;i++)
            System.out.println(nodeName[i]+"\t\t"+(i+1));
    }
    public void printEdgeInfo() {
        for(int i = 0;i<edgeList.size();i++) {
            edgeList.get(i).printinfo();
        }
    }
}

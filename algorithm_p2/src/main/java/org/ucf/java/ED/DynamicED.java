package org.ucf.java.ED;

/**
 * Dynamically calculate the distance between two strings
 * 
 * Distance represents the number of operations it would require to transform
 * the first string into the other string.
 * 
 * @author Bing
 */
public class DynamicED
{
    /**
     * The memoized array for calculations
     */
    public DynamicED(int gp,int sp){
    	this.GAP_COST = gp;
    	this.SUB_COST = sp;
    }
    int[][] memo;
    /**
     * The cost of a gap operations (adding/removing a letter)
     */
    public int GAP_COST;
    /**
     * The cost of swapping a character for another
     */
    public int SUB_COST;
   
    
    public int aLength = 0;
    public int bLength = 0;
    public static int MATCH = 0;
    public static int INSERT = 1;
    public static int DELETE = 2;
    
    /**
     * Calculate the distance using dynamic programming.
     * 
     * @param a the original string
     * @param b the destination string
     * @return the distance
     */
    public int distance(final String a, final String b) {
    	
    	this.aLength = a.length();
    	this.bLength = b.length();
    	
        this.memo = new int[a.length()+1][b.length()+1];
        memo[0][0] = 0;
        for (int i = 1; i < a.length(); i++) {
            memo[i][0] = i*GAP_COST;
        }
        for (int j = 1; j < b.length(); j++) {
            memo[0][j] = j*GAP_COST;
        }
       
        int[] opt = new int[3];
        for (int i = 1; i < a.length()+1; i++) {
            for (int j = 1; j < b.length()+1; j++) {
                opt[MATCH] = memo[i-1][j-1] + ((a.charAt(i-1) == b.charAt(j-1)) ? 0 : SUB_COST);
                opt[INSERT] = memo[i][j-1] + GAP_COST;
                opt[DELETE] = memo[i-1][j] + GAP_COST;
                
                /**Choose the min value and store*/
                memo[i][j] = opt[0];
                for (int k = 1; k < 3; k++) {
                    if (opt[k] < memo[i][j]) {
                        memo[i][j] = opt[k];
                    }
                }
            }
        }
        return memo[a.length()][b.length()];
    }
    /**
     * Print the result matrix table info to debug
     */
    public void printDistanceTable(){
    	int row = memo.length;
    	int col = memo[0].length;
    	
    	System.out.println(row);
    	System.out.println(col);
    	
    	for(int i=0;i<row;i++){
    		System.out.println();
    		for(int j = 0;j<col;j++){
    			System.out.print(memo[i][j]+"\t");
    		}	
    	}	
    }
}

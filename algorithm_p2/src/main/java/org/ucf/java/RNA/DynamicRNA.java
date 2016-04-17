package org.ucf.java.RNA;
import java.util.TreeMap;
/**
 * Determine the maximum folding of an RNA sequence using
 * dynamic programming.
 * @author Bing
 */
public class DynamicRNA
{
    /**
     * The memoized array of folding values
     */
    
	public DynamicRNA(){
		super();
	}
    public int[][] memo;
    
    /**
     * The trace table of possible TreeMaps
     */
    
    public TreeMap<Integer,Integer>[][] trace;
    
    /**
     * Find the secondary structure using dynamic programming
     * 
     * @param inputSequence the sequence in consideration
     * @return the TreeMap of the secondary structure
     */
    @SuppressWarnings("unchecked")
	public TreeMap<Integer,Integer> secondaryStructure(String inputSequence) {
    	
    	int length = inputSequence.length();
        memo = new int[length][length];
        
        // Unsafe operation here, but it still compiles.
        trace = new TreeMap[length][length];
        for (int i = 0; i < trace.length; i++) {
            for (int j = 0; j < trace.length; j++) {
                trace[i][j] = new TreeMap<Integer,Integer>();
            }
        }
        for (int k = 4; k < length; k++) {
            for (int i = 0; i < length - k; i++) {
                int j = i + k;
                memo[i][j] = memo[i][j-1];
                trace[i][j] = trace[i][j-1];
                int value = 0;
                for (int t = i; t < j - 5; t++) {
                	/*if tth and jth are pair of based pair*/
                    if (isMatch(inputSequence,t,j)) {
                        TreeMap<Integer,Integer> map = new TreeMap<Integer,Integer>();
                        if (t == 0) {
                            map.put(t,j);
                            map.putAll(trace[t+1][j-1]);
                            value = 1+memo[t+1][j-1];
                        }
                        else {
                            map.put(t,j);
                            map.putAll(trace[i][t-1]);
                            map.putAll(trace[t+1][j-1]);
                            value = 1 + memo[i][t-1] + memo[t+1][j-1];
                        }
                        if (value > memo[i][j]) {
                            memo[i][j] = value;
                            trace[i][j] = map;
                        }
                    }
                }
            }
        }
        return trace[0][length-1];
    }
    /**
     * Is the character in position i a match for the character in position j?
     * 
     * Returns true of A-U or C-G match is found
     * 
     * @param inputSequence the string in consideration
     * @param i the ith position
     * @param j the jth position
     * @return true if there is a match, false otherwise
     */
    private boolean isMatch(String inputSequence, int i, int j) {
        return ((inputSequence.charAt(i) == 'U' && inputSequence.charAt(j) == 'A' ||
            inputSequence.charAt(i) == 'C' && inputSequence.charAt(j) == 'G' ||
            inputSequence.charAt(j) == 'U' && inputSequence.charAt(i) == 'A' ||
            inputSequence.charAt(j) == 'C' && inputSequence.charAt(i) == 'G'));
    }
}

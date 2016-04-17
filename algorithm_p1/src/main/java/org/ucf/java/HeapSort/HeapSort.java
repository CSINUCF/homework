package org.ucf.java.HeapSort;

/**
 * Created by Bing on 3/16/2016.
 */

public class HeapSort {
    private int[] heap;
    public HeapSort(int[] array)
    {
        this.heap = array;
    }
    public int Parent(int i){
        return (i-1)/2;
    }
    public int Left(int i){
        return 2*(i+1)-1;
    }
    public int Right(int i){
        return 2*(i+1);
    }
    public void siftup(int idx) {
        int p_idx = Parent(idx);
        if(p_idx<0) {
            return;
        }
        else
        {
            if(heap[p_idx]>heap[idx])
            {
                int tmp = heap[p_idx];
                heap[p_idx] = heap[idx];
                heap[idx] = tmp;
                siftup(p_idx);
            }
            else
                return;
        }

    }
    public void siftdown(int idx){
        int l = Left(idx);
        int r = Right(idx);
        int lgt_idx;
        if((l<heap.length-1)&&(r<heap.length-1))
        {
            if(heap[l]>heap[r])
            {
                /*if the left value is bigger then the right one*/
                lgt_idx = r;
            }
            else
            {
                /*if the left value is smaller or equal to the right one*/
                lgt_idx = l;
            }
        }
        else if(l == heap.length -1){
            lgt_idx = l;
        }
        else if(r == heap.length-1){
            lgt_idx = r;
        }else{
            return;
        }
        if(heap[lgt_idx]<heap[idx]) {
            int tmp = heap[lgt_idx];
            heap[lgt_idx] = heap[idx];
            heap[idx] = tmp;
            siftdown(lgt_idx);
        }
        else {
            return;
        }
    }
    public void BuildMinHeap() {
        for(int i = 0;i<heap.length;i++){
            siftup(i);
        }
    }
    public void HeapSortTree() {
        int[] tmp_a = new int[heap.length];
        for(int i=0;i<heap.length;i++)
        {
            tmp_a[i] = heap[0];
            heap[0]=heap[heap.length-1-i];
            heap[heap.length-1-i] = Integer.MAX_VALUE;
            siftdown(0);
        }
        for(int i=0;i<heap.length;i++)
        {
            heap[i] = tmp_a[i];
        }
    }

    public void printHeapTree() {
        for(int i=1;i<heap.length;i=i*2){
            for(int k = i-1;k<2*i-1&&k<heap.length;k++){
                System.out.println(heap[k]+" ");
            }
            System.out.println();
        }
    }
    public void printHeap(){
        for(int i = 0; i<heap.length;i++){
            System.out.print(heap[i]+" ");
        }
        System.out.println();
    }
    public int[] getHeap(){
        return heap;
    }
}

package org.ucf.hw.PrimeNumber;
import java.util.Arrays;

class PrimeNumber implements Runnable
{
	int Number_start;
	int Number_end;
	Thread tmp;
	long StartTime;
	long EndTime;
	int Prime_count;
	long Prime_sum;
	int[] last_primes_10;
	
	
	PrimeNumber(int start,int end,String name)
	{
		tmp = new Thread(this,name);
		this.Number_start = start;
		this.Number_end = end;
		this.StartTime = 0;
		this.EndTime = 0;
		this.Prime_count = 0;
		this.Prime_sum = 0;
		last_primes_10 = new int[10];
		Arrays.fill(last_primes_10, 0);
		tmp.start();
	}
	private boolean IsPrime(int Number)
	{
		if(Number <= 1)
			return false;
		long k = (int) Math.sqrt(Number);
		for(long i = 2;i<=k;i++)
		{
			if(Number%i == 0)
				return false;
		}
		return true;
	}
	/*
	private boolean IsPrime(int Number)
	{
		if(Number <= 1)
			return false;
		long k = Number -1;
		for(long i = 2;i<=k;i++)
		{
			if(Number%i == 0)
				return false;
		}
		return true;
	}
	*/
	public void run()
	{
			this.StartTime = System.nanoTime();
			for(int i = this.Number_start;i>=this.Number_end;i--)
			{
				
				if(IsPrime(i) == true)
				{
					this.Prime_sum += i;
					if(this.Prime_count<10)
					{
						this.last_primes_10[this.Prime_count] = i;
					}
					this.Prime_count++;
				}
				
			}
			this.EndTime = System.nanoTime();
	}
	long thread_spendtime()
	{
		return (this.EndTime - this.StartTime);
	}
}
class PrimesObject{
	final int prime_start;
	final int prime_end;
	final int a1;
	final int d;
	
	final int thread_count;
	long prime_count = 0;
	long prime_sum = 0;
	long prime_time= 0;
	long start_time = 0;
	long end_time = 0;
	PrimesObject()
	{
		this.prime_start = 100000000;
		this.prime_end = 1;
		this.thread_count = 8;/*the parameter to control the number of issued threads*/
		this.a1 = 10000000;
		this.d = 700000;
	}
	int get_number_count(int t_idx)
	{
		int an = a1 +(t_idx-1)*d;
		return (a1+an)*t_idx/2;
	}
	int get_numer_start(int t_idx)
	{
		int start =0;
		if(t_idx == 1)
		{
			start = prime_start;
		}
		else
		{
			start = prime_start - get_number_count(t_idx-1)-1;
		}
		if(start<this.prime_end)
		{
			System.out.println("the start number is:"+start);
			start = this.prime_end;
		}
		return start;
	}
	int get_numer_end(int t_idx)
	{
		int end = prime_start - get_number_count(t_idx);
		if((t_idx == this.thread_count)&&(end>this.prime_end))
		{
			end = this.prime_end;
		}
		if(end<this.prime_end)
		{
			System.out.println("the end number is:"+end);
			end = this.prime_end;
		}
		return end;
	}
}
class PrimeNumberMainClass 
{
	public static void main(String args[])
	{
		
		PrimesObject prime_task = new PrimesObject();
		PrimeNumber[] Pnumber_t = new PrimeNumber[prime_task.thread_count];
		String[] thread_name = {"p_0","p_1","p_2","p_3","p_4","p_5","p_6","p_7"};
		int i,start = 0,end = 0;
	
		try
		{
			/*To record the start time when the primes thread start*/
			prime_task.start_time = System.nanoTime();
			for(i=0;i<prime_task.thread_count;i++)
			{
				start = prime_task.get_numer_start(i+1);
				end = prime_task.get_numer_end(i+1);
				
				/*Create thread instances and start each of them to deal with primes problem*/
				Pnumber_t[i] = new PrimeNumber(start,end,thread_name[i]);
			}
			
			/*Waiting for primes threads to finish*/
			for(i=0;i<prime_task.thread_count;i++)
			{
				Pnumber_t[i].tmp.join();
			}
		}
		catch (InterruptedException e)
		{
			System.out.println("Main thread has been interrupted");
		}finally{
			/*To record the current time when the primes thread finished*/
			prime_task.end_time = System.nanoTime();
			/*The execution time of these threads*/
			prime_task.prime_time = prime_task.end_time - prime_task.start_time;
		}
		/*Print the results of each thread execute*/
		System.out.println("\nThe results of each thread is followed:");
		for(i=0;i<prime_task.thread_count;i++)
		{
			System.out.print(Pnumber_t[i].tmp.getName()+"["+Pnumber_t[i].Number_start+":"+Pnumber_t[i].Number_end+":"+(Pnumber_t[i].Number_start - Pnumber_t[i].Number_end +1)+"]");
			System.out.print("\t\t\t Time<"+Pnumber_t[i].thread_spendtime()+">");
			System.out.print("\t\t Number<"+Pnumber_t[i].Prime_count+">");
			System.out.println("\t\t Sum<"+Pnumber_t[i].Prime_sum+">");
			prime_task.prime_count += Pnumber_t[i].Prime_count;
			prime_task.prime_sum += Pnumber_t[i].Prime_sum;
			
		}
		
		/*Print the result of main thread to compare with the up threads*/
		System.out.println("\nThe result of main thread is followed:");
		System.out.print(Thread.currentThread()+"[1:"+prime_task.prime_end+"]");
		System.out.print("\t\t\t Time<"+prime_task.prime_time+">");
		System.out.print("\t\t Number<"+prime_task.prime_count+">");
		System.out.println("\t\t Sum<"+prime_task.prime_sum+">");
		
		System.out.println("The last ten primes number are:");
		for(i=0;i<prime_task.thread_count;i++)
		{
			if(i == 0)
			{
				for(int j=9;j>=0;j--)
				{
					int prime = Pnumber_t[i].last_primes_10[j];
					System.out.print(prime+" ");
				
				}
			}
		}
	}
}
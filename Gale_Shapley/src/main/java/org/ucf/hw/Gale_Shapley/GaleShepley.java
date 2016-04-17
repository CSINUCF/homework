package org.ucf.hw.Gale_Shapley;

/**
 * Java Program to implement Gale Shapley Algorithm 
 *
 */
public class GaleShepley
{
	public String[] m;
	public String[] w;
	public String[][] m_pref;
	public String[][] w_pref;
	public int N,engagedCount;
	public Boolean[] free;
	public String[] wife;
	public String[] husband;
	
	public GaleShepley(String[] men,String[] women,String[][] mp,String[][] wp)
	{
		this.m = men;
		this.w = women;
		this.m_pref = mp;
		this.w_pref = wp;
		this.N = mp.length;
		this.engagedCount = 0;
		this.free = new Boolean[N];
		this.wife = new String[N];
		this.husband = new String[N];
		for(int i=0;i<N;i++)
		{
			free[i] = false;
		}
		
		//System.out.println("N is"+N);
	}
	/*Return the index of man whose name is "name"*/
	public int indexofmen(String name)
	{
		for(int i=0;i<N;i++)
		{
			if(this.m[i].equals(name))
				return i;
		}
		return -1;
	}
	/*Return the index of woman whoase name is "name"*/
	public int indexofwomen(String name)
	{
		for(int i=0;i<N;i++)
		{
			if(this.w[i].equals(name))
				return i;
		}
		return -1;
	}
	/*Check whether or not woman prefer new partner over old assigned partner*/
	public Boolean preference_more(String curpartner,String newpartner,int th)
	{
		/*Get the preference list of index woman*/
		String[] woman_pref = this.w_pref[th];
		/*Traverse all woman preference list*/
		for(int i=0;i<N;i++)
		{
			if(woman_pref[i].equals(curpartner)== true)
			{
				/*
				 * if woman prefer current partner to new partner, 
				 * then return false
				 * */
				return false;
			}
			if(woman_pref[i].equals(newpartner) == true)
			{
				/*
				 * if woman prefer new partner to current partner,
				 * then return true;
				 * */
				return true;
			}
		}
		return false;
	}
	/*calculate the stable match policy*/
	public void stable_match()
	{
		String man;
		String[] man_pref;
		int first_free;
		while(engagedCount < N)
		{
			/*find the first man who is not engaged*/
			for(first_free = 0;first_free<N;first_free++)
			{
				if(free[first_free] == false)
				{
					break;
				}
			}
			man = this.m[first_free];
			man_pref = this.m_pref[first_free];
			//System.out.println("No."+engagedCount+":"+man);
			/*Check the prefer list of this man who is not engaged one by one*/
			for(int i=0;i<N && !free[first_free];i++)
			{
				/*Get the prefer name of woman*/
				String woman_name = man_pref[i];
				/*Get the index of current prefer woman*/
				int idx_w = indexofwomen(woman_name);
				/*Check current prefer woman whether or not is engaged*/
				if(husband[idx_w] == null)
				{
					/*the prefer woman is not engaged with some guy*/
					wife[first_free] = woman_name;
					free[first_free] = true;
					husband[idx_w] = man;
					engagedCount++;
					//System.out.println("engagedCount:"+engagedCount);
				}
				/*
				 * The prefer woman is engaged and check which one (current partner and new partner)
				 * is better to this woman
				 * */
				else if(preference_more(husband[idx_w],man,idx_w) == true)
				{/*if new partner is better than the current one*/
					
					/*get current partner index*/
					int idx_m = indexofmen(husband[idx_w]);
					wife[idx_m] = null;
					free[idx_m] = false;
					husband[idx_w] = man;
					
					wife[first_free] = woman_name;
					free[first_free] = true;
				}				
			}				
		}
	}
	public void printCouples()
	{
		System.out.println("Couples are:");
		for(int i=0;i<N;i++)
		{
			System.out.println(husband[i]+":"+w[i]);
		}
	}
}

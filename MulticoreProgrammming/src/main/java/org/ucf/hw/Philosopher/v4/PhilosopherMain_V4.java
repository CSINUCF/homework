package org.ucf.hw.Philosopher.v4;
/**
 * @author Bing
 *
 */
import java.util.concurrent.atomic.AtomicInteger;

import javax.swing.JOptionPane;
class Philosopher_t implements Runnable
{
	Thread tmp;
	Waitor waiter;
	int P_idx;
	int task_status;
	final int TASK_SUSPEND = 1;
	final int TASK_RUN =2;
	final int TASK_EXIT =3;
	Philosopher_t(String name,int idx,Waitor w)
	{
		this.tmp = new Thread(this,name);
		this.P_idx = idx;
		this.waiter = w;
		this.task_status = TASK_SUSPEND;
		this.tmp.start();
	}
	/*philosopher think function*/
	private void p_thinking()
	{
		System.out.println("Philosopher["+this.P_idx+"] is now thinking");
		try{
			Thread.sleep(1200);//sleep and schedule the CPU
		}catch (InterruptedException e){
			e.printStackTrace();
		}
	}
	/*philosopher eating function*/
	private void p_eating()
	{
		System.out.println("Philosopher["+this.P_idx+"] is now eating");
		try{
			Thread.sleep(500);//sleep and schedule the CPU
		}catch (InterruptedException e){
			e.printStackTrace();
		}
	}
	@Override
	/*Override the thread run function to execute the task*/
	public void run()
	{
		boolean status = true;
		while(status)
		{
			/*According to different task status, the program will execute different function part*/
			switch (this.task_status)
			{
				case TASK_RUN:
				{
					/*If we set thread status is "TASK_Run", the thread will simulate the philosopher problem*/
					p_thinking();
					this.waiter.require_chopsticks(this.P_idx);
					p_eating();
					this.waiter.release_chopsticks(this.P_idx);
				}break;
				case TASK_SUSPEND:
				{
					/*Sleep and schedule CPU*/
					try {
						Thread.sleep(100);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}break;
				case TASK_EXIT:
				{
					/*stop the thread and exit while loop*/
					status = false;
				}break;
				default:
				{
					System.out.println("The task status is wrong:"+this.task_status);
				}
			}
		}
		System.out.println("Philosopher["+this.P_idx+"] is now exiting");
	}
	/*Change current task executing status*/
	void current_task_status_change(int status)
	{
		this.task_status = status;
	}
}

class ShareChopstick
{
	int C_idx;
	boolean C_Status; /*if the value is true, the chopstick is being used, otherwise it is not being used*/
	
	ShareChopstick(int idx)
	{
		this.C_idx = idx;
		this.C_Status = false;
	}
	boolean get_chopstick_status()
	{
		return this.C_Status;
	}
	/*get the chopstick and set its status is in use*/
	void get_chopstick()
	{
		this.C_Status = true;
	}
	/*after eating, release the chopstick and set its status is unused*/
	void free_chopstick()
	{
		this.C_Status = false;
	}
}
/*
 * Define this class to schedule and manger the chopstick resources among these Philosophers, if one want to sit down and 
 * eat, he/she should ask for the waitor and wait for her respondence
 * */
class Waitor
{

	ShareChopstick[] chopsticks;
	ThreadLocal <Integer> myslot = new ThreadLocal <Integer> (){
		@SuppressWarnings("unused")
		protected Integer initiaValue()
		{
			return 0;
		}
	};
	AtomicInteger tail;
	int Chopsticks_cout;
	boolean[] flag;
	Waitor(int count)
	{
		this.Chopsticks_cout = count;
		chopsticks = new ShareChopstick[this.Chopsticks_cout];
		for(int i=0;i<this.Chopsticks_cout;i++)
		{
			chopsticks[i] = new ShareChopstick(i);
		}
		tail =  new AtomicInteger(0);
		flag = new boolean[this.Chopsticks_cout];
		flag[0] = true;
	}
	/*Interface that philosopher will use to require the chopsticks*/
	public synchronized void require_chopsticks(int p_idx)
	{
		System.out.println("Philosopher["+p_idx+"] is now hungry");
		int slot = tail.getAndIncrement()%this.Chopsticks_cout;
		myslot.set(slot);
		while((!flag[slot])&&(chopsticks[p_idx%this.Chopsticks_cout].get_chopstick_status()||chopsticks[(p_idx+1)%this.Chopsticks_cout].get_chopstick_status()))
		{
			/*if the left and right chopsticks are being used, and wait*/
			try{
				wait();
			}catch (InterruptedException e){
				e.printStackTrace();
			}
		}
		/*Both chopsticks are not being used, so the current philosopher get them*/
		chopsticks[p_idx%this.Chopsticks_cout].get_chopstick();
		chopsticks[(p_idx+1)%this.Chopsticks_cout].get_chopstick();
	}
	/*
	 * After eating, release the chopsticks
	 * */
	public synchronized void release_chopsticks(int p_idx)
	{
		int slot = this.myslot.get();
		this.flag[slot%this.Chopsticks_cout] = false;
		this.flag[(slot+1)%this.Chopsticks_cout] = true;
		this.chopsticks[p_idx%this.Chopsticks_cout].free_chopstick();
		this.chopsticks[(p_idx+1)%this.Chopsticks_cout].free_chopstick();
		notifyAll();
	}
}
/*
 * Define this class to control the program flow. In this program, I use JOptionPane to get the input data from the user,
 * after that the program will execute different part according to the input context.
 * */
class DialogOptionPane
{
	Philosopher_t[] Philosophers;
	Waitor manager_waiter;
	int Philosopher_count;

	DialogOptionPane(int count)
	{
		this.Philosopher_count = count;
		this.manager_waiter = new Waitor(this.Philosopher_count);
		this.Philosophers = new Philosopher_t[this.Philosopher_count];
	}
	void initiate()
	{
		int i= 0;
		String str1 = "p_";
		for(i=0;i<this.Philosopher_count;i++)
		{
			Philosophers[i] = new Philosopher_t(str1+i,i,manager_waiter);
		}
	}
	void wait_for_all_finished()
	{
		try{
			for(int i=0;i<this.Philosopher_count;i++)
			{
				Philosophers[i].tmp.join();
			}
		}catch (InterruptedException e){
			System.out.println("The Main Thread has been inturrepted");
		}
		
	}
	void change_alll_phil_thread_status(int stauts)
	{
		for(int i=0;i<this.Philosopher_count;i++)
		{
			this.Philosophers[i].current_task_status_change(stauts);
		}
	}
	
	void change_program_status()
	{
		String str1;
		boolean status = true;
		while(status)
		{
			str1 = JOptionPane.showInputDialog("input(N:stop,R:start;S:suspend)");
			if(str1 == null)
			{
				JOptionPane.showMessageDialog(null,"If you want exit,please input n");
			}
			else if(str1.matches("\\p{Lower}")||str1.matches("\\p{Upper}"))
			{
				switch (str1.toUpperCase())
				{
					case "N"://stop
					{
						change_alll_phil_thread_status(3);
						status = false;
					}break;
					case "R"://resume
					{
						change_alll_phil_thread_status(2);
					}break;
					case "S"://suspend
					{
						change_alll_phil_thread_status(1);
					}break;
					default:
					{
						JOptionPane.showMessageDialog(null,"Input Wrong please input again");
					}
				}
			}
		}
	}

	void run()
	{
		initiate();
		change_program_status();
		wait_for_all_finished();
	}
}
class PhilosopherMain_V4 {

	/**
	 * @param args
	 */
	static int input_and_verify_argument()
	{
		String str1;
		int thread_count = 0;
		while(true)
		{
			try
			{
				str1 = JOptionPane.showInputDialog("Please Inpute the Number of Threads");
				if(str1 == null)
				{
					thread_count = -1;
					System.out.println("The User Cancelled,please check it again");
					break;
				}
				if(str1.matches("\\d*"))
				{
					thread_count = Integer.parseInt(str1);
					if(thread_count != 0)
					{
						System.out.println("The inpute Number is:"+thread_count);
						break;
					}
					else
					{
						JOptionPane.showMessageDialog(null, "Input Wrong, please Input Number again");
					}
				}
				else
				{
					JOptionPane.showMessageDialog(null, "Input Wrong, please Input Number again");
				}
			}catch(NumberFormatException e) {
				e.printStackTrace();
				thread_count = -1;
				return thread_count;
			}finally{
				System.out.println("");
			}
		}
		return thread_count;
	}
	public static void main(String[] args) 
	{
		/**/
		int count = input_and_verify_argument();
		if(count != -1)
		{
			DialogOptionPane philosopher_main = new DialogOptionPane(count);
			
			philosopher_main.run();
		
			System.out.println("The philosophers have finished eating and Thanks for the meal");
		}
		else
		{
			System.out.println("The inpute Arugment is wrong, please check it again");
		}
		System.exit(0);
	}
}
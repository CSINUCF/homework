package org.ucf.hw.Philosopher.v2;
/**
 * @author Bing
 *
 */
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
	private void p_thinking()
	{
		System.out.println("Philosopher["+this.P_idx+"] is now thinking");
		try{
			Thread.sleep(1200);
		}catch (InterruptedException e){
			e.printStackTrace();
		}
	}
	private void p_eating()
	{
		System.out.println("Philosopher["+this.P_idx+"] is now eating");
		try{
			Thread.sleep(500);
		}catch (InterruptedException e){
			e.printStackTrace();
		}
	}
	@Override
	public void run()
	{
		boolean status = true;
		while(status)
		{
			switch (this.task_status)
			{
				case TASK_RUN:
				{
					p_thinking();
					this.waiter.require_chopsticks(this.P_idx);
					p_eating();
					this.waiter.release_chopsticks(this.P_idx);
				}break;
				case TASK_SUSPEND:
				{
					try {
						Thread.sleep(100);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}break;
				case TASK_EXIT:
				{
					status = false;
				}break;
				default:
				{
					System.out.println("the system is out");
				}
			}
		}
		System.out.println("Philosopher["+this.P_idx+"] is now exiting");
	}
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
	void get_chopstick()
	{
		this.C_Status = true;
	}
	void free_chopstick()
	{
		this.C_Status = false;
	}
}
class Waitor
{
	ShareChopstick[] chopsticks = new ShareChopstick[5];
	Waitor()
	{
		for(int i=0;i<5;i++)
		{
			chopsticks[i] = new ShareChopstick(i);
		}
	}
	public synchronized void require_chopsticks(int p_idx)
	{
		System.out.println("Philosopher["+p_idx+"] is now hungry");
		while(chopsticks[p_idx].get_chopstick_status()||chopsticks[(p_idx+1)%5].get_chopstick_status())
		{
			/*If there is at least one more chopstick are being used and we should wait*/
			try{
				wait();
			}catch (InterruptedException e){
				e.printStackTrace();
			}
		}
		chopsticks[p_idx].get_chopstick();
		chopsticks[(p_idx+1)%5].get_chopstick();
	}
	public synchronized void release_chopsticks(int p_idx)
	{
		chopsticks[p_idx].free_chopstick();
		chopsticks[(p_idx+1)%5].free_chopstick();
		notifyAll();
	}
}
class DialogOptionPane
{
	Philosopher_t[] Philosophers;
	Waitor manager_waiter;
	DialogOptionPane()
	{
		this.Philosophers = new Philosopher_t[5];
		this.manager_waiter = new Waitor();
	}
	void initiate()
	{
		int i= 0;
		String str1 = "p_";
		for(i=0;i<5;i++)
		{
			Philosophers[i] = new Philosopher_t(str1+i,i,manager_waiter);
		}
	}
	void wait_for_all_finished()
	{
		try{
			for(int i=0;i<5;i++)
			{
				this.Philosophers[i].tmp.join();
			}
		}catch (InterruptedException e){
			System.out.println("The Main Thread has been inturrepted");
		}
	}
	void change_alll_phil_thread_status(int stauts)
	{
		for(int i=0;i<5;i++)
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
class PhilosopherMain_V2 
{
	public static void main(String[] args)
	{
		DialogOptionPane ControlPane = new DialogOptionPane();
		ControlPane.run();
		System.out.println("The philosophers have finished eating and Thanks for the meal");
	}

}


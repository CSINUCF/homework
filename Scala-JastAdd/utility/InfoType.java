package utility;
public enum InfoType {
	Normal(0),
	RDkill(1),
	RDgen(2),
	RDentry(3),
	RDexit(4);
	private int index;
	private InfoType(int idx){
		this.index = idx;
	}
	public int getValue()
	{
		return index;
	}
}

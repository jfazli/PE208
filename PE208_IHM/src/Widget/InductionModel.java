package Widget;

import nativeIhm.NativeData;
import ej.microui.Listener;
import ej.mwt.Widget;

public abstract class InductionModel extends Widget implements Listener{
	
	private int data;
	protected Listener listener;
	
	public InductionModel(){
		
	}
	

	public void setData(int data, NativeData nd){
		
//		if(setChanged(data)){
			notifyListener(this.data,nd);
			this.data = data;
//		}
		
	}
	
	public boolean setChanged(int data){
		if(this.data != data)
			return true;
		return false;
	}
	
	public void setListener(Listener listener){
		this.listener=listener;
	}
	
	 public void notifyListener(int valeur) {
		 	NativeData nd = null;
			this.listener.performAction(valeur,nd);	
	 }
	
	public void notifyListener(int valeur, NativeData nd) {
		this.listener.performAction(valeur,nd);	
	}
	
}

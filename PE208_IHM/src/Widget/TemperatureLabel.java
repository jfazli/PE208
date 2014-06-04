package Widget;

import nativeIhm.NativeData;
import nativeIhm.ScrutationInfos;
import ej.microui.Listener;

public class TemperatureLabel extends Label{

	private String title;
	private int value;
	private String unite;
	
	public TemperatureLabel(String title, int value, String unite) {
		super();
		this.title = title;
		this.setValue(value);
		this.unite = unite;
	}
	
	public void setValue(int value) {
		this.value = value;
	}

	public int getValue() {
		return value;
	}
	
	public void performAction() {System.out.println("not implemented");}

	public void performAction(int value) {
		System.out.println("not implemented");
	}

	public void performAction(int value, Object object) {		
		
		NativeData nd = (NativeData) object;
		
		if(nd.getSelectionMode() == NativeData.MODE_FONCTIONNEMENT ){
			int dizaine = value;
			setValue(value);
			setText(this.title+ String.valueOf(dizaine) /*+","+ String.valueOf(value-(10*dizaine))*/ + this.unite);
			setVisible(true);
		}
		else{
			setText(null);
			setVisible(false); 
		}

		
	}
}

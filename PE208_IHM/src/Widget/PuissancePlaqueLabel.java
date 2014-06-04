package Widget;

import nativeIhm.NativeData;
import ej.microui.Listener;

public class PuissancePlaqueLabel extends Label implements Listener{

	private String title;
	private int index;
	private int value=-1;
		
	public int getIndex() {
		return index;
	}


	public void setIndex(int index) {
		this.index = index;
	}


	public String getTitle() {
		return title;
	}


	public void setTitle(String title) {
		this.title = title;
	}


	public int getValue() {
		return value;
	}


	public void setValue(int value) {
		this.value = value;
	}


	public String getUnite() {
		return unite;
	}


	public void setUnite(String unite) {
		this.unite = unite;
	}

	private String unite;
	
	public PuissancePlaqueLabel(String title, int value, String unite,int index) {
		super();
		this.title = title;
		this.value = value;
		this.unite = unite;
		this.index = index;
	}
	
	
	public void performAction() {
		System.out
				.println("RecipeParameterWidget.performAction() --> NOT IMPLEMENTED");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see ej.microui.Listener#performAction(int)
	 */
	public void performAction(int value) {
		System.out
				.println("RecipeParameterWidget.performAction(int) --> NOT IMPLEMENTED");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see ej.microui.Listener#performAction(int, java.lang.Object)
	 */
	public void performAction(int value, Object object) {
		NativeData nd = (NativeData) object;
		String foyerSelect="";
		
		
		if(nd.getSelectionMode() == NativeData.MODE_FONCTIONNEMENT  && index == nd.getSelectionFoyer() ){
			setValue(value);
			setText(this.title+ String.valueOf(value));
			setVisible(true);
		}
		else{
			setText(null);
			setVisible(false); 
		}		
	}
}

package Widget;


import ej.microui.Listener;
import ej.mwt.MWT;


public class Label extends InductionModel implements Listener {

	private String text;
	private int alignment;

	public Label() {
		this(null, MWT.CENTER);
	}

	public Label(String text) {
		this(text, MWT.CENTER);
	}

	public Label(String text, int alignment) {
		super();
		this.text = text;
		this.alignment = alignment;
	}

	public int getAlignment() {
		return alignment;
	}

	public String getText() {
		return this.text;
	}

	public void setAlignment(int alignment) {
		this.alignment = alignment;
		revalidate();
	}

	public void setText(String text) {
		this.text = text;
		revalidate();
	}

	/*
	 * METHODS FROM THE INTERFACE 'LISTENER'
	 */

	/*
	 * (non-Javadoc)
	 * 
	 * @see ej.microui.Listener#performAction()
	 */
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
	public void performAction(int value, Object object) {
		System.out.println("RecipeParameterWidget.performAction(int) --> NOT IMPLEMENTED");
	}

}

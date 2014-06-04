package Composite;

import Resource.Resources;
import ej.microui.Listener;
import ej.mwt.Composite;
import ej.mwt.MWT;
import ej.mwt.Widget;

/**
 * Composite thats hold the widgets </pre>
 * 
 */
// Widget CS
public class PlaqueComposite extends Composite implements Listener {

	private final Widget lowerBar;
	private final Widget foyer;
	private final Widget button;
	private final Widget wireless;
	private String background;

	private static final int CENTRAl_MARGIN = 5;

	// FIXME: margin to prevent the central widget from printing on the color
	// gradients joining the tool bars and the central widget
	public PlaqueComposite(Widget foyer,Widget button, Widget lowerBar,Widget wireless, String background){
		super();
		super.add(lowerBar);
		super.add(foyer);
		super.add(button);
		super.add(wireless);
		
		this.lowerBar = lowerBar;
		this.foyer = foyer;
		this.background = background;
		this.wireless = wireless;
		this.button=button;
	}

	

	public void add(Widget widget) {
		// Not possible to add
	}

	public void validate(int widthHint, int heightHint) {

		// TODO : if not visible, no need to validate
		// See : com.is2t.mwt.composites.GridComposite.validate
		button.validate(widthHint*2/3, heightHint-Resources.HEIGHT_BUTTON/2+CENTRAl_MARGIN);
		int buttonWidth = Resources.WIDTH_BUTTON;
		int buttonHeight = Resources.HEIGHT_BUTTON;
		button.setBounds((widthHint-buttonWidth)/2, heightHint/2, buttonWidth, buttonHeight);
		
		
		wireless.validate(widthHint, MWT.NONE);
		int wirelessPositionPreferredHeight = wireless.getPreferredHeight();
		wireless.setBounds(widthHint * 7 / 8, 0 + CENTRAl_MARGIN, widthHint, wirelessPositionPreferredHeight);
		
		
		
		
		foyer.validate(widthHint, heightHint * 6 / 8 - CENTRAl_MARGIN);
		foyer.setBounds(0, 0, widthHint, heightHint * 7 / 8 - CENTRAl_MARGIN);
		
		/*hauteur du bouton = 30px*/
		
		
		lowerBar.validate(widthHint, heightHint / 8);
		lowerBar.setBounds(0, heightHint * 7 / 8, widthHint, heightHint / 8);
		
		

		// Set our own preferred size
		setPreferredSize(widthHint, heightHint);
	}

	public String getBackground() {
		return background;
	}

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

	/*
	 * (non-Javadoc)
	 * 
	 * @see ej.microui.Listener#performAction(int, java.lang.Object)
	 */
	public void performAction(int value, Object object) {
//		String selectionFoyer1 = "foyer1";
//		String selectionFoyer2 = "foyer2";
		
		/*Mode scrutation*/
		if(value == 2){
			if (this.background.equals(Resources.PLAQUE_INDUCTION) == true) {
				this.background = Resources.PLAQUE_INDUCTION_CHAUFFE;
			} else {
				this.background = Resources.PLAQUE_INDUCTION;
			}
		}
		else if (value == 0){
			this.background = Resources.PLAQUE_INDUCTION;
		}
		else if (value == 1){
			this.background = Resources.PLAQUE_INDUCTION_CHAUFFE;
		}
				
		repaint(); // ask for a display update
	}

}
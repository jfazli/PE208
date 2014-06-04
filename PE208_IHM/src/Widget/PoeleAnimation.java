package Widget;

import nativeIhm.NativeData;
import nativeIhm.PcomNative;
import Utilities.Animation;
import Utilities.Animator;
import Utilities.Utilities;
import ej.bon.Timer;
import ej.microui.Command;
import ej.microui.Event;
import ej.microui.Listener;
import ej.microui.io.Display;
import ej.microui.io.Image;
import ej.microui.io.Pointer;
import ej.mwt.Widget;

//Widget CS
public class PoeleAnimation extends InductionModel /*FoyerAnimation*/ implements Animator, Listener {
	private Image image;
	private int typeAnimation;
	private int bgColor;
	private int bgFocusedColor;
	private Animation animation;
	private final Timer timer;
	private int choixFoyer;
	private int choixAnimation;
	
	
	public PoeleAnimation(Image image, int typeAnimation, int bgColor,int bgFocusedColor) {
		super();
		this.image = image;
		this.typeAnimation = typeAnimation;
		this.bgColor = bgColor;
		this.bgFocusedColor = bgFocusedColor;

		this.timer = Utilities.getTimer();
		this.animation = null;
		
		/*
		 * if (this.animation == null || !this.animation.isRunning()) {
		 * this.animation = new Animation(this, this.timer, 2000, 100);
		 * this.animation.start(); }
		 */
	}

	public void setTypeAnimation(int typeAnimation) {
		this.typeAnimation = typeAnimation;
	}

	public int getTypeAnimation() {
		return typeAnimation;
	}

	public int getChoixFoyer() {
		return choixFoyer;
	}

	public void setChoixFoyer(int choixFoyer) {
		this.choixFoyer = choixFoyer;
	}

	public int getChoixAnimation() {
		return choixAnimation;
	}

	public void setChoixAnimation(int choixAnimation) {
		this.choixAnimation = choixAnimation;
	}
//	public int getBgColor() {
//		return bgColor;
//	}
//
//	public int getBgFocusedColor() {
//		return bgFocusedColor;
//	}

	public Image getImage() {
		return image;
	}


//	public void setBgColor(int bgColor) {
//		this.bgColor = bgColor;
//	}
//
//	public void setBgFocusedColor(int bgFocusedColor) {
//		this.bgFocusedColor = bgFocusedColor;
//	}

	public void setImage(Image image) {
		this.image = image;
		revalidate();
	}

//	public void setSelectionListener(Listener listener) {
//		this.selectionListener = listener;
//	}

//	protected void fireSelection() {
//		if (isEnabled() && selectionListener != null) {
//			selectionListener.performAction(0, this);
//		}
//	}

	/*
	 * private void _click() { if (this.animation == null ||
	 * !this.animation.isRunning()) { this.animation = new Animation(this,
	 * this.timer, 2000, 100); this.animation.start(); } }
	 */

	protected Listener selectionListener;

	public void update() {
		Display.getDefaultDisplay().callSerially(new Runnable() {
			public void run() {
				repaint();
//				revalidate();

			}
		});
	}

	public void begin() {
		// nothing to do
	}

	public void end() {
		this.animation = null;
//		revalidate();
		repaint();
		// fireSelection();
	}

	public Animation getAnimation() {
		return this.animation;
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
		/* On a détecté un foyer ==> apparition de la poele sur le bon foyer */
		NativeData nd = (NativeData) object;
		
		setChoixFoyer(nd.getSelectionFoyer());
		setTypeAnimation(nd.getSelectionMode());
		
		if(nd.getSelectionMode() == NativeData.MODE_SCRUTATION){
//			setVisible(true);
			if (this.animation == null || !this.animation.isRunning()) {
				this.animation = new Animation(this, this.timer, 400, 100);
				this.animation.start();
			}
		}
		else if(nd.getSelectionMode() == nd.MODE_FONCTIONNEMENT){
			if (this.animation != null ) {
				if(this.animation.isRunning())
					this.animation.stop();
			}
		}
	}
}

package Widget;

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

//CS
//animation de la page welcome
public class Picture extends Widget implements Animator {
	private Image image;
	private final int typeAnimation;
	private int bgColor;
	private int bgFocusedColor;
	private Animation animation;
	private final Timer timer;

	public static final int ANIMATION_ZOMM_DEZOOM = 0;
	public static final int ANIMATION_ROTATE = 1;

	public Picture(Image image, int typeAnimation, int bgColor,
			int bgFocusedColor) {
		super();
		this.image = image;
		this.typeAnimation = typeAnimation;
		this.bgColor = bgColor;
		this.bgFocusedColor = bgFocusedColor;

		this.timer = Utilities.getTimer();
		this.animation = null;

		if (this.animation == null || !this.animation.isRunning()) {
			this.animation = new Animation(this, this.timer, 3000, 100);
			this.animation.start();
			System.out.println("Picture");
		}
	}

	public int getTypeAnimation() {
		return typeAnimation;
	}

	public int getBgColor() {
		return bgColor;
	}

	public int getBgFocusedColor() {
		return bgFocusedColor;
	}

	public Image getImage() {
		return image;
	}

	public boolean handleEvent(int event) {
		int type = Event.getType(event);

		if ((Event.getType(event) == Event.COMMAND)
				&& (Event.getData(event) == Command.SELECT)) {

			return true;
		} else if (type == Event.POINTER) {
			int action = Pointer.getAction(event);
			if (action == Pointer.RELEASED) {
				Pointer pointer = (Pointer) Event.getGenerator(event);
				// retrieve coordinates of pointer relative to this widget
				// parent (Composite or Panel)
				int x = pointer.getX() - getAbsoluteX() + getX();
				int y = pointer.getY() - getAbsoluteY() + getY();
				if (contains(x, y)) {
					// _click();

					return true;
				}
			}
			// returned value doesn't matter, pointer events are not propagated
			// by system to hierarchies
			// fall down
		}

		return false;
	}

	public void setBgColor(int bgColor) {
		this.bgColor = bgColor;
	}

	public void setBgFocusedColor(int bgFocusedColor) {
		this.bgFocusedColor = bgFocusedColor;
	}

	public void setImage(Image image) {
		this.image = image;
		revalidate();
	}

	public void setSelectionListener(Listener listener) {
		this.selectionListener = listener;
	}

	protected void fireSelection() {
		if (isEnabled() && selectionListener != null) {
			selectionListener.performAction(0, this);
		}
	}

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
			}
		});
	}

	public void begin() {
		// nothing to do
	}

	public void end() {
		this.animation = null;
		fireSelection();
	}

	public Animation getAnimation() {
		return this.animation;
	}

}

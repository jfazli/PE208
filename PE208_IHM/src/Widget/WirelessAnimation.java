package Widget;
import ej.microui.io.Image;


public class WirelessAnimation extends InductionModel {

	private Image ImageOn;
	private Image ImageOff;
	private int StateImageOn;
	private int StateImageOff;

	public WirelessAnimation(Image ImageOn){
		super();
		this.ImageOn = ImageOn;
	}
	public WirelessAnimation(Image ImageOn, Image ImageOff) {
		super();
		this.ImageOn = ImageOn;
		this.ImageOff = ImageOff;
	}

	public Image getImageOn() {
		return ImageOn;
	}

	public void setImageOn(Image imageOn) {
		ImageOn = imageOn;
	}

	public Image getImageOff() {
		return ImageOff;
	}

	public void setImageOff(Image imageOff) {
		ImageOff = imageOff;
	}

	public int getStateImageOn() {
		return StateImageOn;
	}

	public void setStateImageOn(int stateImageOn) {
		StateImageOn = stateImageOn;
	}

	public int getStateImageOff() {
		return StateImageOff;
	}

	public void setStateImageOff(int stateImageOff) {
		StateImageOff = stateImageOff;
	}

	public void performAction() {
		// TODO Auto-generated method stub

	}

	public void performAction(int value) {
		// TODO Auto-generated method stub

	}

	public void performAction(int value, Object object) {
		// TODO Auto-generated method stub
		if (value == 1) {
			this.setStateImageOn(1);
			this.setStateImageOff(0);
		} else {
			this.setStateImageOn(0);
			this.setStateImageOff(1);
		}
		setVisible(true);
		repaint(); // ask for a display update

	}

}

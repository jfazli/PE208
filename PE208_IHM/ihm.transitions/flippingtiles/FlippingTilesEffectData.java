package flippingtiles;

import intf.DelayFunction;
import intf.Effect;
import intf.EffectData;
import ej.microui.io.Image;

public class FlippingTilesEffectData implements EffectData {

	/**
	 * Number of cols for tiles
	 */

	private final int cols;

	private final Image image1;

	private final Image image2;

	private final DelayFunction delayFunction;

	private final int phases;

	private final int pov;

	private final boolean rotateX;

	private final boolean rotateY;

	/**
	 * Number of rows for tiles
	 */
	private final int rows;

	public static FlippingTilesEffectData getEffectData(Image image1,
			Image image2, int cols, int rows, int phases, int pov,
			DelayFunction delayFunction, boolean rotateX, boolean rotateY) {

		return new FlippingTilesEffectData(image1, image2, cols, rows, phases,
				pov, delayFunction, rotateX, rotateY);

	}

	public DelayFunction getDelayFunction() {
		return delayFunction;
	}

	private FlippingTilesEffectData(Image image1, Image image2, int cols,
			int rows, int phases, int pov, DelayFunction delayFunction,
			boolean rotateX, boolean rotateY) {

		this.image1 = image1;
		this.image2 = image2;
		this.cols = cols;
		this.rows = rows;
		this.phases = phases;
		this.pov = pov;
		this.delayFunction = delayFunction;
		this.rotateX = rotateX;
		this.rotateY = rotateY;
	}

	public int getCols() {
		return cols;
	}

	public Image getImage1() {
		return image1;
	}

	public Image getImage2() {
		return image2;
	}

	public int getPhases() {
		return phases;
	}

	public int getPov() {
		return pov;
	}

	public int getRows() {
		return rows;
	}

	public boolean isRotateX() {
		return rotateX;
	}

	public boolean isRotateY() {
		return rotateY;
	}

	public Effect createEffect() {

		return new FlippingTiles(this);
	}

}

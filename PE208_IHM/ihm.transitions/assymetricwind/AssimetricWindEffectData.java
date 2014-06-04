package assymetricwind;

import intf.Effect;
import intf.EffectData;
import ej.microui.io.Image;

public class AssimetricWindEffectData implements EffectData {
	private float factor;

	public float getFactor() {
		return factor;
	}

	public void setFactor(float factor) {
		this.factor = factor;
	}

	/**
	 * Returns a new EffectData for Assimetric effect
	 * 
	 * @param appear
	 *            if true the image will appear (from a hided state), if false,
	 *            the image will disappear from the fully displayed state.
	 * @param image
	 *            the image
	 * @param cols
	 *            number of columns of tiles
	 * @param rows
	 *            number of rows of tiles
	 * @param phases
	 *            number of phases in the animation
	 * @param pov
	 *            camera distance from origo
	 * @param oneTilePhaseDelay
	 * @param rotateX
	 * @param rotateY
	 * @param rotateZ
	 * @return
	 */
	public static AssimetricWindEffectData getEffectData(boolean appear,
			boolean leftToRight, Image image, int cols, int rows, int phases,
			int pov, int oneTilePhaseDelay, boolean rotateX, boolean rotateY,
			float factor) {
		return new AssimetricWindEffectData(appear, leftToRight, image, cols,
				rows, phases, pov, oneTilePhaseDelay, rotateX, rotateY, factor);
	}

	private final boolean appear;
	/**
	 * Number of cols for tiles
	 */

	private final int cols;
	private final Image image;

	private final boolean leftToRight;

	private final int oneTilePhaseDelay;

	private final int phases;

	private final int pov;

	private final boolean rotateX;

	private final boolean rotateY;

	private final int rows;

	/**
	 * Constructor
	 * 
	 * @param appear
	 * @param imageName
	 * @param cols
	 * @param rows
	 * @param phases
	 * @param pov
	 * @param oneTilePhaseDelay
	 * @param rotateX
	 * @param rotateY
	 * @param rotateZ
	 */
	private AssimetricWindEffectData(boolean appear, boolean leftToRight,
			Image image, int cols, int rows, int phases, int pov,
			int oneTilePhaseDelay, boolean rotateX, boolean rotateY,
			float factor) {
		super();
		this.appear = appear;
		this.cols = cols;
		this.rows = rows;
		this.image = image;
		this.oneTilePhaseDelay = oneTilePhaseDelay;
		this.phases = phases;
		this.pov = pov;
		this.rotateX = rotateX;
		this.rotateY = rotateY;
		this.leftToRight = leftToRight;
		this.factor = factor;

	}

	public Effect createEffect() {

		return new AssimetricWindEffect(this);

	}

	public int getCols() {
		return cols;
	}

	public Image getImage() {
		return image;
	}

	public int getOneTilePhaseDelay() {
		return oneTilePhaseDelay;
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

	public boolean isAppear() {
		return appear;
	}

	public boolean isLeftToRight() {
		return leftToRight;
	}

	public boolean isRotateX() {
		return rotateX;
	}

	public boolean isRotateY() {
		return rotateY;
	}

	public String toString() {
		return "AssimetricEffectData [appear=" + appear + ", cols=" + cols
				+ ", image=" + image + ", oneTilePhaseDelay="
				+ oneTilePhaseDelay + ", phases=" + phases + ", pov=" + pov
				+ ", rotateX=" + rotateX + ", rotateY=" + rotateY + ", rows="
				+ rows + "]";
	}

}

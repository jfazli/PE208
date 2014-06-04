package flippingtiles;

import intf.DelayFunction;
import intf.Effect;
import intf.EffectData;

import java.util.Vector;

import util.Math3d;
import util.Point3d;
import util.Tile;
import util.Util;
import ej.microui.io.GraphicsContext;
import ej.microui.io.Image;

/**
 * Effect for transitioning between two same sized image
 * 
 * @author �kos N�meth
 * 
 */
public class FlippingTiles implements Effect {

	/**
	 * Number of cols for tiles
	 */
	private final int cols;

	/**
	 * Precalculated coordinates for drawDeformedImage
	 */
	private int[][] coordinates = null;

	/**
	 * Array of booleans, when a given boolean is false, the first image should
	 * be displayed. When the boolean is true, the second image should be used
	 */
	private boolean[] faces = null;

	private boolean finishedPhases = false;

	private final int height;

	/**
	 * Delay before a given tile starts to rotate
	 */
	private int[] phaseDelay = null;

	private final int phases;

	/**
	 * Number of rows for tiles
	 */
	private final int rows;

	/**
	 * List of tiles for side A
	 */
	Vector tiles1 = null;

	/**
	 * List of tiles for side B
	 */
	Vector tiles2 = null;

	private final int width;

	/**
	 * Constructor for creating tiles.
	 * 
	 * @param image1
	 *            image for one side of the tile
	 * @param image2
	 *            image for other side of the tile
	 * @param cols
	 *            number of columns > 0
	 * @param rows
	 *            number of rows > 0
	 */
	private FlippingTiles(Image image1, Image image2, int cols, int rows,
			int phases, int pov, DelayFunction delayFunction, boolean rotateX,
			boolean rotateY, boolean rotateZ) {

		if (image1 == null) {
			throw new IllegalArgumentException("Image1 should not be null!");
		}
		if (image2 == null) {
			throw new IllegalArgumentException("Image2 should not be null!");
		}
		if (rows < 1) {
			throw new IllegalArgumentException("Rows should be > 0!");
		}
		if (cols < 1) {
			throw new IllegalArgumentException("Cols should be > 0!");
		}
		if ((image1.getWidth() != image2.getWidth())
				| (image1.getHeight() != image2.getHeight())) {
			throw new IllegalArgumentException(
					"Image1 and Image2 dimension should be the same!");
		}
		/*
		 * if ((cols & 1) == 1 || (rows & 1) == 1 || (phases & 1) == 1) { throw
		 * new IllegalArgumentException(
		 * "rows, cols and phases should be even!"); }
		 */
		this.rows = rows;
		this.cols = cols;
		this.tiles1 = Util.getTiles(image1, cols, rows);
		this.tiles2 = Util.getTiles(image2, cols, rows);
		this.phases = phases;
		this.phaseDelay = getDelays(phases, delayFunction);
		this.width = image1.getWidth();
		this.height = image1.getHeight();

		int tileWidth = image1.getWidth() / cols;
		int tileHeight = image1.getHeight() / rows;

		calculateCoordinates(phases, tileWidth, tileHeight, pov, rotateX,
				rotateY);

	}

	public FlippingTiles(EffectData edata) {

		FlippingTilesEffectData data = (FlippingTilesEffectData) edata;

		Image image1 = null;

		image1 = data.getImage1();
		Image image2 = null;

		image2 = data.getImage2();

		if (image1 == null) {
			throw new IllegalArgumentException("Image1 should not be null!");
		}
		if (image2 == null) {
			throw new IllegalArgumentException("Image2 should not be null!");
		}
		if (data.getRows() < 1) {
			throw new IllegalArgumentException("Rows should be > 0!");
		}
		if (data.getCols() < 1) {
			throw new IllegalArgumentException("Cols should be > 0!");
		}
		if ((image1.getWidth() != image2.getWidth())
				| (image1.getHeight() != image2.getHeight())) {
			throw new IllegalArgumentException(
					"Image1 and Image2 dimension should be the same!");
		}

		this.rows = data.getRows();
		this.cols = data.getCols();
		this.tiles1 = Util.getTiles(image1, cols, rows);
		this.tiles2 = Util.getTiles(image2, cols, rows);
		this.phases = data.getPhases();
		this.phaseDelay = getDelays(phases, data.getDelayFunction());
		this.width = image1.getWidth();
		this.height = image1.getHeight();

		int tileWidth = image1.getWidth() / cols;
		int tileHeight = image1.getHeight() / rows;

		calculateCoordinates(phases, tileWidth, tileHeight, data.getPov(), data
				.isRotateX(), data.isRotateY());

	}

	/**
	 * Calculate coordinates for the animation
	 * 
	 * @param phases
	 * @param tileHeight
	 * @param tileWidth
	 * @return
	 */
	private void calculateCoordinates(int phases, int tileWidth,
			int tileHeight, int pov, boolean rotateX, boolean rotateY) {

		// allocate an array
		coordinates = new int[phases][];

		faces = new boolean[phases];

		// result array
		int[] phaseCoords = new int[8];
		Point3d[] source = Math3d.getTileCoordinates(tileWidth, tileHeight);
		// temporary array. because it is created outside of the loop, no
		// garbage collection is needed
		Point3d[] dest = Math3d.initPoint3d(source.length);
		// mirrored coordinates
		int[] mirrored = new int[8];

		for (int i = 0; i < phases; i++) {

			float xa = 0;
			if (rotateX) {
				xa = (float) (Math.PI / (phases - 1)) * i;
			}
			float ya = 0;
			if (rotateY) {
				ya = (float) (Math.PI / (phases - 1)) * i;

			}

			Math3d.get2dCoordinates(0, 0, source, dest, phaseCoords, xa, ya, 0,
					pov);

			coordinates[i] = new int[phaseCoords.length];

			if (i > phases / 2) {
				faces[i] = false;
				// mirror the coordinates
				if (rotateY & !rotateX) {
					mirrorY(phaseCoords, mirrored, i);
				}
				if (rotateX & !rotateY) {
					mirrorX(phaseCoords, mirrored, i);
				}
				if (rotateX & rotateY) {
					mirrorXY(phaseCoords, mirrored, i);
				}

			} else {
				faces[i] = true;
				// copy the calculated 2d coordinates to the array
				System.arraycopy(phaseCoords, 0, coordinates[i], 0,
						phaseCoords.length);
			}

		}

	}

	/**
	 * Free resources
	 */
	public void dispose() {
		this.coordinates = null;
		this.faces = null;
		this.phaseDelay = null;
		tiles1.removeAllElements();
		tiles1 = null;
		tiles2.removeAllElements();
		tiles2 = null;
	}

	/**
	 * See {@link Effect#draw(GraphicsContext)}
	 */
	public boolean draw(GraphicsContext gc, int x, int y, int width, int height) {

		gc.setClip(x, y, width, height);

		Vector tiles1 = this.getTiles1();
		Vector tiles2 = this.getTiles2();
		// int globalTranslateX = (320 - this.getWidth()) / 2;
		// int globalTranslateY = (240 - this.getHeight()) / 2;

		int globalTranslateX = x - 1;
		int globalTranslateY = y - 1;

		for (int i = 0; i < tiles1.size(); i++) {
			Tile tile = (Tile) tiles1.elementAt(i);

			int tilePhase = this.getPhaseDelayForTile(i);
			if (tilePhase < 0) {
				tilePhase = 0;
			}
			if (tilePhase >= this.getPhases()) {
				tilePhase = this.getPhases() - 1;
			}

			Image image;
			if (this.getFaces()[tilePhase]) {
				image = ((Tile) tiles1.elementAt(i)).getTileImage();
			} else {
				image = ((Tile) tiles2.elementAt(i)).getTileImage();
			}

			// draws the deformed image
			gc.translate(tile.getX() + globalTranslateX, tile.getY()
					+ globalTranslateY);

			int[] xys = this.getCoordinates()[tilePhase];

			gc.drawDeformedImage(image, 0, 0, xys, 0);
			gc.translate(-gc.getTranslateX(), -gc.getTranslateY());

		}

		incPhases();
		return isFinished();
	}

	public int getCols() {
		return cols;
	}

	public int[][] getCoordinates() {
		return coordinates;
	}

	private int[] getDelays(int phases2, DelayFunction delayFunction) {

		int[] result = new int[this.rows * this.cols];

		int i = 0;
		for (int r = 0; r < this.rows; r++) {
			for (int c = 0; c < this.cols; c++) {
				result[i++] = delayFunction.getDelay(phases2, this.cols,
						this.rows, c, r);
			}
		}
		return result;
	}

	public boolean[] getFaces() {
		return faces;
	}

	public int getHeight() {
		return height;
	}

	public int[] getPhaseDelay() {
		return phaseDelay;
	}

	public int getPhaseDelayForTile(int index) {

		return phaseDelay[index];
	}

	public int getPhases() {
		return phases;
	}

	public int getRows() {
		return rows;
	}

	public Vector getTiles1() {
		return tiles1;
	}

	public Vector getTiles2() {
		return tiles2;
	}

	public int getWidth() {
		return width;
	}

	public void incPhases() {

		boolean finished = true;

		int[] p = phaseDelay;

		for (int r = 0; r < p.length; r++) {

			p[r]++;
			// check if all tiles have gone through every phases
			if (p[r] < phases) {
				finished = false;

			}
		}
		finishedPhases = finished;
	}

	public boolean isFinished() {
		return finishedPhases;
	}

	private void mirrorX(int[] phaseCoords, int[] mirrored, int i) {
		mirrored[0] = phaseCoords[6];
		mirrored[1] = phaseCoords[7];
		mirrored[2] = phaseCoords[4];
		mirrored[3] = phaseCoords[5];
		mirrored[4] = phaseCoords[2];
		mirrored[5] = phaseCoords[3];
		mirrored[6] = phaseCoords[0];
		mirrored[7] = phaseCoords[1];
		// copy the calculated+mirrored 2d coordinates to the array
		System.arraycopy(mirrored, 0, coordinates[i], 0, mirrored.length);
	}

	private void mirrorXY(int[] phaseCoords, int[] mirrored, int i) {
		mirrored[0] = phaseCoords[4];
		mirrored[1] = phaseCoords[5];
		mirrored[2] = phaseCoords[6];
		mirrored[3] = phaseCoords[7];
		mirrored[4] = phaseCoords[0];
		mirrored[5] = phaseCoords[1];
		mirrored[6] = phaseCoords[2];
		mirrored[7] = phaseCoords[3];
		// copy the calculated+mirrored 2d coordinates to the array
		System.arraycopy(mirrored, 0, coordinates[i], 0, mirrored.length);
	}

	private void mirrorY(int[] phaseCoords, int[] mirrored, int i) {
		mirrored[0] = phaseCoords[2];
		mirrored[1] = phaseCoords[3];
		mirrored[2] = phaseCoords[0];
		mirrored[3] = phaseCoords[1];
		mirrored[4] = phaseCoords[6];
		mirrored[5] = phaseCoords[7];
		mirrored[6] = phaseCoords[4];
		mirrored[7] = phaseCoords[5];
		// copy the calculated+mirrored 2d coordinates to the array
		System.arraycopy(mirrored, 0, coordinates[i], 0, mirrored.length);
	}

	public boolean isClearScreen() {
		// TODO Auto-generated method stub
		return true;
	}

}

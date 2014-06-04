package assymetric;

import intf.Effect;
import intf.EffectData;

import java.util.Vector;

import util.Math3d;
import util.Point3d;
import util.Tile;
import util.Util;
import ej.microui.io.GraphicsContext;
import ej.microui.io.Image;

public class AssimetricEffect implements Effect {

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
	private int[][] phaseDelay = null;

	private final int phases;

	/**
	 * Number of rows for tiles
	 */
	private final int rows;

	/**
	 * List of tiles
	 */
	Vector tiles = null;

	private final int width;

	private final boolean appear;

	/**
	 * Constructor for creating tiles.
	 * 
	 * @param image1
	 *            image for one side of the tile
	 * @param image
	 *            image for other side of the tile
	 * @param cols
	 *            number of columns > 0
	 * @param rows
	 *            number of rows > 0
	 */
	private AssimetricEffect(boolean appear, boolean leftToRight, Image image,
			int cols, int rows, int phases, int pov, int oneTilePhaseDelay,
			boolean rotateX, boolean rotateY) {

		this.appear = appear;

		if (image == null) {
			throw new IllegalArgumentException("Image2 should not be null!");
		}
		if (rows < 1) {
			throw new IllegalArgumentException("Rows should be > 0!");
		}
		if (cols < 1) {
			throw new IllegalArgumentException("Cols should be > 0!");
		}

		/*
		 * if ((cols & 1) == 1 || (rows & 1) == 1 || (phases & 1) == 1) { throw
		 * new IllegalArgumentException(
		 * "rows, cols and phases should be even!"); }
		 */
		this.rows = rows;
		this.cols = cols;
		this.tiles = Util.getTiles(image, cols, rows);
		this.phases = phases;
		this.phaseDelay = getDelays(phases, oneTilePhaseDelay);
		this.width = image.getWidth();
		this.height = image.getHeight();

		int tileWidth = image.getWidth() / cols;
		int tileHeight = image.getHeight() / rows;

		calculateCoordinates(appear, leftToRight, phases, tileWidth,
				tileHeight, pov, rotateX, rotateY);

	}

	public String toString() {
		return "AssimetricEffect [appear="
				+ appear
				+ ", cols="
				+ cols
				+ ", coordinates="
				+ (coordinates != null ? arrayToString(coordinates,
						coordinates.length) : null)
				+ ", faces="
				+ (faces != null ? arrayToString(faces, faces.length) : null)
				+ ", finishedPhases="
				+ finishedPhases
				+ ", height="
				+ height
				+ ", phaseDelay="
				+ (phaseDelay != null ? arrayToString(phaseDelay,
						phaseDelay.length) : null) + ", phases=" + phases
				+ ", rows=" + rows + ", tiles=" + tiles + ", width=" + width
				+ "]";
	}

	private String arrayToString(Object array, int len) {
		StringBuffer buffer = new StringBuffer();
		buffer.append("[");
		for (int i = 0; i < len; i++) {
			if (i > 0)
				buffer.append(", ");
			if (array instanceof boolean[])
				buffer.append(((boolean[]) array)[i]);
			if (array instanceof Object[])
				buffer.append(((Object[]) array)[i]);
		}
		buffer.append("]");
		return buffer.toString();
	}

	public AssimetricEffect(EffectData edata) {

		AssimetricEffectData data = (AssimetricEffectData) edata;

		Image image = data.getImage();

		if (image == null) {
			throw new IllegalArgumentException("Image1 should not be null!");
		}

		if (data.getRows() < 1) {
			throw new IllegalArgumentException("Rows should be > 0!");
		}
		if (data.getCols() < 1) {
			throw new IllegalArgumentException("Cols should be > 0!");
		}
		if ((image.getWidth() != image.getWidth())
				| (image.getHeight() != image.getHeight())) {
			throw new IllegalArgumentException(
					"Image1 and Image2 dimension should be the same!");
		}
		/*
		 * if ((cols & 1) == 1 || (rows & 1) == 1 || (phases & 1) == 1) { throw
		 * new IllegalArgumentException(
		 * "rows, cols and phases should be even!"); }
		 */
		this.rows = data.getRows();
		this.cols = data.getCols();
		this.tiles = Util.getTiles(image, this.cols, this.rows);

		this.phases = data.getPhases();
		this.phaseDelay = getDelays(phases, data.getOneTilePhaseDelay());
		this.width = image.getWidth();
		this.height = image.getHeight();
		this.appear = data.isAppear();

		int tileWidth = image.getWidth() / cols;
		int tileHeight = image.getHeight() / rows;

		calculateCoordinates(appear, data.isLeftToRight(), phases, tileWidth,
				tileHeight, data.getPov(), data.isRotateX(), data.isRotateY());

	}

	/**
	 * Calculate coordinates for the animation
	 * 
	 * @param appear
	 * 
	 * @param phases
	 * @param tileHeight
	 * @param tileWidth
	 * @return
	 */
	private void calculateCoordinates(boolean appear, boolean leftToRight,
			int phases, int tileWidth, int tileHeight, int pov,
			boolean rotateX, boolean rotateY) {

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

		float startXRad = 0;
		float endXRad = 0;
		float xRadIncrement = 0;
		float startYRad = 0;
		float endYRad = 0;
		float YRadIncrement = 0;

		if (appear) {
			if (leftToRight) {
				// tiles are starting with rotation = PI
				if (rotateX) {
					startXRad = (float) Math.PI / 2;
					endXRad = 0;
				}
				if (rotateY) {
					startYRad = (float) Math.PI / 2;
					endYRad = 0;
				}
			} else {
				// right to left rotation
				if (rotateX) {
					startXRad = (float) Math.PI / 2;
					endXRad = (float) Math.PI;
				}
				if (rotateY) {
					startYRad = (float) Math.PI / 2;
					endYRad = (float) Math.PI;
				}
			}
		} else {
			if (leftToRight) {
				// tiles are starting with rotation = 0;
				if (rotateX) {
					endXRad = (float) Math.PI / 2;
					startXRad = 0;
				}
				if (rotateY) {
					endYRad = (float) Math.PI / 2;
					startYRad = 0;
				}
			} else {
				// right to left
				// tiles are starting with rotation = 0;
				if (rotateX) {
					endXRad = (float) Math.PI / 2;
					startXRad = (float) Math.PI;
				}
				if (rotateY) {
					endYRad = (float) Math.PI / 2;
					startYRad = (float) Math.PI;
				}
			}
		}

		xRadIncrement = (endXRad - startXRad) / (phases);
		YRadIncrement = (endYRad - startYRad) / (phases);
		float xa = startXRad;
		float ya = startYRad;
		for (int i = 0; i < phases; i++) {

			if (rotateX) {
				xa += xRadIncrement;
			}

			if (rotateY) {
				ya += YRadIncrement;

			}

			Math3d.get2dCoordinates(0, 0, source, dest, phaseCoords, xa, ya, 0,
					pov);

			coordinates[i] = new int[phaseCoords.length];

			if (!leftToRight) {

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
		tiles.removeAllElements();
		tiles = null;

	}

	/**
	 * See {@link Effect#draw(GraphicsContext)}
	 */
	public boolean draw(GraphicsContext gc, int x, int y, int width, int height) {

		gc.setClip(x, y, width, height);

		Vector tiles = this.getTiles();
		/*
		 * int globalTranslateX = (320 - this.getWidth()) / 2; int
		 * globalTranslateY = (240 - this.getHeight()) / 2;
		 */

		int globalTranslateX = x - 1;
		int globalTranslateY = y - 1;

		for (int i = 0; i < tiles.size(); i++) {
			Tile tile = (Tile) tiles.elementAt(i);

			int tilePhase = this.getPhaseDelayForTile(i);
			if (tilePhase < 0) {
				tilePhase = 0;
			}
			if (tilePhase >= this.getPhases()) {
				tilePhase = this.getPhases() - 1;
			}

			Image image = ((Tile) tiles.elementAt(i)).getTileImage();

			// draws the deformed image
			gc.translate(tile.getX() + globalTranslateX, tile.getY()
					+ globalTranslateY);

			int[] xys = this.getCoordinates()[tilePhase];

			gc.drawDeformedImage(image, 0, 0, xys, 0);
			gc.translate(-gc.getTranslateX(), -gc.getTranslateY());

		}
		incPhases();
		return isFinishedPhases();
	}

	public int getCols() {
		return cols;
	}

	public int[][] getCoordinates() {
		return coordinates;
	}

	private int[][] getDelays(int phases2, int oneTilePhaseDelay) {

		int[][] result = new int[cols][];
		for (int r = 0; r < cols; r++) {
			result[r] = new int[rows];
			for (int c = 0; c < rows; c++) {
				result[r][c] = -(c + r) * oneTilePhaseDelay;

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

	public int[][] getPhaseDelay() {
		return phaseDelay;
	}

	public int getPhaseDelayForTile(int index) {
		int row = index / cols;
		int col = index - (row * cols);
		return phaseDelay[col][row];
	}

	public int getPhases() {
		return phases;
	}

	public int getRows() {
		return rows;
	}

	public Vector getTiles() {
		return tiles;
	}

	public int getWidth() {
		return width;
	}

	public void incPhases() {

		boolean finished = true;

		int[][] p = phaseDelay;

		for (int r = 0; r < p.length; r++) {
			for (int c = 0; c < p[r].length; c++) {
				p[r][c]++;
				// check if all tiles have gone through every phases
				if (p[r][c] < phases) {
					finished = false;
				}
			}
		}
		finishedPhases = finished;
	}

	public boolean isFinishedPhases() {
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

	public boolean isFinished() {
		return finishedPhases;
	}

	public boolean isClearScreen() {
		// TODO Auto-generated method stub
		return true;
	}

}

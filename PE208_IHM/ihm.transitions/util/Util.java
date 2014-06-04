package util;

import java.util.Vector;

import ej.microui.io.Image;

public class Util {

	public static Vector getTiles(Image image1, int cols, int rows) {

		Vector v = new Vector(cols * rows);

		int x = 0;
		int y = 0;
		int x1 = 0;
		int y1 = 0;
		int width = 0;
		int height = 0;

		int xDivisions[] = Util.divide(image1.getWidth(), cols);
		int yDivisions[] = Util.divide(image1.getHeight(), rows);
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {

				x = xDivisions[c];
				x1 = xDivisions[c + 1];
				y = yDivisions[r];
				y1 = yDivisions[r + 1];
				width = x1 - x;
				height = y1 - y;

				Image i = Image.createImage(image1, x, y, width, height);
				// Images.addStatistics(i);

				Tile t = new Tile(i, (x1 + x) / 2, (y1 + y) / 2);

				v.addElement(t);

			}

		}

		return v;
	}

	/**
	 * 
	 * @param image1
	 *            the image to slice up
	 * @param cols
	 *            number of columns
	 * @param rows
	 *            number of rows
	 * @param factor
	 * @param appear
	 *            if true, the tiles will be starts to move from an offscreen
	 *            position to their end positions. if appear is false, the tiles
	 *            are starting from their normal position and starts to move to
	 *            an offscreen position
	 * @return
	 */
	public static Vector getTilesForWindEffect(Image image1, int cols,
			int rows, boolean appear, int phases, float factor) {

		Vector v = new Vector(cols * rows);

		int x = 0;
		int y = 0;
		int x1 = 0;
		int y1 = 0;
		int width = 0;
		int height = 0;

		int xDivisions[] = Util.divide(image1.getWidth(), cols);
		int yDivisions[] = Util.divide(image1.getHeight(), rows);
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {

				x = xDivisions[c];
				x1 = xDivisions[c + 1];
				y = yDivisions[r];
				y1 = yDivisions[r + 1];
				width = x1 - x;
				height = y1 - y;

				Image i = Image.createImage(image1, x, y, width, height);
				// Images.addStatistics(i);

				int tileX = (x1 + x) / 2;
				int tileY = (y1 + y) / 2;
				Tile t = new Tile(i, tileX, tileY);

				float ox = (float) ((float) image1.getWidth() / 2 + 0.1);
				float oy = (float) ((float) image1.getHeight() / 2 + 0.1);

				float ix = (tileX - ox);
				float iy = (tileY - oy);

				float rx = (ix * factor);
				float ry = (iy * factor);

				int steps = phases;
				t.setMovingSteps(steps);
				if (appear) {
					// tiles are coming from offscreen location
					t.setStartX((int) rx);
					t.setStartY((int) ry);
					t.setEndX(tileX);
					t.setEndY(tileY);
					float dx = (tileX - rx) / phases;
					float dy = (tileY - ry) / phases;
					t.setDx(dx);
					t.setDy(dy);
					t.setAx(rx);
					t.setAy(ry);
					t.setX((int) rx);
					t.setY((int) ry);

				} else {

					// tiles are going to offscreen location
					t.setStartX(tileX);
					t.setStartY(tileY);
					t.setEndX((int) rx);
					t.setEndY((int) ry);
					float dx = (rx - tileX) / phases;
					float dy = (ry - tileY) / phases;
					t.setDx(dx);
					t.setDy(dy);
					t.setAx(t.getX());
					t.setAy(t.getY());
				}

				v.addElement(t);

			}

		}

		return v;
	}

	/**
	 * Returns the distance between (x,y)-(x1,y1)
	 * 
	 * @param x
	 * @param y
	 * @param x1
	 * @param y1
	 * @return
	 */
	private static float distance(float x, float y) {

		return (float) Math.sqrt(x * x + y * y);
	}

	/**
	 * Divide the length by divisons. The result is an array containing the
	 * interval starting from 0 to length. The result array will have
	 * divisions+1 elements.
	 * 
	 * @param length
	 *            length of interval
	 * @param divisions
	 *            number of divisions
	 * @return divisions + 1 length int array, containing the divisions (e.g.
	 *         for length = 4, divisions = 2: [0, 2, 4])
	 */
	public static int[] divide(int length, int divisions) {

		int[] result = new int[divisions + 1];
		for (int i = 0; i < divisions + 1; i++) {
			result[i] = (int) ((length * (i)) / ((float) divisions) + 0.5);

		}
		return result;
	}

}

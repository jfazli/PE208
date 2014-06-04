package util;

import ej.microui.io.Image;

public class Tile {

	/**
	 * Actual x,y position in floating point to not accumulate errors
	 */
	private float ax;
	private float ay;
	/**
	 * movement vector (end-start)/movementsteps
	 */
	private float dx;

	public float getAx() {
		return ax;
	}

	public void setAx(float ax) {
		this.ax = ax;
	}

	public float getAy() {
		return ay;
	}

	public void setAy(float ay) {
		this.ay = ay;
	}

	private float dy;

	// end position when wind effect is used
	private int endX;

	private int endY;

	/**
	 * Steps of movement when wind effect is used. = phasedelay + phasenum
	 */
	private int movingSteps;

	// start position for wind effect
	private int startX;

	// start position for wind effect
	private int startY;
	/**
	 * dynamic image
	 */
	private Image tileImage;

	/**
	 * Center of the tile when not distorted current position when wind effect
	 * is used
	 */
	private int x;

	private int y;

	public Tile(Image tileImage, int x, int y) {
		super();
		this.tileImage = tileImage;
		this.x = x;
		this.y = y;
	}

	public float getDx() {
		return dx;
	}

	public float getDy() {
		return dy;
	}

	public int getEndX() {
		return endX;
	}

	public int getEndY() {
		return endY;
	}

	public int getMovingSteps() {
		return movingSteps;
	}

	public int getStartX() {
		return startX;
	}

	public int getStartY() {
		return startY;
	}

	public Image getTileImage() {
		return tileImage;
	}

	public int getX() {
		return x;
	}

	public int getY() {
		return y;
	}

	public void setDx(float dx) {
		this.dx = dx;
	}

	public void setDy(float dy) {
		this.dy = dy;
	}

	public void setEndX(int endX) {
		this.endX = endX;
	}

	public void setEndY(int endY) {
		this.endY = endY;
	}

	public void setMovingSteps(int movingSteps) {
		this.movingSteps = movingSteps;
	}

	public void setStartX(int startX) {
		this.startX = startX;
	}

	public void setStartY(int startY) {
		this.startY = startY;
	}

	public void setTileImage(Image tileImage) {
		this.tileImage = tileImage;
	}

	public void setX(int x) {
		this.x = x;
	}

	public void setY(int y) {
		this.y = y;
	}

	public String toString() {
		return "Tile [x=" + x + ", y=" + y + "(" + tileImage.getWidth() + "x"
				+ tileImage.getHeight() + ")]";
	}

}

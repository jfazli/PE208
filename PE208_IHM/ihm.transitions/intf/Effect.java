package intf;

import ej.microui.io.GraphicsContext;

public interface Effect {

	/**
	 * Draws the effect to the GraphicsContext, returns true if the effect is
	 * finished. After the call of draw(), the effect phase is increased, so the
	 * next call will display the next phase.
	 * 
	 * @param gc
	 *            GraphicsContext
	 * 
	 * @param x
	 *            top left x coordinate of clipping rectangle. the effect is
	 *            aligned to this coordinate
	 * @param y
	 *            top left y coordinate of clipping rectangle. the effect is
	 *            aligned to this coordinate
	 * @param width
	 *            width of clipping rectangle
	 * @param height
	 *            height of clipping rectangle
	 * 
	 * 
	 * @return false, if the effect is not yet finished. returns true, if the
	 *         effect is finished (reached it's end phase)
	 */
	public boolean draw(GraphicsContext gc, int x, int y, int width, int height);

	/**
	 * Returns true if the effect is finished
	 * 
	 * @return true if the effect is finished
	 */
	public boolean isFinished();

	/**
	 * Returns true if the screen should be cleared before the draw() function.
	 * 
	 * @return
	 */
	public boolean isClearScreen();
}

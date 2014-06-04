package intf;

public interface DelayFunction {
	/**
	 * The methood should return an int value, which will be used as a "delay"
	 * of the phase of the given tile
	 * 
	 * @param phases
	 *            number of all animation phases for the tile
	 * @param cols
	 *            number of all columns for the tiles
	 * @param rows
	 *            number of all rows for the tiles
	 * @param aCol
	 *            actual column
	 * @param aRow
	 *            actual row
	 * @return the phase delay
	 */
	public int getDelay(int phases, int cols, int rows, int aCol, int aRow);
}

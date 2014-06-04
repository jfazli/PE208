package delay;

import intf.DelayFunction;

public class LeftUp implements DelayFunction {

	private final int overlap;

	public int getDelay(int phases, int cols, int rows, int aCol, int aRow) {

		return -(cols - 1 - aCol + rows - 1 - aRow) * phases * overlap / 100;
	}

	public final static DelayFunction INSTANCE = new LeftUp(5);

	/**
	 * Constructor
	 * 
	 * @param overlap
	 */
	public LeftUp(int overlap) {
		super();
		this.overlap = overlap;
	}

	public static LeftUp getInstance(int overlap) {
		return new LeftUp(overlap);
	}

}

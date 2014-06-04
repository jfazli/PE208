package delay;

import intf.DelayFunction;

public class LeftDown implements DelayFunction {

	private final int overlap;

	public int getDelay(int phases, int cols, int rows, int aCol, int aRow) {

		return -(cols - 1 - aCol + aRow) * phases * overlap / 100;
	}

	public final static DelayFunction INSTANCE = new LeftDown(5);

	/**
	 * Constructor
	 * 
	 * @param overlap
	 */
	public LeftDown(int overlap) {
		super();
		this.overlap = overlap;
	}

	public static LeftDown getInstance(int overlap) {
		return new LeftDown(overlap);
	}

}

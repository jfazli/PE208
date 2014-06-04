package delay;

import intf.DelayFunction;

public class Left implements DelayFunction {

	private final int overlap;

	public int getDelay(int phases, int cols, int rows, int aCol, int aRow) {

		return -(cols - 1 - aCol) * phases * overlap / 100;
	}

	public final static DelayFunction INSTANCE = new Left(50);

	/**
	 * Constructor
	 * 
	 * @param overlap
	 */
	public Left(int overlap) {
		super();
		this.overlap = overlap;
	}

	public static Left getInstance(int overlap) {
		return new Left(overlap);
	}

}

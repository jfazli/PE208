package delay;

import intf.DelayFunction;

public class RightUp implements DelayFunction {

	private final int overlap;

	public int getDelay(int phases, int cols, int rows, int aCol, int aRow) {

		return -(aCol + rows - 1 - aRow) * phases * overlap / 100;
	}

	public final static DelayFunction INSTANCE = new RightUp(5);

	/**
	 * Constructor
	 * 
	 * @param overlap
	 */
	public RightUp(int overlap) {
		super();
		this.overlap = overlap;
	}

	public static RightUp getInstance(int overlap) {
		return new RightUp(overlap);
	}

}

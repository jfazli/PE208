package delay;

import intf.DelayFunction;

public class RightDown implements DelayFunction {

	private final int overlap;

	public int getDelay(int phases, int cols, int rows, int aCol, int aRow) {

		return -(aCol + aRow) * phases * overlap / 100;
	}

	public final static DelayFunction INSTANCE = new RightDown(5);

	/**
	 * Constructor
	 * 
	 * @param overlap
	 */
	public RightDown(int overlap) {
		super();
		this.overlap = overlap;
	}

	public static RightDown getInstance(int overlap) {
		return new RightDown(overlap);
	}

}

package delay;

import intf.DelayFunction;

public class Right implements DelayFunction {

	private final int overlap;

	public int getDelay(int phases, int cols, int rows, int aCol, int aRow) {

		return -aCol * phases * overlap / 100;
	}

	public final static DelayFunction INSTANCE = new Right(50);

	/**
	 * Constructor
	 * 
	 * @param overlap
	 */
	public Right(int overlap) {
		super();
		this.overlap = overlap;
	}

	public static Right getInstance(int overlap) {
		return new Right(overlap);
	}

}

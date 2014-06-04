package delay;

import intf.DelayFunction;

public class Down implements DelayFunction {

	private final int overlap;

	public int getDelay(int phases, int cols, int rows, int aCol, int aRow) {

		return -aCol * phases * overlap / 100;
	}

	public final static DelayFunction INSTANCE = new Down(50);

	/**
	 * Constructor
	 * 
	 * @param overlap
	 */
	public Down(int overlap) {
		super();
		this.overlap = overlap;
	}

	public static Down getInstance(int overlap) {
		return new Down(overlap);
	}

}

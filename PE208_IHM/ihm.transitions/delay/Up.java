package delay;

import intf.DelayFunction;

public class Up implements DelayFunction {

	private final int overlap;

	public int getDelay(int phases, int cols, int rows, int aCol, int aRow) {

		return -aCol * phases * overlap / 100;
	}

	public final static DelayFunction INSTANCE = new Up(50);

	/**
	 * Constructor
	 * 
	 * @param overlap
	 */
	public Up(int overlap) {
		super();
		this.overlap = overlap;
	}

	public static Up getInstance(int overlap) {
		return new Up(overlap);
	}

}

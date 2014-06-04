package delay;

import intf.DelayFunction;

public class Center implements DelayFunction {

	private final int overlap;

	public int getDelay(int phases, int cols, int rows, int aCol, int aRow) {

		return -(distance(cols / 2, rows / 2, aCol, aRow)) * phases * overlap
				/ 100;
	}

	private int distance(int x, int y, int x1, int y1) {
		int xd = (x1 - x);
		int yd = (y1 - y);
		return (int) Math.sqrt(xd * xd + yd * yd);
	}

	public final static DelayFunction INSTANCE = new Center(10);

	/**
	 * Constructor
	 * 
	 * @param overlap
	 */
	public Center(int overlap) {
		super();
		this.overlap = overlap;
	}

	public static Center getInstance(int overlap) {
		return new Center(overlap);
	}

}

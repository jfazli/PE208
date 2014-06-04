package delay;

import intf.DelayFunction;

/**
 * Most simple implementation: always returns zero
 * 
 * @author Acer
 * 
 */
public class NoDelay implements DelayFunction {

	public int getDelay(int phases, int cols, int rows, int aCol, int aRow) {

		return 0;
	}

	public static final DelayFunction INSTANCE = new NoDelay();

}

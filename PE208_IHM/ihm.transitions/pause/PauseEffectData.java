package pause;

import intf.Effect;
import intf.EffectData;

public class PauseEffectData implements EffectData {

	public static PauseEffectData getEffectData(int delay) {
		return new PauseEffectData(delay);
	}

	private int delay;

	public PauseEffectData(int delay) {
		this.delay = delay;
	}

	public String toString() {
		return "PauseEffectData [delay=" + delay + "]";
	}

	public int getDelay() {
		return delay;
	}

	public void setDelay(int delay) {
		this.delay = delay;
	}

	public Effect createEffect() {

		return new Pause(this);
	}

}

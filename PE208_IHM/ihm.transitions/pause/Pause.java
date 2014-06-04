package pause;

import intf.Effect;
import intf.EffectData;
import ej.microui.io.GraphicsContext;

public class Pause implements Effect {

	private final int delay;
	private boolean finished = false;

	public boolean draw(GraphicsContext gc, int x, int y, int width, int height) {

		// gc.setClip(x, y, width, height);

		try {

			Thread.sleep(delay);
			finished = true;
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return true;
	}

	public String toString() {
		return "Pause [delay=" + delay + "]";
	}

	public Pause(EffectData data) {

		this.delay = ((PauseEffectData) data).getDelay();

	}

	public boolean isFinished() {

		return finished;
	}

	public boolean isClearScreen() {
		// TODO Auto-generated method stub
		return false;
	}

}

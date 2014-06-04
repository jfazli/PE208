package Utilities;

import ej.bon.Timer;
import ej.bon.TimerTask;

// Animation pour JFA page
public class Animation extends TimerTask {

	private/* final */int duration;
	private final int interval;
	private int count;
	private final Timer timer;
	private final Animator target;

	public Animation(Animator target, Timer timer, int duration, int interval) {
		this.target = target;
		this.duration = duration;
		this.interval = interval;
		this.timer = timer;
	}

	public void start() {
		this.count = 0;
		this.target.begin();
		this.timer.scheduleAtFixedRate(this, 0, this.interval);
	}

	public void stop() {
		cancel();
		this.target.end();
	}

	public void run() {
		this.count++;
		this.duration -= this.interval;

		if (this.duration <= 0) { // animation done
			this.target.update();
			stop();
			// start();
			return;
		}

		this.target.update();
	}

	public boolean isRunning() {
		return this.duration > 0;
	}

	public int getElapsedTime() {
		return this.count * this.interval;
	}

	public int getStep() {
		return this.count;
	}

	public int getTotalTime() {
		return this.duration;
	}

}

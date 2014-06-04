package util;

import intf.DelayFunction;
import intf.Effect;
import intf.EffectData;

import java.util.Random;

import settings.Settings;
import assymetric.AssimetricEffectData;
import assymetricwind.AssimetricWindEffectData;
import delay.Center;
import delay.Down;
import delay.Left;
import delay.LeftDown;
import delay.LeftUp;
import delay.Right;
import delay.RightDown;
import delay.RightUp;
import delay.Up;
import ej.microui.io.Display;
import ej.microui.io.ExplicitFlush;
import ej.microui.io.Image;
import flippingtiles.FlippingTilesEffectData;

/**
 * Plays the effects
 */
public class Player {

	public static final int NONE = 0;
	public static final int SLIDE = 1;
	public static final int CUBE = 2;
	public static final int ANIMATION_TYPE = CUBE;

	public static ExplicitFlush getGc() {
		if (gc == null) {
			gc = Display.getDefaultDisplay().getNewExplicitFlush();
		}
		return gc;
	}

	/**
	 * Returns a random element from the array
	 * 
	 * @param array
	 * @return
	 */
	private static DelayFunction getRandom(DelayFunction[] array) {
		int index = rnd.nextInt(array.length);
		return array[index];
	}

	/**
	 * Returns a random element from the array
	 * 
	 * @param array
	 * @return
	 */
	private static int getRandom(int[] array) {
		int index = rnd.nextInt(array.length);
		return array[index];
	}

	/**
	 * @param A
	 * @param B
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 * @param bgColor
	 */
	public static void playEffect(Image A, Image B, int x, int y, int w, int h,
			int bgColor, boolean leftToRight) {
		/*
		 * switch (ANIMATION_TYPE) { case NONE: // do nothing break; case SLIDE:
		 * playSlideEffect(A, B, x, y, w, h, bgColor, leftToRight); break; case
		 * CUBE:
		 */
		playCubeEffect(A, B, x, y, w, h, bgColor);
		/*
		 * break; }
		 */
	}

	public static void playSlideEffect(Image A, Image B, int x, int y, int w,
			int h, int bgColor, boolean leftToRight) {
		ExplicitFlush g = getGc();
		final int stepBase = 40;
		int step;
		int currentXShift;
		int bImageShift;
		if (leftToRight) {
			step = stepBase;
			currentXShift = 0;
			bImageShift = w;
		} else {
			step = -stepBase;
			currentXShift = 0;
			bImageShift = -w;
		}
		while (true) {
			g.drawImage(A, x + currentXShift, y, 0);
			g.drawImage(B, x - bImageShift + currentXShift, y, 0);
			g.flush();
			currentXShift += step;
			if (leftToRight ? currentXShift > w : currentXShift < -w) {
				break;
			} else {
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
				}
			}
		}
	}

	public static void playCubeEffect(Image A, Image B, int x, int y, int w,
			int h, int bgColor) {

		boolean leftToRight = false;
		if (rnd.nextInt(2) == 1) {
			leftToRight = true;
		}

		boolean rotX = false;
		boolean rotY = false;

		if (Settings.ENABLE_TWO_AXIS_ROTATION) {
			if (rnd.nextInt(2) == 1) {
				rotX = true;
			}

			if (rnd.nextInt(2) == 1) {
				rotY = true;
			}
		} else {
			if (rnd.nextInt(2) == 1) {
				rotX = true;
			} else {
				rotY = true;
			}
		}

		int phases = 15;
		int pov = 300;

		boolean symmetrical = false;
		if (rnd.nextInt(2) == 1) {
			symmetrical = true;
		}

		int rows = getRandom(rowVariations);
		int cols = getRandom(colVariations);

		DelayFunction delay = getRandom(delayVariations);

		if (symmetrical) {

			// flipping tiles

			EffectData data = FlippingTilesEffectData.getEffectData(
			// current content
					A,
					// next content
					B,
					// number of rows
					cols,
					// number of columns
					rows,
					// phases
					phases,
					// POV
					pov,
					// Delay function
					delay,
					// x rotation
					rotX,
					// y rotation
					rotY);

			playEffect(x, y, w, h, bgColor, data);
			data = null;

		} else {
			// Assymetric effect

			int oneTitlePhaseDelay = 1;

			boolean startWind = false;
			boolean endWind = false;

			if (rnd.nextInt(2) == 1) {
				startWind = true;
			}
			if (rnd.nextInt(2) == 1) {
				endWind = true;
			}

			float factor = rnd.nextFloat() * 5 + 5;

			EffectData data1 = null;
			EffectData data2 = null;

			if (!startWind) {
				data1 = playAssimetricEffect(A, false, leftToRight, x, y, w, h,
						bgColor, cols, rows, phases, pov, oneTitlePhaseDelay,
						rotX, rotY);

			} else {

				data1 = playAssimetricWindEffect(A, false, leftToRight, x, y,
						w, h, bgColor, cols, rows, phases, pov,
						oneTitlePhaseDelay, rotX, rotY, factor);
			}

			// end effect can use different settings
			rows = getRandom(rowVariations);
			cols = getRandom(colVariations);
			delay = getRandom(delayVariations);
			factor = rnd.nextFloat() * 5 + 5;
			rotX = false;
			rotY = false;

			if (Settings.ENABLE_TWO_AXIS_ROTATION) {
				if (rnd.nextInt(2) == 1) {
					rotX = true;
				}

				if (rnd.nextInt(2) == 1) {
					rotY = true;
				}
			} else {
				if (rnd.nextInt(2) == 1) {
					rotX = true;
				} else {
					rotY = true;
				}
			}
			if (!endWind) {

				data2 = playAssimetricEffect(B, true, !leftToRight, x, y, w, h,
						bgColor, cols, rows, phases, pov, oneTitlePhaseDelay,
						rotX, rotY);
			} else {

				data2 = playAssimetricWindEffect(B, true, !leftToRight, x, y,
						w, h, bgColor, cols, rows, phases, pov,
						oneTitlePhaseDelay, rotX, rotY, factor);
			}

			playEffect(x, y, w, h, bgColor, data1);
			playEffect(x, y, w, h, bgColor, data2);

		}

		getGc().drawImage(B, x, y, 0);
		getGc().flush();

	}

	private static EffectData playAssimetricWindEffect(Image image,
			boolean appear, boolean leftToRight, int x, int y, int w, int h,
			int bgColor, int cols, int rows, int phases, int pov,
			int oneTitlePhaseDelay, boolean rotX, boolean rotY, float factor) {

		EffectData data = AssimetricWindEffectData.getEffectData(appear,
				leftToRight, image, cols, rows, phases, pov,
				oneTitlePhaseDelay, rotX, rotY, factor);

		/*
		 * playEffect(x, y, w, h, bgColor, data);
		 * 
		 * data = null;
		 */
		return data;
	}

	private static EffectData playAssimetricEffect(Image image, boolean appear,
			boolean leftToRight, int x, int y, int w, int h, int bgColor,
			int cols, int rows, int phases, int pov, int oneTitlePhaseDelay,
			boolean rotX, boolean rotY) {

		EffectData data = AssimetricEffectData.getEffectData(appear,
				leftToRight, image, cols, rows, phases, pov,
				oneTitlePhaseDelay, rotX, rotY);

		/*
		 * playEffect(x, y, w, h, bgColor, data);
		 * 
		 * data = null;
		 */

		return data;

	}

	private static void playEffect(int x, int y, int w, int h, int bgColor,
			EffectData effectData) {

		Effect effect = effectData.createEffect();

		while (!effect.isFinished()) {

			if (effect.isClearScreen()) {

				/*
				 * Image bg = Images.loadPNG(Images.DEMO_CONTENT_PATH);
				 * getGc().drawImage(bg, x, y, 0);
				 */
				getGc().setColor(
						/* 0xECF0F1 */Theme.Skin.SCREEN_BACKGROUND_COLOR);
				getGc().fillRect(x, y, w, h);

			}

			effect.draw(getGc(), x, y, w, h);

			getGc().flush();

			if (GLOBAL_DELAY > 0) {
				try {
					Thread.sleep(GLOBAL_DELAY);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}

		effect = null;
		effectData = null;
	}

	private static int[] colVariations = new int[] { 1, 2, 4, 8 };
	// private static int[] colVariations = new int[] {8};

	private static DelayFunction[] delayVariations = new DelayFunction[] {
			Center.INSTANCE, Down.INSTANCE, Left.INSTANCE, LeftDown.INSTANCE,
			LeftUp.INSTANCE, Right.INSTANCE, RightDown.INSTANCE,
			RightUp.INSTANCE, Up.INSTANCE };

	private static ExplicitFlush gc = null;
	private static final int GLOBAL_DELAY = 0;
	private static final Random rnd = new Random(System.currentTimeMillis());

	private static int[] rowVariations = new int[] { 1, 2, 3, 5, 6, 9 };
	// private static int[] rowVariations = new int[] { 9 };

}

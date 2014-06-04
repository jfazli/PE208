package Utilities;

import ej.bon.Timer;
import ej.microui.io.GraphicsContext;

/**
 * Various utilities
 */
public class Utilities {

	private static Timer timer;

	public static Timer getTimer() {
		if (timer == null) {
			timer = new Timer();
		}
		return timer;
	}

	public static void beep() {
		System.out.println("playing a beep");
	}

	public static String timeFormatter(int mn) {
		int hours = mn / 60;
		int minutes = mn % 60;
		StringBuffer sb = new StringBuffer();
		if (0 == hours) {
			sb.append("0h");
		} else {
			sb.append(hours).append("h");
		}
		if (minutes <= 9) {
			sb.append("0");
		}
		sb.append(minutes).append("m");

		return sb.toString();
	}

	public static void drawLine(GraphicsContext g, int x1, int y1, int x2,
			int y2, int width) {
		if (width == 0) {
			return;
		} else if (width == 1) {
			g.drawLine(x1, y1, x2, y2);
		} else {
			int halfWidth = width >> 1;
			double coef = (double) Math.abs(x1 - x2)
					/ (double) Math.abs(y1 - y2);

			if (coef > 1) {
				int offsetX = 0;
				int offsetY = -halfWidth;

				for (int i = width; --i >= 0;) {
					g.drawLine(x1 + offsetX, y1 + offsetY, x2 + offsetX, y2
							+ offsetY);

					offsetX += 0;
					offsetY += 1;
				}
			} else {
				int offsetX = -halfWidth;
				int offsetY = 0;

				for (int i = width; --i >= 0;) {
					g.drawLine(x1 + offsetX, y1 + offsetY, x2 + offsetX, y2
							+ offsetY);

					offsetX += 1;
					offsetY += 0;
				}
			}
		}
	}

}

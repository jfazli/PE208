package util;

import java.util.Vector;

import ej.microui.io.Display;
import ej.microui.io.Displayable;
import ej.microui.io.GraphicsContext;
import ej.microui.io.Image;
import ej.mwt.Composite;
import ej.mwt.Desktop;
import ej.mwt.Panel;
import ej.mwt.Renderable;
import ej.mwt.Widget;
import ej.mwt.rendering.Renderer;
import ej.mwt.rendering.WidgetRenderer;

/**
 * Utility class for Debug and Rendering Widgets in bitmaps for fast effects
 */
public class ImageRenderer {

	/**
	 * Prints all panels hierarchy for debugging
	 */
	public static void printPanels() {
		Displayable d = Display.getDefaultDisplay().getDisplayable();
		if (d == null) {
			return;
		}

		// Log.trace("displayble class:" + d.getClass());
		if (d instanceof Desktop) {
			Desktop desktop = (Desktop) d;
			Panel[] panels = desktop.getPanels();

			for (int i = 0; i < panels.length; i++) {

				Panel p = panels[i];

				printPanel(p);
			}
		}
	}

	/**
	 * Prints the hierarchy of the given panel
	 * 
	 * @param p
	 *            the panel
	 */
	public static void printPanel(Panel p) {
		Widget w = p.getWidget();
		if (w != null) {
			// Log.trace("Panel: " + p);
			printTree(w);
		}
	}

	private static void printTree(Widget widget, int level) {
		if (widget == null) {
			return;
		}

		for (int i = 0; i < level; i++) {
			System.out.print("\t");
		}
		// Log.trace("" + widget);
		// Log.trace(" x:" + widget.getX() + ",y:" + widget.getY() + ",w:"
		// + widget.getWidth() + ",h:" + widget.getHeight()
		// + ", visible: " + widget.isVisible());
		if (widget instanceof Composite) {
			Composite c = (Composite) widget;
			if (c.getWidgetsCount() > 0) {
				for (int j = 0; j < c.getWidgetsCount(); j++) {
					Widget w = null;
					try {
						w = c.getWidgets()[j];
					} catch (Throwable t) {
						// Log.trace("widget #" + j + " throwed an exception: "
						// + t + ", " + t.getMessage());
					}
					printTree(w, level + 1);
				}
			}
		}
	}

	private static void drawTree(GraphicsContext gc, Widget widget, int level) {
		Renderer r = widget.getRenderer();

		if (r == null) {
		} else {

			r.render(gc, widget);

		}

		if (widget instanceof Composite) {
			Composite c = (Composite) widget;
			Widget[] widgetsArray = c.getWidgets();

			if (widgetsArray.length > 0) {

				// draw children
				for (int j = 0; j < widgetsArray.length; j++) {

					Widget w = widgetsArray[j];

					// set translation
					gc.translate(w.getX(), w.getY());
					gc.setClip(0, 0, w.getWidth(), w.getHeight());

					drawTree(gc, w, level + 1);

					// restore translation
					gc.translate(-w.getX(), -w.getY());

				}
			}
		}
	}

	/**
	 * Prints the data and hierarchy of the widget
	 * 
	 * @param widget
	 *            the widget to display
	 */
	public static void printTree(Widget widget) {
		Vector v = new Vector();
		// Log.trace("Parents:" + printParents(widget, v));
		printTree(widget, 0);

	}

	public static void drawTree(GraphicsContext gc, Widget widget) {
		gc.translate(widget.getX(), widget.getY());
		gc.setClip(0, 0, widget.getWidth(), widget.getHeight());

		drawTree(gc, widget, 0);

	}

	private static Vector printParents(Widget widget, Vector parents) {
		if (widget.getParent() != null) {
			widget = widget.getParent();
			parents.insertElementAt("" + widget + ",x:" + widget.getX() + ",y:"
					+ widget.getY(), 0);
			printParents(widget, parents);
		} else {
			// parents.insertElementAt("No more parent", 0);
		}
		return parents;
	}

	/**
	 * Returns the Rendered image of the given renderable. Only widgets are
	 * rendered.
	 * 
	 * @param widget
	 *            the widget to render
	 * @return the widget image
	 */
	public static Image getWidgetImage(Renderable renderable) {

		// long start = System.currentTimeMillis();
		try {
			if (renderable instanceof Widget) {
				// Widget widget = (Widget) renderable;

				int w = renderable.getWidth();
				int h = renderable.getHeight();
				// Log.trace("getWidgetImage() width:" + w + ",height:" + h );
				if (w == 0 || h == 0) {
					// Log.trace("widget:" + widget + ", w:" + w + ", h:" + h);
					return null;
				}

				Image i = Image.createImage(w, h);
				// Images.addStatistics(i);

				GraphicsContext gc = i.getGraphicsContext();

				drawTree(gc, (Widget) renderable, 0);

				return i;
			} else {
				// Log.trace("Not Widget: " + renderable);
			}
			return null;

		} finally {
			// long end = System.currentTimeMillis();
			// Log.trace("ImageRenderer rendering time: " + renderable + " ("
			// + (end - start) + "ms)");
		}
	}

	/**
	 * Renders the widget image into an Image
	 * 
	 * @param widget
	 *            the widget to render
	 * @param renderer
	 *            renderer to use
	 * @return the widget's image
	 */
	public static Image getWidgetImage(Widget widget, WidgetRenderer renderer) {
		// long start = System.currentTimeMillis();
		try {

			int w = widget.getWidth();
			int h = widget.getHeight();

			if (w == 0 || h == 0) {
				// Log.trace("getWidgetImage() width:" + w + ",height:" + h
				// + ", widget:" + widget);
				return null;
			}

			Image i = Image.createImage(w, h);
			// Images.addStatistics(i);

			GraphicsContext gc = i.getGraphicsContext();

			renderer.render(gc, widget);

			return i;
		} finally {
			// long end = System.currentTimeMillis();
			// Log.trace("Rendering time for: " + widget + " by " + renderer
			// + " (" + (end - start) + "ms)");
		}
	}

}

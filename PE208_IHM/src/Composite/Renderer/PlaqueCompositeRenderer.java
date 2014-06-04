package Composite.Renderer;

import java.io.IOException;

import Composite.PlaqueComposite;
import Page.MainMenu;
import Resource.Resources;
import Theme.Skin;
import Widget.WirelessAnimation;
import ej.microui.io.GraphicsContext;
import ej.microui.io.Image;
import ej.mwt.Renderable;
import ej.mwt.Widget;
import ej.mwt.rendering.WidgetRenderer;

/**
 * Renderer for OneBarComposite. It just draws the background image.
 * 
 * @author Ch Saunier
 * 
 */

public class PlaqueCompositeRenderer extends WidgetRenderer {

	private static final boolean DEBUG = false;
	private final int POSITION_WIDTH_PLAQUE=30; 

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * ej.mwt.rendering.WidgetRenderer#getPreferredContentWidth(ej.mwt.Widget)
	 */
	public int getPreferredContentWidth(Widget widget) {
		String path = ((PlaqueComposite) widget).getBackground();
		Image image;
		int imageWidth = 0;

		try {
			image = Image.createImage(path, Image.PNG);
			imageWidth = image.getWidth();
		} catch (IOException e) {
			if (DEBUG) {
				// Sonar does not like stack trace printing
				e.printStackTrace(); // NOSONAR
			}
		}

		return imageWidth;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * ej.mwt.rendering.WidgetRenderer#getPreferredContentHeight(ej.mwt.Widget)
	 */
	public int getPreferredContentHeight(Widget widget) {
		String path = ((PlaqueComposite) widget).getBackground();
		Image image;
		int imageHeight = 0;

		try {
			image = Image.createImage(path, Image.PNG);
			imageHeight = image.getHeight();
		} catch (IOException e) {
			if (DEBUG) {
				// Sonar does not like stack trace printing
				e.printStackTrace(); // NOSONAR
			}
		}

		return imageHeight;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see ej.mwt.rendering.Renderer#getManagedType()
	 */
	public Class getManagedType() {
		return PlaqueComposite.class;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see ej.mwt.rendering.Renderer#render(ej.microui.io.GraphicsContext,
	 * ej.mwt.Renderable)
	 */
	public void render(GraphicsContext g, Renderable renderable) {
		String path = ((PlaqueComposite) renderable).getBackground();
		Image image;


		try {
			image = Image.createImage(path, Image.PNG);
			g.setColor(Skin.getColor(Skin.SCREEN_BACKGROUND_COLOR));
			g.fillRect(0, 0, Resources.SIZE_WIDTH_DISPLAY,
					Resources.SIZE_HEIGHT_DISPLAY);

			g.drawImage(image, POSITION_WIDTH_PLAQUE, 0, GraphicsContext.TOP
							| GraphicsContext.LEFT);
		} catch (IOException e) {
			if (DEBUG) {
				// Sonar does not like stack trace printing
				e.printStackTrace(); // NOSONAR
			}
		}

	}

}

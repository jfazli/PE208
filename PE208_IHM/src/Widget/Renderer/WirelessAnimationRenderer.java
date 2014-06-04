package Widget.Renderer;

import Widget.WirelessAnimation;
import ej.microui.io.GraphicsContext;
import ej.microui.io.Image;
import ej.mwt.Renderable;
import ej.mwt.Widget;
import ej.mwt.rendering.WidgetRenderer;

public class WirelessAnimationRenderer extends WidgetRenderer {

	private static int MARGIN = 0;

	public int getMargin() {
		return MARGIN;
	}

	public int getPreferredContentHeight(Widget widget) {
		WirelessAnimation wirelessAnimation = (WirelessAnimation) widget;
		return wirelessAnimation.getImageOn().getHeight(); // getImageOn() et
		// getImageOff sont
		// similaire
	}

	public int getPreferredContentWidth(Widget widget) {
		WirelessAnimation wirelessAnimation = (WirelessAnimation) widget;
		return wirelessAnimation.getImageOn().getWidth();
	}

	public Class getManagedType() {
		return WirelessAnimation.class;
	}

	public void render(GraphicsContext g, Renderable renderable) {
		WirelessAnimation wirelessAnimation = (WirelessAnimation) renderable;
		Image image = null;
		Image imageOn = wirelessAnimation.getImageOn();
		Image imageOff = wirelessAnimation.getImageOff();
		int getHeight = wirelessAnimation.getImageOn().getHeight();
		int getWidth = wirelessAnimation.getImageOn().getWidth();
		if (wirelessAnimation.getStateImageOn() == 1) {
			image = imageOn;
			g.drawImage(image, 0, 0, GraphicsContext.TOP| GraphicsContext.LEFT);
		} else {
			image = imageOff;// imageOff;// imageOff;
			g.drawRect(0, 0, getWidth, getHeight);
		}

	}

}

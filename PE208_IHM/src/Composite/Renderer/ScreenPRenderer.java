package Composite.Renderer;

import Main;
import Composite.ScreenP;
import ej.microui.Colors;
import ej.microui.io.GraphicsContext;
import ej.mwt.Renderable;
import ej.mwt.Widget;
import ej.mwt.rendering.WidgetRenderer;

public class ScreenPRenderer extends WidgetRenderer {

	public int getPreferredContentHeight(Widget widget) {
		// TODO Auto-generated method stub
		return 0;
	}

	public int getPreferredContentWidth(Widget widget) {
		// TODO Auto-generated method stub
		return 0;
	}

	public Class getManagedType() {
		// TODO Auto-generated method stub
		return ScreenP.class;
	}

	public void render(GraphicsContext g, Renderable renderable) {
		//
		g.setColor(Colors.BLACK);
		g.fillRect(0, 0, Main.desktop.getHeight(), 240);

	}

}

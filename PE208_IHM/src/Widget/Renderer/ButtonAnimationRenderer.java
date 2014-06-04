package Widget.Renderer;

import java.io.IOException;

import Resource.Resources;
import Widget.ButtonAnimation;
import ej.microui.io.GraphicsContext;
import ej.microui.io.Image;
import ej.mwt.Renderable;
import ej.mwt.Widget;
import ej.mwt.rendering.WidgetRenderer;

public class ButtonAnimationRenderer extends WidgetRenderer {
	
	

	public int getPreferredContentWidth(Widget widget) {
		String path = Resources.START_BUTTON;
		Image image;
		int imageWidth = 0;

		try {
			image = Image.createImage(path, Image.PNG);
			imageWidth = image.getWidth();
		} catch (IOException e) {
			e.printStackTrace(); 		
		}

		return imageWidth;
	}

	public int getPreferredContentHeight(Widget widget) {
		String path = Resources.START_BUTTON;	//les 3 images sont similaires
		Image image;
		int imageHeight = 0;

		try {
			image = Image.createImage(path, Image.PNG);
			imageHeight = image.getHeight();
		} catch (IOException e) {
			e.printStackTrace(); 
		}

		return imageHeight;
	}

	public Class getManagedType() {
		// TODO Auto-generated method stub
		return ButtonAnimation.class;
	}

	public void render(GraphicsContext g, Renderable renderable) {
		ButtonAnimation button = (ButtonAnimation) renderable;
		Image image;
		
		try {
			image = Image.createImage(button.getPath(), Image.PNG);
			g.drawImage(image, 0, 0, GraphicsContext.TOP| GraphicsContext.LEFT);
		} catch (IOException e) {
			e.printStackTrace(); 
		}
	}

	
}

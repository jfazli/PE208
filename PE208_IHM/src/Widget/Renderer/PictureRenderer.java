package Widget.Renderer;

import Page.MainMenu;
import Resource.Resources;
import Utilities.Animation;
import Widget.Picture;
import ej.microui.Colors;
import ej.microui.io.GraphicsContext;
import ej.microui.io.Image;
import ej.mwt.Renderable;
import ej.mwt.Widget;
import ej.mwt.rendering.WidgetRenderer;

//Renderer CS
public class PictureRenderer extends WidgetRenderer {

	private static int MARGIN = 0;

	public Class getManagedType() {
		return Picture.class;
	}

	public int getMargin() {
		return MARGIN;
	}

	public int getPreferredContentHeight(Widget widget) {
		Picture picture = (Picture) widget;
		return picture.getImage().getHeight();
	}

	public int getPreferredContentWidth(Widget widget) {
		Picture picture = (Picture) widget;
		return picture.getImage().getWidth();
	}

	public void render(GraphicsContext g, Renderable renderable) {
		Picture picture = (Picture) renderable;
		Image image = picture.getImage();

		int x = 0;
		int y = 0;
		int[] xys = new int[8];

		int iWidth = image.getWidth();
		int iHeight = image.getHeight();

		int WidthResized = iWidth;
		int HeightResized = iHeight;

		if (picture.getTypeAnimation() == Picture.ANIMATION_ZOMM_DEZOOM) {
			Animation animation = picture.getAnimation();
			if (animation != null && animation.isRunning()) {
				if ((animation.getStep() <= 10)) {
					WidthResized = ((iWidth - 1) / 20)
							* (animation.getStep() * 2);
					HeightResized = ((iHeight - 1) / 20)
							* (animation.getStep() * 2);
				} else if ((animation.getStep() <= 20)) {
					WidthResized = ((iWidth - 1) / 20)
							* (20 - ((animation.getStep() * 2) - 20));
					HeightResized = ((iHeight - 1) / 20)
							* (20 - ((animation.getStep() * 2) - 20));
				} else {
				}
			} else {
			}

			xys[0] = 0;
			xys[1] = 0;
			xys[2] = WidthResized;
			xys[3] = 0;
			xys[4] = WidthResized;
			xys[5] = HeightResized;
			xys[6] = 0;
			xys[7] = HeightResized;

			/* Position d'affichage de l'image */
			x = Resources.SIZE_WIDTH_DISPLAY / 2;
			y = Resources.SIZE_HEIGHT_DISPLAY / 2;
		} else if (picture.getTypeAnimation() == Picture.ANIMATION_ROTATE) {
			Animation animation = picture.getAnimation();
			if (animation != null && animation.isRunning()) {

				if ((animation.getStep() <= 10)) {
					WidthResized = ((iWidth - 1) / 20)
							* (animation.getStep() * 2);
					HeightResized = ((iHeight - 1) / 20)
							* (animation.getStep() * 2);

					xys[0] = 0;
					xys[1] = 0;
					xys[2] = iWidth - WidthResized;
					xys[3] = 0;
					xys[4] = iWidth - WidthResized;
					xys[5] = iHeight;
					xys[6] = 0;
					xys[7] = iHeight;
				} else {
					WidthResized = ((iWidth - 1) / 20)
							* (20 - ((animation.getStep() * 2) - 20));
					HeightResized = ((iHeight - 1) / 20)
							* (20 - ((animation.getStep() * 2) - 20));

					xys[0] = 0;
					xys[1] = 0;
					xys[2] = iWidth - WidthResized;
					xys[3] = 0;
					xys[4] = iWidth - WidthResized;
					xys[5] = iHeight;
					xys[6] = 0;
					xys[7] = iHeight;
				}
			} else {
			}

			/* Position d'affichage de l'image */
			x = Resources.SIZE_WIDTH_DISPLAY / 2;
			y = Resources.SIZE_HEIGHT_DISPLAY / 4;

		} else {
		}

		// image
		if (image != null) {
			/*
			 * int[] xys = new int[]{ 0,0, WidthResized,0,
			 * WidthResized,HeightResized, 0,HeightResized };
			 */
			g.setColor(Colors.WHITE);
			g.fillRect(0, 0, Resources.SIZE_WIDTH_DISPLAY,
					Resources.SIZE_HEIGHT_DISPLAY);
			g.drawDeformedImage(image, x, y, xys, GraphicsContext.HCENTER
					| GraphicsContext.VCENTER);
		}

	}

}

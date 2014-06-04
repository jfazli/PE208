package Widget.Renderer;

import nativeIhm.NativeData;
import Page.MainMenu;
import Resource.Resources;
import Utilities.Animation;
import Widget.PoeleAnimation;
import ej.microui.Colors;
import ej.microui.io.GraphicsContext;
import ej.microui.io.Image;
import ej.mwt.Renderable;
import ej.mwt.Widget;
import ej.mwt.rendering.WidgetRenderer;

//Renderer CS
public class PoeleAnimationRenderer extends WidgetRenderer {

	private static int MARGIN = 0;

	public Class getManagedType() {
		return PoeleAnimation.class;
	}

	public int getMargin() {
		return MARGIN;
	}

	public int getPreferredContentHeight(Widget widget) {
		PoeleAnimation poeleAnimation = (PoeleAnimation) widget;
		return poeleAnimation.getImage().getHeight();
	}

	public int getPreferredContentWidth(Widget widget) {
		PoeleAnimation poeleAnimation = (PoeleAnimation) widget;
		return poeleAnimation.getImage().getWidth();
	}

	public void render(GraphicsContext g, Renderable renderable) {
		PoeleAnimation poeleAnimation = (PoeleAnimation) renderable;
		Image image = poeleAnimation.getImage();

		int x = 0;
		int y = 0;
		int x_delete = 0;

		int iWidth = image.getWidth();
		int iHeight = image.getHeight();

		int WidthResized = iWidth;
		int HeightResized = iHeight;
		
		int ty=poeleAnimation.getChoixAnimation();
		
		if(poeleAnimation.getTypeAnimation()  == NativeData.MODE_FONCTIONNEMENT ){

			if (poeleAnimation.getChoixFoyer() == NativeData.CHOIX_FOYER1) {
				Animation animation = poeleAnimation.getAnimation();
				if (animation != null && animation.isRunning()) {
					/* Position d'affichage de l'image */
					x = Resources.SIZE_WIDTH_DISPLAY / 6 + 27;
					y = 3*(Resources.SIZE_HEIGHT_DISPLAY- (Resources.SIZE_HEIGHT_DISPLAY / 30)* animation.getStep());
	
					x_delete = 20 ;
				} else {
					x = Resources.SIZE_WIDTH_DISPLAY / 6 + 27;
					y = Resources.SIZE_HEIGHT_DISPLAY / 3 + 43;
	
					 x_delete = 20;
				}
			} else if (poeleAnimation.getChoixFoyer() == NativeData.CHOIX_FOYER2) {
				Animation animation = poeleAnimation.getAnimation();
				if (animation != null && animation.isRunning()) {
					/* Position d'affichage de l'image */
					x = 4 * Resources.SIZE_WIDTH_DISPLAY / 7 + 57;
					y = 3*(Resources.SIZE_HEIGHT_DISPLAY- (Resources.SIZE_HEIGHT_DISPLAY / 30)* animation.getStep());
	
					x_delete = 270 ;
				} else {
					x = 4 * Resources.SIZE_WIDTH_DISPLAY / 7 + 57;
					y = Resources.SIZE_HEIGHT_DISPLAY / 3 + 43;
	
					 x_delete = 270;
				}
			} else {
			}

			// image
			if (image != null && x != 0 && y != 0) {
				// g.setColor(Skin.getColor(Skin.SCREEN_BACKGROUND_COLOR));
			
				 if (x_delete != 0)
				 g.fillRect(x_delete, y / 3, iWidth, iHeight+Resources.SIZE_HEIGHT_DISPLAY / 30);
			
				g.drawImage(image, x, y, GraphicsContext.HCENTER | GraphicsContext.VCENTER);
			
			}
		}
		else if(poeleAnimation.getTypeAnimation()  == NativeData.MODE_SCRUTATION){
			if (poeleAnimation.getChoixFoyer() == NativeData.CHOIX_FOYER1) {
				x = Resources.SIZE_WIDTH_DISPLAY / 6 +8;
				y = Resources.SIZE_HEIGHT_DISPLAY / 6;
				g.setColor(Colors.RED);
				g.fillCircle(x, y, 80);
			}
			else if (poeleAnimation.getChoixFoyer() == NativeData.CHOIX_FOYER2) {
				x = 4 * Resources.SIZE_WIDTH_DISPLAY / 6 -9;
				y = Resources.SIZE_HEIGHT_DISPLAY / 6 ;
				g.setColor(Colors.RED);
				g.fillCircle(x, y, 80);
			}
			

		}
	}

}

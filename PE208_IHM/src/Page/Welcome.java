package Page;

import java.io.IOException;

import util.ImageRenderer;
import Theme.Skin;
import Widget.NavigationElement;
import Widget.Picture;

import com.is2t.mwt.composites.BorderComposite;

import ej.microui.Listener;
import ej.microui.io.Image;
import ej.mwt.MWT;
import ej.mwt.Widget;

public class Welcome extends BorderComposite implements NavigationElement {

	private static final int IMG_WELCOME = 0;
	private static final int IMG_COUNT = 1 + IMG_WELCOME;

	private static Image images[] = new Image[IMG_COUNT];

	// static {
	// try {
	// images[IMG_WELCOME] = Image.createImage("/Image/TouchAndCook.png",
	// Image.PNG);
	// } catch (IOException e) {
	// e.printStackTrace();
	// }
	// }

	public Welcome() {

		try {
			images[IMG_WELCOME] = Image.createImage("/Image/TouchAndCook.png",
					Image.PNG);
		} catch (IOException e) {
			e.printStackTrace();
		}

		// KRUPS
		Picture p = new Picture(images[IMG_WELCOME],
				Picture.ANIMATION_ZOMM_DEZOOM, Skin
						.getColor(Skin.MAIN_MENU_BUTTON_COLOR), Skin
						.getColor(Skin.MAIN_MENU_LEGACY_FOCUSED_COLOR));
		addAt(p, MWT.CENTER);
		p.setSelectionListener(new Listener() {
			public void performAction() {
			}

			public void performAction(int value) {
			}

			public void performAction(int value, java.lang.Object o) {
				GestionPage.Instance.SetStateOfPage(GestionPage.APPLI_PAGE, 0);
			}
		});
	}

	public Widget getContent() {
		System.out.println(this);
		return this;
	}

	public Image getContentImage() {
		return ImageRenderer.getWidgetImage(this);
	}

	public String getTitle() {
		return null;
	}

	public Image getImageTitle() {
		return null;
	}

}

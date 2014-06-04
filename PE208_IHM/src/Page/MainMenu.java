package Page;

import java.io.IOException;

import nativeIhm.NativeData;
import nativeIhm.ScrutationInfos;
import util.ImageRenderer;
import Composite.PlaqueComposite;
import Resource.Resources;
import Theme.Skin;
import Utilities.Utilities;
import Widget.ButtonAnimation;
import Widget.Label;
import Widget.NavigationElement;
import Widget.PoeleAnimation;
import Widget.PuissancePlaqueLabel;
import Widget.TemperatureLabel;
import Widget.WirelessAnimation;

import com.is2t.mwt.composites.BorderComposite;
import com.is2t.mwt.composites.GridComposite;
import com.jfa.observer.Observer;

import ej.bon.Timer;
import ej.microui.Listener;
import ej.microui.io.Image;
import ej.mwt.MWT;
import ej.mwt.Widget;

/**
 * This class shows a grid of 8 items giving access to list of recipes.
 * 
 * It is build using a GridComposite to inherit the widget positioning and event
 * handling.
 */
// Page principale CS
public class MainMenu extends BorderComposite implements NavigationElement {

	private static final int IMG_PLAQUE = 0;
	private static final int IMG_POELE = IMG_PLAQUE + 1;
	private static final int IMG_WIRELESS_ON = IMG_POELE + 1;
	private static final int IMG_COUNT = 1 + IMG_WIRELESS_ON;

	private static Image images[] = new Image[IMG_COUNT];

	private ScrutationInfos scrutationInfos;
	private TemperatureLabel temperature;
	private PuissancePlaqueLabel puissance;
	private PuissancePlaqueLabel puissance2;
	private PoeleAnimation poele;
	private WirelessAnimation wireless;
	private ButtonAnimation button;
	
	public MainMenu() {
		try {
			// images[IMG_PLAQUE] =
			// Image.createImage("/Image/Plaque_Induction.jpeg", Image.JPEG);
			images[IMG_PLAQUE] = Image.createImage(Resources.PLAQUE_INDUCTION,Image.JPEG);
			images[IMG_POELE] = Image.createImage(Resources.POELE, Image.PNG);
			images[IMG_WIRELESS_ON] = Image.createImage(Resources.WIRELESS_ON,Image.JPEG);
		} catch (IOException e) {
			System.out.println("marche pas");
			e.printStackTrace();
		}

		/********************/

		GridComposite lowerBar = new GridComposite(3);
		/*créer un composite pour la barre du bas*/
		
		puissance = new PuissancePlaqueLabel("Puissance: ", 0, "%",NativeData.CHOIX_FOYER1);
		puissance.setListener(puissance);
		lowerBar.add(puissance);

		temperature = new TemperatureLabel("T°Poele: ", 0, "°C");
		temperature.setListener(temperature);
		lowerBar.add(temperature);

		puissance2 = new PuissancePlaqueLabel("Puissance: ", 0, "%",NativeData.CHOIX_FOYER2);
		puissance2.setListener(puissance2);
		lowerBar.add(puissance2);
		
		/**********************/
		poele = new PoeleAnimation(images[IMG_POELE], 0, Skin.getColor(Skin.MAIN_MENU_BUTTON_COLOR), Skin.getColor(Skin.MAIN_MENU_LEGACY_FOCUSED_COLOR));
		poele.setListener(poele);
//		poele.setVisible(false);

		wireless = new WirelessAnimation(images[IMG_WIRELESS_ON]); /*affichage icone*/
		wireless.setListener(wireless);
		
		button = new ButtonAnimation();
		button.setListener(button);
		/**********************/
		
		

		PlaqueComposite plaqueComposite = new PlaqueComposite(poele,button, lowerBar,wireless, Resources.PLAQUE_INDUCTION);
		addAt(plaqueComposite, MWT.CENTER);

		this.scrutationInfos = new ScrutationInfos();
		
		this.scrutationInfos.addObserver(new Observer(){

			public void update(NativeData nd) {
				temperature.setData(nd.getTemperaturePoele(),nd);
				
				puissance.setData(nd.getPuissanceFoyer(), nd);
				puissance2.setData(nd.getPuissanceFoyer(), nd);
				poele.setData(nd.getSelectionFoyer(), nd);
				wireless.setData(nd.getCommunicationUart(), nd);
				button.setData(nd.getAutorisationAppli(), nd);
			}
			
		});
		new Thread(scrutationInfos).start();
	}

	// NavigationElement interface

	public Widget getContent() {
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

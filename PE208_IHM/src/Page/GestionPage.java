package Page;

import Composite.ScreenP;
import Widget.NavigationElement;
import ej.microui.Listener;
import ej.mwt.Widget;

public class GestionPage implements Listener {

	public static final int STATE_NONE = -1;
	public static final int MAIN_MENU_PAGE = 0;
	public static final int APPLI_PAGE = MAIN_MENU_PAGE+1;

	private final ScreenP screen;
	private int states; // stocker les pages
	public static GestionPage Instance = new GestionPage();// Singleton

	private int initialState = STATE_NONE;
	public Widget w = null;

	public GestionPage() {
		this.screen = new ScreenP();
	}

	// Initialisation au démarrage à la page MaiMenu page
	public void SetInitialiseState(int state) {
		this.initialState = state;
		SetStateOfPage(state, 0);

	}

	public void SetScreen(NavigationElement element, int way) {
		// recuperation de tous les widgets de la page
		w = element.getContent();

		screen.setWidget(w);
	}

	public void SetStateOfPage(int state, int way) { // Definition de la
		// position de la page en fonction des évenements
		this.states = state;
		// Chaque page est interfacé avec la classe "NavigationElement"
		NavigationElement gP = null;

		switch (state) {
		case MAIN_MENU_PAGE:
			gP = new Welcome();
			break;
		case APPLI_PAGE:
			gP = new MainMenu();
			break;

		}
		// SetScreen(gP, 0);
		w = gP.getContent();
		screen.setWidget(w);
	}

	public Widget getWidget() {
		// TODO Auto-generated method stub

		return screen;
	}

	public void performAction() {
		// TODO Auto-generated method stub

	}

	public void performAction(int value) {
		// TODO Auto-generated method stub

	}

	public void performAction(int value, Object object) {
		// TODO Auto-generated method stub
		SetStateOfPage(this.states, value);
	}

}

import Page.GestionPage;
import Theme.ClassicTheme;
import ej.microui.MicroUI;
import ej.microui.io.Display;
import ej.microui.io.Image;
import ej.mwt.Desktop;
import ej.mwt.MWT;
import ej.mwt.Panel;

//import Widget.Label;
//import Widget.InductionHob;

public class Main {

	public static final int DEBUG = 0; /*
										 * DEBUG = 0 ==> On fonctionne sur
										 * l'embarqué
										 */
	private static final int IMG_INDUCTION_HOB1 = 0;
	private static final int IMG_COUNT = IMG_INDUCTION_HOB1 + 1;
	private static Image images[] = new Image[IMG_COUNT];
	public static Panel panel;
	public static Desktop desktop;

	public static void main(String[] args) {
		MicroUI.errorLog(true);
		MWT.RenderingContext.add(new ClassicTheme());
		desktop = new Desktop(Display.getDefaultDisplay());
		panel = new Panel(0, 0, desktop.getWidth(), desktop.getHeight());
		// démarrage à la page initiale.
		GestionPage.Instance.SetInitialiseState(0); // Initialisation au
		panel.setWidget(GestionPage.Instance.getWidget());

		panel.show(desktop);
		// shows the Desktop
		desktop.show();
	}
}

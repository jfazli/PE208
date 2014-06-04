package Composite;

import ej.microui.Event;
import ej.microui.io.Pointer;
import ej.mwt.Composite;
import ej.mwt.Widget;

public class ScreenP extends Composite {

	private Widget w;

	public ScreenP() {

	}

	public boolean setWidget(Widget w) {
		removeAllWidgets();
		this.w = w;
		add(w); // récuperer tous les widget de la page que l'on veut afficher
		// dans le composite actuel
		revalidate();
		// Main.panel.setWidget(w);
		return true;
	}

	public boolean handleEvent(int event) {
		if (event == Event.POINTER) {
			int action = Pointer.getAction(event);
			if (action == Pointer.RELEASED) {
				Pointer pointer = (Pointer) Event.getGenerator(event);
				// retrieve coordinates of pointer relative to this widget
				// parent (Composite or Panel)
				int x = pointer.getX() - getAbsoluteX() + getX();
				int y = pointer.getY() - getAbsoluteY() + getY();
				System.out.println("Screen handel event");
				return true;
			}
		}
		// returned value doesn't matter, pointer events are not propagated by
		// system to hierarchies
		// fall down
		return false;

	}

	public void validate(int widthHint, int heightHint) {

		w.validate(widthHint, heightHint);
		w.setBounds(0, 0, widthHint, heightHint);
		setPreferredSize(widthHint, heightHint);

	}
}

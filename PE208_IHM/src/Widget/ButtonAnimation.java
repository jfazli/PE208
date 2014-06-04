package Widget;

import nativeIhm.NativeData;
import nativeIhm.PcomNative;
import Resource.Resources;
import ej.microui.Command;
import ej.microui.Event;
import ej.microui.Listener;
import ej.microui.io.Pointer;
import ej.mwt.Widget;

/**
 * 	Si l'appli n'a pas démarrer et que l'on pas reçu la trame qui nous dit de 
 * démarrer, le bouton est en wait, on ne notify pas l'appli
 * 	Si l'appli à reçu une trame pour démarrer, le bouton qui s'affiche est le bouton Start mais on est en etat 
 * stop
 * 	Si l'appli à recu une trame pour démarrer et que l'on appuie sur le bouton Start, on affiche le bouton stop,
 * mais on est en état Start
 */

public class ButtonAnimation extends InductionModel  {

	private int state;
	private String path= Resources.START_GRISE_BUTTON;
	

	public ButtonAnimation(){
		super();
	}
	
	public String getPath() {
		return path;
	}


	public void setPath(String path) {
		this.path = path;
	}


	

	public int getState() {
		return state;
	}

	public void setState(int state) {
		this.state = state;
	}
	
	public void _click(){
		if(getState() == NativeData.STATE_APPLI_START){
			setState(NativeData.STATE_APPLI_STOP);
		}
		else if(getState() == NativeData.STATE_APPLI_STOP){
			setState(NativeData.STATE_APPLI_START);
		}
		super.notifyListener(getState());
		
	}
	
	
	
	public boolean handleEvent(int event) {
		int type = Event.getType(event);

		if (	(Event.getType(event) == Event.COMMAND)
			&&	(Event.getData(event) == Command.SELECT)) {
			_click();
			return true;
		} else if (type == Event.POINTER) {
			int action = Pointer.getAction(event);
			if (action == Pointer.RELEASED) {
				Pointer pointer = (Pointer) Event.getGenerator(event);
				// retrieve coordinates of pointer relative to this widget parent (Composite or Panel)
				int x = pointer.getX() - getAbsoluteX() + getX();
				int y = pointer.getY() - getAbsoluteY() + getY();
				if (contains(x, y)) {
					_click();
					return true;
				}
			}
			// returned value doesn't matter, pointer events are not propagated by system to hierarchies
			// fall down
		}

		return false;
	}
	
	

	public void performAction() {
		// TODO Auto-generated method stub
		System.out.println("not implemented0");
	}

	public void performAction(int value) {
		// TODO Auto-generated method stub
		System.out.println("not implemented1");
	}

	public void performAction(int value, Object object) {
		// TODO Auto-generated method stub
		if( value == 1){
			if(getState() == NativeData.STATE_APPLI_WAIT){
				setState(NativeData.STATE_APPLI_STOP);
			}
			
			if(getState() == NativeData.STATE_APPLI_START ){
				setPath(Resources.STOP_BUTTON);
			}
			else if(getState() == NativeData.STATE_APPLI_STOP){
				setPath(Resources.START_BUTTON);
			}
		}else{
			setState(NativeData.STATE_APPLI_WAIT);
			setPath(Resources.START_GRISE_BUTTON);
		}
		NativeData.stateAppli= getState();
	}
	
	
}

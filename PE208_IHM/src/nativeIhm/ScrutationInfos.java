package nativeIhm;

//import Main;
import java.util.Vector;

import com.jfa.observer.Observable;
import com.jfa.observer.Observer;
import ej.bon.Immortals;
import ej.bon.Timer;
import ej.bon.TimerTask;
import ej.microui.Listener;

public class ScrutationInfos implements Observable,Runnable {

	private final int temperature[]= new int[1];
	private final char trameUart[] = new char[1];
	private final char memoPuissance[]=new char[1];
	private final char selectMode[]=new char[1];
	private final char choixFoyer[]=new char[1];	
	private final char startAppli[]=new char[1];	
	private final int stateAppli[]=new int[1];
	
	private Vector listObserver = new Vector();
	private NativeData nd = new NativeData();
	private Observer obs;
	

	public ScrutationInfos() {
		Immortals.setImmortal(temperature);
		Immortals.setImmortal(trameUart);
		Immortals.setImmortal(memoPuissance);
		Immortals.setImmortal(selectMode);
		Immortals.setImmortal(choixFoyer);
		Immortals.setImmortal(startAppli);
		Immortals.setImmortal(stateAppli);
		PcomNative.init();
	}

	public void run() {		
		while(true){
			PcomNative.DataReaded(temperature,trameUart, memoPuissance,selectMode,choixFoyer,startAppli);
			
			nd.setTemperaturePoele(temperature[0]);
			nd.setCommunicationUart((int) trameUart[0]);
			nd.setPuissanceFoyer((int)memoPuissance[0]);
			nd.setSelectionMode((int)selectMode[0]);
			nd.setSelectionFoyer((int)choixFoyer[0]);
			nd.setAutorisationAppli((int)startAppli[0]);

			stateAppli[0] = NativeData.stateAppli;
			
			
			this.updateObserver();

			PcomNative.stateButton(stateAppli[0]);
			try {
				Thread.sleep(100);//ms
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}

	public void addObserver(Observer obs) {
		// TODO Auto-generated method stub
		this.listObserver.addElement(obs);
	}

	public void updateObserver() {
		for(int i =0; i< this.listObserver.size();i++){
			obs=(Observer) this.listObserver.elementAt(i);
			obs.update(nd);
		}
	}

	public void delObserver() {
		// TODO Auto-generated method stub
		this.listObserver = new Vector();
	}

}

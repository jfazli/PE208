package nativeIhm;

public class NativeData {
	
	public static final int MODE_INIT=0;
	public static final int MODE_SCRUTATION=MODE_INIT+1;
	public static final int MODE_FONCTIONNEMENT=MODE_SCRUTATION+1;
	
	public static final int CHOIX_FOYER1=0;
	public static final int CHOIX_FOYER2=CHOIX_FOYER1+1;
	
	
	public static final int STATE_APPLI_WAIT=0;
	public static final int STATE_APPLI_START=STATE_APPLI_WAIT+1;
	public static final int STATE_APPLI_STOP=STATE_APPLI_START+1;

	public static int stateAppli; //aprés appuie sur le bouton pour démarrer

	
	
	private int temperaturePoele;
	private int selectionMode;
	private int puissanceFoyer;
	private int communicationUart;
	private int selectionFoyer;
	private int autorisationAppli;
	
	public int getPuissanceFoyer() {
		return puissanceFoyer;
	}
	
	public int getAutorisationAppli() {
		return autorisationAppli;
	}

	public int getTemperaturePoele() {
		return temperaturePoele;
	}

	public void setTemperaturePoele(int temperaturePoele) {
		this.temperaturePoele = temperaturePoele;
	}

	public int getSelectionMode() {
		return selectionMode;
	}

	public void setSelectionMode(int selectionMode) {
		this.selectionMode = selectionMode;
	}

	public int getCommunicationUart() {
		return communicationUart;
	}

	public void setCommunicationUart(int communicationUart) {
		this.communicationUart = communicationUart;
	}

	public int getSelectionFoyer() {
		return selectionFoyer;
	}

	public void setSelectionFoyer(int selectionFoyer) {
		this.selectionFoyer = selectionFoyer;
	}

	public void setPuissanceFoyer(int puissanceFoyer) {
		this.puissanceFoyer = puissanceFoyer;
	}

	public void setAutorisationAppli(int autorisationAppli) {
		this.autorisationAppli = autorisationAppli;
	}


	
	
}

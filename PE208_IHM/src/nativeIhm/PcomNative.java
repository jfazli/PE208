package nativeIhm;

public class PcomNative {

	/*
	 * Initialisation du protocole et declaration des equipements. Cette
	 * fonction doit être appele au demarrage
	 */
	public static native void init();
	public static native void stateButton( int state);
	public static native void DataReaded(int pTemperature[],char pReceptionTrameUart[], char pMemorisationPuissance[],char pSelectMode[],char pChoixFoyer[],char pStartAppli[]);

}

package nativeIhm;

import com.is2t.hil.HIL;
import com.is2t.hil.NativeInterface;

public class PcomNative {
//	private static SimNativeIhm sim;


	// private final byte[] tabDataReceived = new byte[10];

	public PcomNative() {
	}

	/*
	 * Initialisation du protocole et declaration des equipements. Cette
	 * fonction doit être appele au demarrage
	 */
	public static void init(){
		System.out.println("Création thread natif");
		SimNativeIhm sim = new SimNativeIhm();
	}
	public static void stateButton(int state){
		NativeInterface ni = HIL.getInstance();
		ni.refreshContent(state);
		SimNativeIhm.stateAppli= state; 
	}

	public static void DataReaded(int pTemperature[],char pReceptionTrameUart[], char pMemorisationPuissance[],char pSelectMode[],char pChoixFoyer[],char pStartAppli[]){
		NativeInterface ni = HIL.getInstance();
		pTemperature[0]=(char) SimNativeIhm.Temperature;
//		pTemperature2[0]=(char) SimNativeIhm.Temperature;
		pReceptionTrameUart[0]=(char) SimNativeIhm.ReceptionTrameUart;
		pMemorisationPuissance[0]=(char) SimNativeIhm.MemorisationPuissance;
		pSelectMode[0]=(char) SimNativeIhm.SelectionMode;
		pChoixFoyer[0]=(char) SimNativeIhm.choixFoyer;
		pStartAppli[0]=(char) SimNativeIhm.startAppli;
		ni.flushContent(pTemperature,0,1);
//		ni.flushContent(pTemperature2,0,1);
		ni.flushContent(pReceptionTrameUart,0,1);
		ni.flushContent(pMemorisationPuissance,0,1);
		ni.flushContent(pSelectMode,0,1);
		ni.flushContent(pChoixFoyer,0,1);
		ni.flushContent(pStartAppli,0,1);
	}

}

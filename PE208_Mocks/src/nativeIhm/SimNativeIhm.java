package nativeIhm;

import java.io.InputStream;

import javax.security.sasl.SaslClient;

//import com.is2t.examples.ecom.uart.AbstractCommTest;

//import ej.ecom.io.CommConnection;



public class SimNativeIhm implements Runnable {

	private static final int MODE_INIT=0;
	private static final int MODE_SCRUTATION=1;
	private static final int MODE_FCT=2;
	public static int Temperature;
	public static int SelectionMode;
	public static int ReceptionTrameUart;
	public static int MemorisationPuissance;
	public static int startAppli;
	
	public static int choixFoyer=0;
	private static int changeMode=0;
	private static int countTimeMode=0;
	private static int countTimeScrutation=0;
	private static final int TIME_Interrupt			= 100;	//100ms
	public static final String CONNECTION_STRING 	= "comm:com51;baudrate=9600;bitsperchar=8;stopbits=1;parity=none";
//	protected static CommConnection connection;	// current comm connection
	protected static InputStream in;				// current comm connection's input stream
	private static long time;
	public static int stateAppli; //appuie sur le bouton

	/*Constructor*/
	public SimNativeIhm(){
		launch(SimNativeIhm.CONNECTION_STRING);
	}
	
	private void launch(String connectionString) {
		
		// 1- Try to open the comm connection
//		connection = openConnection(connectionString);
		 time = System.currentTimeMillis();		// get the current time
//		if (connection == null) 
//			return;
//		
		 
//		// 2- Try to open the output stream from the comm connection
//		in = openInputStream(connection);
//		if (in == null) {
//			// we should close the connection now.
//			closeConnection(connection);
//			return;
//		}
		
		new Thread(this).start();
	}
	
	
	public void setModeDebug(int Mode)
	{
		if(Mode == MODE_INIT){
			SelectionMode=0;
			ReceptionTrameUart=0;	
			choixFoyer=0;
			Temperature=5;
			startAppli=1;
		}
		else if(Mode == MODE_SCRUTATION){

			SelectionMode=1;
			ReceptionTrameUart=1;
		}
		else if(Mode == MODE_FCT){
			choixFoyer=1;
			SelectionMode=2;
			Temperature+=10;
			if(Temperature >= 250){
				Temperature=0;
			}
//			MemorisationPuissance+=1;
//			if(MemorisationPuissance > 10 )
//			{
//				MemorisationPuissance=0;
//			}
		}
			
	}
		

	@Override
	public void run() {
		for(;;){
			
			if(System.currentTimeMillis() - time >= TIME_Interrupt){
				countTimeMode++;
				if(countTimeMode >= 100){ //toute les secs
					if(stateAppli == 1)
						changeMode++;
					else 
						changeMode=MODE_INIT;
					if(changeMode > MODE_FCT)
						changeMode = MODE_INIT;
					countTimeMode=0;
					setModeDebug(changeMode);	
					System.out.println(" loop Thread nativeIhm"+changeMode);			
				
				}
				if(stateAppli == 0)changeMode=MODE_INIT;
				
				if(changeMode == MODE_SCRUTATION){
					countTimeScrutation++;
					ReceptionTrameUart=1;
					if(countTimeScrutation >= 10){
						
						if(choixFoyer==0){
							choixFoyer=1;
						}
						else if(choixFoyer==1){
							choixFoyer=2;
						}
						else if(choixFoyer==2){
							choixFoyer=0;
						}
						System.out.println("choixFoyer:"+choixFoyer);
						countTimeScrutation=0;
					}					
				}
				else if((changeMode == MODE_FCT)){
					countTimeScrutation++;
					ReceptionTrameUart=1;
					if(countTimeScrutation >= 10){
						MemorisationPuissance+=1;
						if(MemorisationPuissance > 10 )
						{
							MemorisationPuissance=0;
						}
					}
				}
				
				
				time = System.currentTimeMillis();		// get the current time
			}			
		}

	}

}

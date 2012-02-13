package teaonly.projects.teaonlydemo;

import java.io.*; 
import java.net.*;

import android.net.*;
import android.util.Log;
import org.xmlpull.v1.XmlSerializer;  
import android.util.Xml;

//public class NativeAgent extends GenericTask{
public class NativeAgent {
    
	public NativeAgent() {
    
    }
    
    static private native int nativeDoLogin(String userName, String passwd);
    static public void NativeDoLogin(String userName, String passwd) {
        Log.d("TEAONLY", "Call native function: nativeDoLogin");
    	nativeDoLogin(userName, passwd);
	}
    
    public static void LoadLibraries() {
        //Local library .so files before this activity created.
        //System.loadLibrary("teaonly");		
        //System.loadLibrary("ipcamera");		
    }
}

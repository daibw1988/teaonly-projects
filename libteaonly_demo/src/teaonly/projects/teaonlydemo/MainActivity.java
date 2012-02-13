package teaonly.projects.teaonlydemo;

import libteaonly.TeaonlyServer;

import java.io.*;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.WebView;

public class MainActivity extends Activity {
    private WebView webview;
	private TeaonlyServer teaServer;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.main);
        
		// setup native libraries
		NativeAgent.LoadLibraries();

		// setup Teaonly server
		try {
			teaServer = new TeaonlyServer(19790, "/android_asset");
		} catch (IOException e) {
			e.printStackTrace();
			teaServer = null;
		}
		if ( teaServer != null) {
			teaServer.registerJGI("/jgi/login", doLogin);
			teaServer.registerJGI("/jgi/query", doQuery);
		}

 		//setup webView
		webview = (WebView)findViewById(R.id.webview);
        webview.getSettings().setJavaScriptEnabled(true);
        /*
		webview.setLayoutParams(new LinearLayout.LayoutParams(                                                                                                  
					ViewGroup.LayoutParams.FILL_PARENT,
					ViewGroup.LayoutParams.FILL_PARENT, 
					1.0F));
		*/
		webview.setVerticalScrollBarEnabled(false);
		webview.loadUrl("file:///android_asset/ppcam.html");
	}
	
	private TeaonlyServer.JavaGatewayInterface doLogin = new TeaonlyServer.JavaGatewayInterface () {
		@Override
		public String doServe(String uri) {
			return "OK!";	
		}
	};

	private TeaonlyServer.JavaGatewayInterface doQuery = new TeaonlyServer.JavaGatewayInterface () {
		@Override
		public String doServe(String uri) {
			return "OK!";		
		}
	};


}

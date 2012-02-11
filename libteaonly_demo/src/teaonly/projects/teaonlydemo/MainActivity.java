package teaonly.projects.teaonlydemo;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.WebView;

public class MainActivity extends Activity {
    private WebView webview;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        
        setContentView(R.layout.main);
        webview = (WebView)findViewById(R.id.webview);
        webview.getSettings().setJavaScriptEnabled(true);
        /*
		webview.setLayoutParams(new LinearLayout.LayoutParams(                                                                                                  
					ViewGroup.LayoutParams.FILL_PARENT,
					ViewGroup.LayoutParams.FILL_PARENT, 
					1.0F));
		*/
		webview.setVerticalScrollBarEnabled(false);

		webview.loadUrl("file:///android_asset/hello.html");
    }
	


}

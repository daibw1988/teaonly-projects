package teaonly.projects.teaplayer;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.os.Handler;
import android.os.SystemClock;
import android.content.Context;
import android.util.AttributeSet;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Point;
import android.view.Display;
import android.view.SurfaceView;
import android.view.Window;
import android.view.WindowManager;
import android.view.View; 
import android.view.SurfaceHolder; 

public class TeaPlayer extends Activity implements SurfaceHolder.Callback {
	private SurfaceView videoSurface;
	private SurfaceHolder mSurfaceHolder;

	private RenderThread mRender; 
	private Bitmap	mVideoPicture;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	    requestWindowFeature(Window.FEATURE_NO_TITLE);
        
        setContentView(R.layout.main);
        Window win = getWindow();
        win.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);    
        win.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        LoadLibraries();

		videoSurface = (SurfaceView)findViewById(R.id.surface_video);
        mSurfaceHolder = videoSurface.getHolder();
        mSurfaceHolder.addCallback(this);

        Display display = getWindowManager().getDefaultDisplay();
		int width = display.getWidth();
		int height = display.getHeight();
        mVideoPicture = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);

         
	}


    /* Callback invoked when the surface dimensions change. */
    public void surfaceChanged(SurfaceHolder holder, int format, int width,
            int height) {
    }

    /*
     * Callback invoked when the Surface has been created and is ready to be
     * used.
     */
    public void surfaceCreated(SurfaceHolder holder) {
        // start the thread here so that we don't busy-wait in run()
        // waiting for the surface to be created
        startPlayer(mVideoPicture);
        mRender = new RenderThread();     
        mRender.mRun = true;
        mRender.start();
    }

    /*
     * Callback invoked when the Surface has been destroyed and must no longer
     * be touched. WARNING: after this method returns, the Surface/Canvas must
     * never be touched again!
     */
    public void surfaceDestroyed(SurfaceHolder holder) {
        boolean retry = true;
        
        mRender.mRun = false;

        while (retry) {
            try {
                mRender.join();
                retry = false;
            } catch (InterruptedException e) {
            }
        }         
    
        stopPlayer();
    }

    public static void LoadLibraries() {
        //Local library .so files before this activity created.
        System.loadLibrary("ffmpeg");		
        System.loadLibrary("teaonly");		
        System.loadLibrary("teaplayer");		
    }

    private native int startPlayer(Bitmap bmp);
    private native int renderBMP(Bitmap bmp);
    private native int stopPlayer();
	
    class RenderThread extends Thread {
    	public boolean mRun = false;

    	@Override
        public void run() {
            while (mRun) {
                Canvas c = null;
                try {
                    c = mSurfaceHolder.lockCanvas(null);
                    synchronized (mSurfaceHolder) {
        				doRender(c);            		
                    }
                    sleep(80);
                } catch (Exception e) {
                    break;                 
                } finally {
                    // do this in a finally so that if an exception is thrown
                    // during the above, we don't leave the Surface in an
                    // inconsistent state
                    if (c != null) {
                        mSurfaceHolder.unlockCanvasAndPost(c);
                    }
                }
            }
        }

        private void doRender(Canvas c) {
            if ( renderBMP( mVideoPicture) >= 0) {
                c.drawBitmap(mVideoPicture, 0, 0, null);
            }
        }
    }
}




package com.googlecode.libteaonly;

import com.googlecode.libteaonly.NanoHTTPD;

import java.io.*;
import java.util.*;
import android.util.Log;

public class TeaonlyServer extends NanoHTTPD
{
    private File homeDir;

    public TeaonlyServer(int port, String wwwroot) throws IOException {
        super(port, new File( wwwroot ).getAbsoluteFile() );
        homeDir = new File( wwwroot);
    }
    
    public Response serve( String uri, String method, Properties header, Properties parms, Properties files ) {
    	return null;
	}

    public void serveDone( Response r) {
    }
}

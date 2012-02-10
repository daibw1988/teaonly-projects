package libteaonly;

import libteaonly.NanoHTTPD;

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
        Log.d("TEAONLY", "httpd request >>" + method + " '" + uri + "' " );
    
        if ( uri.startsWith("/cgi/") ) {
            return serveCGI(uri, method, header, parms, files);
        } else if ( uri.startsWith("/jgi/") ){
            return serveJGI(uri, method, header, parms, files);
        } else if ( uri.startsWith("/live/") ){
            return serveStreaming(uri);
        } else {
            return serveFile( uri, header, homeDir, true ); 
        }
	}

    public Response serveStreaming(String uri) {
        return null;
    }

    public Response serveCGI( String uri, String method, Properties header, Properties parms, Properties files ) {
        return null;    
    }

    public Response serveJGI( String uri, String method, Properties header, Properties parms, Properties files ) {
        String jgiUri;
        int pos = uri.indexOf( "?" );
        if ( pos == -1 )
            jgiUri = uri;
        else {
            jgiUri = uri.substring(0, pos);
        }
        if ( jgiUri != null) {
            JavaGatewayInterface jgi = jgiEntries.get(jgiUri);
            if ( jgi != null) {
                String msg = jgi.doServe(uri);
                Response res = new Response( HTTP_OK, MIME_PLAINTEXT, msg);
                return res;
            }
        }
        return null;
    }

    public static interface JavaGatewayInterface {
        public String doServe(String uri); 
    }
    private Map<String, JavaGatewayInterface> jgiEntries = new HashMap<String, JavaGatewayInterface>();
    public void registerJGI(String uri, JavaGatewayInterface jgi) {
        jgiEntries.put(uri, jgi);
    }

}

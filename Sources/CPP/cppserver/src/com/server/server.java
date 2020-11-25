package com.server;

import java.net.*;
import java.io.*;
public class server {
//    static Socket serverSocket;
//    static ServerSocket bindServer;
    static ServerSocket mSocket;
    public static void main(String _args[]) throws Exception{
        int PORT = Integer.parseInt(_args[0]);
        try {
            mSocket = new ServerSocket(PORT);
            while(true) {
                System.out.println("waiting for clients (msgs are terminated with \\n) ...");
                Socket sock = mSocket.accept();
                System.out.println("client connected ....");
                InputStream in = sock.getInputStream();
                OutputStream out = sock.getOutputStream();
                int data = -1;
                StringBuffer line = new StringBuffer();
                while((data = in.read()) != -1) {
                    line.append((char)data);
                    if(((char)data) == '\n'){
                        break;
                    } // if
                } // while
                System.out.println(line);
            } // while true

        } catch(IOException _e){
            _e.printStackTrace();
        }
    }

}

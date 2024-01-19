package edu.montana.csci.csci366.demo.net;

import java.io.*;
import java.net.Socket;

public class Curl {
    public static void main(String[] args) throws Exception {
        Socket s = new Socket("www.montana.edu", 80);

        PrintWriter writer = new PrintWriter(s.getOutputStream());

        writer.println("GET / HTTP/1.1");
        writer.println("Host: www.montana.edu");
        writer.println("");
        writer.flush();

        BufferedReader bufRead = new BufferedReader(new InputStreamReader(s.getInputStream()));
        String line = bufRead.readLine();
        while(line != null){
            System.out.println(line);
            line = bufRead.readLine();
        }

        bufRead.close();
        writer.close();
        s.close();
    }
}

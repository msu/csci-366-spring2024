package edu.montana.csci.csci366.demo.net;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class SimpleServer {
    public static void main(String[] args) throws Exception {
        ServerSocket serverSocket = new ServerSocket(9000);
        while(true){
            Socket connectedSocket = serverSocket.accept();

            StringBuilder echo = new StringBuilder();
            BufferedReader bufRead = new BufferedReader(new InputStreamReader(connectedSocket.getInputStream()));
            String line = bufRead.readLine();
            while(line != null && !line.isEmpty()){
                System.out.println(line);
                echo.append(line);
                echo.append("\n");
                line = bufRead.readLine();
            }

            PrintWriter writer = new PrintWriter(connectedSocket.getOutputStream());
            writer.println("HTTP/1.1 200 OK\n\n");
            writer.println(echo);
            writer.println("\n\n");
            writer.flush();

            bufRead.close();
            writer.close();
            connectedSocket.close();
        }
    }
}

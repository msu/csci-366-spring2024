package edu.montana.csci.csci366.demo.net;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class Curl2 {
    public static void main(String[] args) throws Exception {

        URI uri = URI.create("http://montana.edu");
        var client = HttpClient.newHttpClient();
        var request = HttpRequest.newBuilder(uri).build();
        var response = client.send(request, HttpResponse.BodyHandlers.ofString());
        System.out.println(response.body());

    }
}

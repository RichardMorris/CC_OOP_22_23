package uk.ac.cornwallcollege.richardmorris.java2223;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Hello {
    BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

    public void word() {
        System.out.print("word");
    }
    
    public void great() throws Exception {
        String name = in.readLine();
        System.out.println("Hello "+name);
    }

    public static void main(String[] args) {
        System.out.println("Hello world!");
    }
}
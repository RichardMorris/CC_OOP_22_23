package uk.ac.cornwallcollege.richardmorris.java2223;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

public class Hello {

    public void great() {
        try(BufferedReader in = new BufferedReader(new InputStreamReader(System.in))) {
            String name = in.readLine();
            System.out.println("Hello "+name);
        } catch(IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

    public static void main(String[] args) {
        System.out.println("Hello world!");
    }
}
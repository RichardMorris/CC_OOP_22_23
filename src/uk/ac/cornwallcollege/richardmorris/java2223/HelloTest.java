package uk.ac.cornwallcollege.richardmorris.java2223;

import static org.junit.jupiter.api.Assertions.*;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.io.InputStream;
import java.io.StringReader;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.AfterEach;

public class HelloTest {
    private final ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    private final PrintStream originalOut = System.out;

    @BeforeEach
    public void setUpStreams() {
        System.setOut(new PrintStream(outContent));
    }

    @AfterEach
    public void restoreStreams() {
        System.setOut(originalOut);
    }

    @Test
    public void testHi() {
        assertTrue(true);
    }
    
    @Test
    public void out() {
        System.out.print("hello");
        assertEquals("hello", outContent.toString());
    }
    
    @Test
    public void testStdout() {
        Hello.main(new String[]{});
        assertEquals("Hello world!\n", outContent.toString());
    }

    @Test
    public void testInOut() throws Exception {
        String string = "Richard";
        try (InputStream stringStream = new java.io.ByteArrayInputStream(string.getBytes())) {
            System.setIn(stringStream);

            Hello h = new Hello();
            h.great();
            assertEquals("Hello Richard\n", outContent.toString());
        }
    }
    
}
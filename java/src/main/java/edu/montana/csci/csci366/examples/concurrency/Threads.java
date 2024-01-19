package edu.montana.csci.csci366.demo.concurrency;

public class Threads {

    public static void run() {

        var i = 0;

        while(i++ < 5){
            System.out.println("Running...");
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

    static class MyRunnable implements Runnable {
        public void run() {

            var i = 0;

            while(i++ < 5){
                System.out.println("Running...");
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

        }
    }

    public static void main(String[] args) throws InterruptedException {

        Thread thread = new Thread(new MyRunnable());
        System.out.println("Starting Thread...");
        thread.start();
        System.out.println("Joining Thread...");
        thread.join();
        System.out.println("Thread done, exiting...");

    }

}

package edu.montana.csci.csci366.demo.concurrency;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Semaphors {

    static Semaphore semaphore = new Semaphore(0, true);
    static Lock lock = new ReentrantLock();
    static List vals = new LinkedList();

    static void consume() {
        try {
            while (true) {
                semaphore.acquire();
                lock.lock();
                System.out.println("Consuming: " + vals.remove(0));
                lock.unlock();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static void produce() {
        while(true){
            lock.lock();
            double random = Math.random();
            System.out.println("Producing: " + random);
            vals.add(random);
            semaphore.release();
            lock.unlock();
            try {
                Thread.sleep((int) (Math.random() * 2000.0));
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Thread consumer = new Thread(Semaphors::consume);
        Thread producer = new Thread(Semaphors::produce);
        consumer.start();
        producer.start();
    }

}

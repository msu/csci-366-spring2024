package edu.montana.csci.csci366.demo.concurrency;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class HigherLevelConcurrency {

    static AtomicInteger atomicInteger = new AtomicInteger();

    public static void atomicIntDemo() {
        for (int i = 0; i < 5; i++) {
            new Thread(() -> {
                while(true){
                    int value = atomicInteger.incrementAndGet();
                    System.out.println("Current Thread: " +
                            Thread.currentThread().getId() +
                            " value: " +
                            value);
                    try {
                        Thread.sleep((long) (Math.random() * 2000));
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }

    static String[] strings = new String[]{"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L",
                                           "AA", "AB", "AC", "AD", "AE", "AF", "AG", "AH", "AI", "AJ", "AK", "AL",
                                           "BA", "BB", "BC", "BD", "BE", "BF", "BG", "BH", "BI", "BJ", "BK", "BL",
                                           "CA", "CB", "CC", "CD", "CE", "CF", "CG", "CH", "CI", "CJ", "CK", "CL",
                                           "DA", "DB", "DC", "DD", "DE", "DF", "DG", "DH", "DI", "DJ", "DK", "DL",
                                           "EA", "EB", "EC", "ED", "EE", "EF", "EG", "EH", "EI", "EJ", "EK", "EL"};
    static String getRandomString() {
        int rnd = new Random().nextInt(strings.length);
        return strings[rnd];
    }

    static HashMap<String, String> hashMap = new HashMap<>(0);
    public static void concurrentMapAccess() {
        for (int i = 0; i < 5; i++) {
            new Thread(() -> {
                while(true){
                    String randomStringToAdd = getRandomString();
                    System.out.println("Current Thread: " +
                            Thread.currentThread().getId() +
                            " adding: " +
                            randomStringToAdd);
                    hashMap.put(randomStringToAdd, randomStringToAdd.toLowerCase());
                    String randomStringToRemove = getRandomString();
                    System.out.println("Current Thread: " +
                            Thread.currentThread().getId() +
                            " removing: " +
                            randomStringToAdd);
                    hashMap.remove(randomStringToRemove);
                }
            }).start();
        }
    }

    static Map<String, String> synchronizedMap =
            Collections.synchronizedMap(new HashMap<>(0));


    public static void concurrentSynchronizedMapAccess() {
        for (int i = 0; i < 5; i++) {
            new Thread(() -> {
                while(true){
                    String randomStringToAdd = getRandomString();
                    System.out.println("Current Thread: " +
                            Thread.currentThread().getId() +
                            " adding: " +
                            randomStringToAdd);
                    synchronizedMap.put(randomStringToAdd, randomStringToAdd.toLowerCase());
                    String randomStringToRemove = getRandomString();
                    System.out.println("Current Thread: " +
                            Thread.currentThread().getId() +
                            " removing: " +
                            randomStringToAdd);
                    synchronizedMap.remove(randomStringToRemove);
                }
            }).start();
        }
    }

    static Map<String, String> concurrentHashMap =
            new ConcurrentHashMap<>();

    public static void concurrentConcurrentMapAccess() {
        for (int i = 0; i < 5; i++) {
            new Thread(() -> {
                while(true){
                    String randomStringToAdd = getRandomString();
                    System.out.println("Current Thread: " +
                            Thread.currentThread().getId() +
                            " adding: " +
                            randomStringToAdd);
                    concurrentHashMap.put(randomStringToAdd, randomStringToAdd.toLowerCase());
                    String randomStringToRemove = getRandomString();
                    System.out.println("Current Thread: " +
                            Thread.currentThread().getId() +
                            " removing: " +
                            randomStringToAdd);
                    concurrentHashMap.remove(randomStringToRemove);
                }
            }).start();
        }
    }

    static Lock lock = new ReentrantLock();
    static Map<String, Integer> countMap =
            new ConcurrentHashMap<>();
    public static void concurrentConcurrentMapAccess2() {
        for (int i = 0; i < 5; i++) {
            new Thread(() -> {
                while(true){
                    String randomStringToIncrement = getRandomString();
                    lock.lock();
                    Integer count = countMap.get(randomStringToIncrement);
                    if(count == null) {
                        count = 0;
                    }
                    count++;
                    countMap.put(randomStringToIncrement, count);
                    lock.unlock();
                }
            }).start();
        }
    }

    static void latchDemo() {
        int THREADS = 5;
        CountDownLatch latch = new CountDownLatch(THREADS);
        for (int i = 0; i < THREADS; i++) {
            new Thread(() -> {
                try {
                    Thread.sleep((int) (Math.random() * 5000));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("Decrementing latch");
                latch.countDown();
            }).start();
        }

        try {
            System.out.println("Waiting on the latch");
            latch.await();
            System.out.println("Done waiting on the latch");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }


    static ThreadPoolExecutor fixedSizeThreadPool =
            (ThreadPoolExecutor) Executors.newFixedThreadPool(10);
    static void threadPoolExample() {
        for (int i = 0; i < 100; i++) {
            int jobId = i;
            fixedSizeThreadPool.execute(() -> {
                System.out.println("Executing job " + jobId +
                        " in Thread " + Thread.currentThread().getId());
                try {
                    Thread.sleep((int) (Math.random() * 2000));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            });
        }
    }

    public static void main(String[] args) {
        concurrentMapAccess();
    }

}

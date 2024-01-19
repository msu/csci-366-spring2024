package edu.montana.csci.csci366;

import org.junit.jupiter.api.Test;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class Homework {

    int count = 0;

    /*
      For the first example, you will need to update the runnable being passed into the thread (that funny () -> { ... } thing)
      so that is increments the count variable by 1.  You'll need to start the thread to actually do the update and then call
      .join() on it so the main thread waits until the new thread is complete.
     */
    @Test
    void syncThreadsWithJoin() throws Exception {
        count = 0;
        Thread anotherThread = new Thread(() -> {
            sleep(100);
            // TODO: increment count by 1
        });
        // TODO: start the thread then join it
        assertEquals(1, count);
    }

    /*
      In this example we will use a CountdownLatch rather than join() to synchronize the main thread with th new thread
      that updates the count.  This feels a bit cleaner to me and more likely what you would do in real world code.
     */
    @Test
    void syncThreadsWithALatch() throws Exception{
        count = 0;
        CountDownLatch latch = new CountDownLatch(1);
        Thread anotherThread = new Thread(() -> {
            sleep(100);
            // TODO: increment count by 1 and count down on the latch
        });

        // TODO: start the thread then wait on the latch

        assertEquals(1, count);
    }

    /*
      In this example, rather than using an explicit thread, we will use a thread pool executor and submit our Runnable
      to that.  We will need to use the same latch-technique for blocking the main thread until after the update of
      the count variable has occurred.
     */
    ThreadPoolExecutor executor = new ThreadPoolExecutor(10, 10, 10, TimeUnit.SECONDS,
            new ArrayBlockingQueue<>(10));

    @Test
    void syncExecutorJobWithALatch() throws Exception{
        count = 0;
        CountDownLatch latch = new CountDownLatch(1);
        Runnable runnable = () -> {
            sleep(100);
            // TODO: increment count by 1 and count down on the latch
        };

        // TODO: submit the job to the executor, then wait on the latch

        assertEquals(1, count);
    }

    private void sleep(int millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

}

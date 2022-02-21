import java.util.*;
import java.util.concurrent.*;

public class Main {

    public static void main(String[] args) {
        int n = 10;
        int m = 5;
        int[] resultArray = new int[n];


        try {
            resultArray = randomFromM(n, m);
        } catch(InterruptedException e){
            System.out.println("Error: Interrupted Exception\n" + e + "\n");
        }


        outArray(resultArray);
    }

    //Input: array of elements [e1, e2, e3, ..., en]
    //Output: array of elements, where each element is the summation of all
    //corresponding elements in the input array besides the proceeding ones.
    //Example: the third element in the output array will be e3 + e4 + e5 + ... + en.
    static int[] randomFromM(int n, int m) throws InterruptedException {
        int[] result = new int[n];
        ExecutorService mahPool = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());

        for (int i = 0; i < n; i++) {
            mahPool.execute(new RunnableWorker(result, m, i));
        }
        mahPool.shutdown();
        mahPool.awaitTermination(10, TimeUnit.SECONDS);

        return result;
    }

    static void outArray(int[] array){
        for(int i=0; i<array.length; i++){
            System.out.print(array[i] + " ");
        }
        System.out.print("\n");
    }

}

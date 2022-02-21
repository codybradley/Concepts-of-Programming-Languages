import java.util.*;
import java.util.concurrent.*;

public class Main {

    public static void main(String[] args) {
        int[] inputArray = new int[] {3, 5, 2, 6, 1};
        int[] outputArray = new int[inputArray.length];

        try {
            outputArray = partialArraySum(inputArray);
        } catch(InterruptedException e){
            System.out.println("Error: Interrupted Exception\n" + e + "\n");
        }

        outArray(inputArray);
        outArray(outputArray);
    }

    //Input: array of elements [e1, e2, e3, ..., en]
    //Output: array of elements, where each element is the summation of all
    //corresponding elements in the input array besides the proceeding ones.
    //Example: the third element in the output array will be e3 + e4 + e5 + ... + en.
    static int[] partialArraySum(int[] input) throws InterruptedException {
        int[] result = new int[input.length];//output array same length as input
        ExecutorService mahPool = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());

        for (int i = 0; i < input.length; i++) {
            mahPool.execute(new RunnableWorker(input, i, result));
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

import java.util.stream.*;

public class RunnableWorker implements Runnable {
    private final int[] inputArray;
    private final int startInterval;
    private int[] result;

    //constructor
    public RunnableWorker(int[] inputArray, int intervalStart, int[] result) {
        this.inputArray = inputArray;
        this.startInterval = intervalStart;
        this.result = result;
    }

    @Override
    public void run() {
//        for (int i = startInterval; i < inputArray.length; i++) {
//            result[startInterval] += inputArray[i];
//        }
        result[startInterval] = IntStream
                .range(startInterval, inputArray.length)
                //if startInterval is 3 and inputArray.length is 10,
                //creates a stream of numbers: 3, 4, 5, 6, 7, 8, 9
                .map(x -> inputArray[x])//stream becomes value at each index
                .sum();//sums the values in the stream
    }
}

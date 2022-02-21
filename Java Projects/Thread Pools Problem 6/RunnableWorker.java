import java.util.*;
import java.util.stream.*;

public class RunnableWorker implements Runnable {
    private int[] result;
    private int range;
    private int index;

    //constructor
    public RunnableWorker(int[] result, int range, int index) {
        this.result = result;
        this.range = range;
        this.index = index;
    }

    @Override
    public void run() {
        Random rand = new Random();

        result[index] = rand.nextInt(range)+1;//1, 2, ..., 25
    }
}

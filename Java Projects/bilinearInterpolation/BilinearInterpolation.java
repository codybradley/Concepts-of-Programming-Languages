import java.util.concurrent.*;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class BilinearInterpolation {

    public static void main(String[] args) throws IOException {
        long time1, time2;
        float timeElapsed;
        File lenna = new File("Lenna100.jpg");
        BufferedImage image = ImageIO.read(lenna);

        //testing
        System.out.println("1920 by 1080 pixels, scaled up 1.6x");

        System.out.println("Serial Approach");
        time1 = System.currentTimeMillis();
        BufferedImage image2 = scale(image, 1.6f, 1.6f);
        File lenna2 = new File("Lenna100_larger_S.jpg");
        ImageIO.write(image2, "jpg", lenna2);
        time2 = System.currentTimeMillis();
        timeElapsed = (time2-time1)/1000F;
        System.out.println("Time Used: " + timeElapsed + " seconds");

        System.out.println("Parallel Approach");
        time1 = System.currentTimeMillis();
        BufferedImage image3 = scale_a(image, 1.6f, 1.6f);
        File lenna3 = new File("Lenna100_larger_P.jpg");
        ImageIO.write(image3, "jpg", lenna3);
        time2 = System.currentTimeMillis();
        timeElapsed = (time2-time1)/1000F;
        System.out.println("Time Used: " + timeElapsed + " seconds");
    }

    /* gets the 'n'th byte of a 4-byte integer */
    public static int get(int self, int n) {
        return (self >> (n * 8)) & 0xFF;
    }

    public static float lerp(float s, float e, float t) {
        return s + (e - s) * t;
    }

    public static float blerp(final Float c00, float c10, float c01, float c11, float tx, float ty) {
        return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
    }

    //Serial approach
    public static BufferedImage scale(BufferedImage self, float scaleX, float scaleY) {
        int newWidth = (int) (self.getWidth() * scaleX);
        int newHeight = (int) (self.getHeight() * scaleY);
        BufferedImage newImage = new BufferedImage(newWidth, newHeight, self.getType());
        for (int x = 0; x < newWidth; ++x) {
            for (int y = 0; y < newHeight; ++y) {
                float gx = ((float) x) / newWidth * (self.getWidth() - 1);
                float gy = ((float) y) / newHeight * (self.getHeight() - 1);
                int gxi = (int) gx;
                int gyi = (int) gy;
                int rgb = 0;
                int c00 = self.getRGB(gxi, gyi);
                int c10 = self.getRGB(gxi + 1, gyi);
                int c01 = self.getRGB(gxi, gyi + 1);
                int c11 = self.getRGB(gxi + 1, gyi + 1);
                for (int i = 0; i <= 2; ++i) {
                    float b00 = get(c00, i);
                    float b10 = get(c10, i);
                    float b01 = get(c01, i);
                    float b11 = get(c11, i);
                    int ble = ((int) blerp(b00, b10, b01, b11, gx - gxi, gy - gyi)) << (8 * i);
                    rgb = rgb | ble;
                }
                newImage.setRGB(x, y, rgb);
            }
        }
        return newImage;
    }

    //Parallel approach using thread pools
    public static BufferedImage scale_a(BufferedImage self, float scaleX, float scaleY) {

        ExecutorService pool = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());

        int newWidth = (int) (self.getWidth() * scaleX);
        int newHeight = (int) (self.getHeight() * scaleY);
        BufferedImage newImage = new BufferedImage(newWidth, newHeight, self.getType());
        for (int x = 0; x < newWidth; ++x) {
            pool.execute(new BiRunnableWorker(self, newImage, newWidth, newHeight, x));
        }
        pool.shutdown();

        try {
            if (!pool.awaitTermination(60, TimeUnit.SECONDS)) {
                pool.shutdownNow();
            }
        } catch (InterruptedException ex) {
            pool.shutdownNow();
            Thread.currentThread().interrupt();
        }

        return newImage;
    }
}
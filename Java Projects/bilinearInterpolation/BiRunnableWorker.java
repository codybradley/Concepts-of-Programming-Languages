import java.awt.image.BufferedImage;

public class BiRunnableWorker implements Runnable {
    private BufferedImage self;
    private BufferedImage newImage;
    private int newWidth;
    private int newHeight;
    private int x;

    //constructor
    public BiRunnableWorker(BufferedImage self, BufferedImage newImage, int newWidth, int newHeight, int x) {
        this.self = self;
        this.newImage = newImage;
        this.newWidth = newWidth;
        this.newHeight = newHeight;
        this.x = x;
    }

    @Override
    public void run() {
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
                float b00 = BilinearInterpolation.get(c00, i);
                float b10 = BilinearInterpolation.get(c10, i);
                float b01 = BilinearInterpolation.get(c01, i);
                float b11 = BilinearInterpolation.get(c11, i);
                int ble = ((int) BilinearInterpolation.blerp(b00, b10, b01, b11, gx - gxi, gy - gyi)) << (8 * i);
                rgb = rgb | ble;
            }
            newImage.setRGB(x, y, rgb);
        }
    }
}
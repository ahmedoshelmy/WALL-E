package uni.proj.rv.games;

import android.graphics.Bitmap;
import android.graphics.RectF;

import org.opencv.android.Utils;
import org.opencv.core.Point;
import org.opencv.core.Scalar;

import java.util.LinkedList;

import uni.proj.ec.Command;
import uni.proj.rv.ImageProcessing;
import uni.proj.rv.RobotGame;
import uni.proj.rv.fragments.CameraFragment;

public class TestingGame extends RobotGame {

    public TestingGame(CameraFragment context) {
        super(context);
    }

    @Override
    public void onGameSelected() {
        enableCustomDisplay();
        print("Test: onGameStarted\n");

        try {
            sendCommand(Command.fromString("set_game{g=-1}"));
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    @Override
    public void onGameNotSelected() {
        print("Test: onGameNotSelected\n");

    }

    @Override
    public void onStart() {
        print("Test: onStart\n");
        getFragment().setColorLow(BLUE_LOW);
        getFragment().setColorHigh(BLUE_HIGH);
    }

    Bitmap previewBuffer;
    LinkedList<RectF> res = new LinkedList<>();
    Point location;

    private final Scalar BLUE_LOW = new Scalar(159, 67, 117);
    private final Scalar BLUE_HIGH = new Scalar(248, 195, 255);

    @Override
    public Bitmap onCameraFeed(Bitmap image) {
        if (previewBuffer == null){
            previewBuffer = Bitmap.createBitmap(image);
        }else{ //other wise , then we just need to make sure it has the same size as the frame
            if (previewBuffer.getWidth() != image.getWidth() || previewBuffer.getHeight() != image.getHeight()){
                previewBuffer = Bitmap.createBitmap(image.getWidth() , image.getHeight() , Bitmap.Config.ARGB_8888);
            }
        }

        // 132 51 87 251 173 255

        //do a color detection on the image from the camera
        //the blur value is just here to make the image smoother
        // getSelectedColorLow() -> the selected lower range color from the user (click the eye icon to view in-app)
        // getSelectedColorHigh() -> the selected upper range color from the user (click the eye icon to view in-app)
        location = ImageProcessing.DetectLaser(image);
        res.clear();
        res.add(new RectF((int) location.x - 15 , (int) location.y - 15 , (int) location.x + 15 , (int) location.y + 15));
        res.addAll(ImageProcessing.DetectColoredBalls(image, 5, getSelectedColorLow(), getSelectedColorHigh() , 30 , 400));

        //this game returns a image to the user because we enabled the displaying with enableCustomDisplay()
        //so , I just get the type of preview the user want to see , then return it
        //almost every time you will take this code copy-paste
        switch (getCurrentPreview()) {
            //copy the input image
            case 0 : Utils.matToBitmap(ImageProcessing.src, previewBuffer); break;

            //copy the image after applying the blur
            case 1 : Utils.matToBitmap(ImageProcessing.blur, previewBuffer); break;

            //copy the image after applying the hsv clipping
            case 2 :Utils.matToBitmap(ImageProcessing.hsv, previewBuffer); break;

            //copy the image after converting it to a mask
            case 3 : Utils.matToBitmap(ImageProcessing.mask, previewBuffer); break;

            //copy the image after applying the dilate filter (tbh idk wtf does this filter do , but hay , it was on StackOverflow so why not)
            case 4 :Utils.matToBitmap(ImageProcessing.dilate, previewBuffer); break;

            //copy the image after applying the erode filter
            case 5 : Utils.matToBitmap(ImageProcessing.erode, previewBuffer); break;

            //copy the final image after converting it to blobs
            case 6 : Utils.matToBitmap(ImageProcessing.blob, previewBuffer); break;
        }


        return previewBuffer;
    }

    @Override
    public LinkedList<RectF> getDetectionResult() {
        return res;
    }


    @Override
    public void onStop() {
        print("Test: onStop\n");
    }

    @Override
    public void onCommand(Command c) {
        print("<< " + c.toString() + "\n");
    }
}

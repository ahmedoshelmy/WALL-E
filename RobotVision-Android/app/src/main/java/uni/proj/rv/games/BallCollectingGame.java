package uni.proj.rv.games;

import android.graphics.Bitmap;
import android.graphics.RectF;
import android.util.Log;
import android.widget.Toast;

import org.opencv.android.Utils;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;

import java.util.LinkedList;

import uni.proj.ec.Command;
import uni.proj.rv.ImageProcessing;
import uni.proj.rv.RobotGame;
import uni.proj.rv.fragments.CameraFragment;

public class BallCollectingGame extends RobotGame {
    Bitmap previewBuffer;
    private final Scalar RED_BALL_LOW = new Scalar(0, 70, 50);
    private final Scalar RED_BALL_HIGH = new Scalar(10, 255, 255);
    private final Scalar BLUE_BALL_LOW = new Scalar(90, 70, 50);
    private final Scalar BLUE_BALL_HIGH = new Scalar(130, 255, 255);
    LinkedList<RectF> res_red;
    LinkedList<RectF> res_blue;

    String prev_comm ;
    String curr_comm ;

    public BallCollectingGame(CameraFragment context) {
        super(context);
        prev_comm = "";
        curr_comm = "";
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
    }


    //received a frame , now we need to process it

    @Override
    public Bitmap onCameraFeed(Bitmap image) {

        //if this is the first frame ever , then create a bitmap matching the source
        if (previewBuffer == null){
            previewBuffer = Bitmap.createBitmap(image);
        }else{ //other wise , then we just need to make sure it has the same size as the frame
            if (previewBuffer.getWidth() != image.getWidth() || previewBuffer.getHeight() != image.getHeight()){
                previewBuffer = Bitmap.createBitmap(image.getWidth() , image.getHeight() , Bitmap.Config.ARGB_8888);
            }
        }

        //do a color detection on the image from the camera
        //the blur value is just here to make the image smoother
        // getSelectedColorLow() -> the selected lower range color from the user (click the eye icon to view in-app)
        // getSelectedColorHigh() -> the selected upper range color from the user (click the eye icon to view in-app)
        // ======= BLUE =========
        //res_blue = ImageProcessing.DetectColoredBalls(image, 5, new Scalar(0,1,59), new Scalar(255,174,156));
        //  process_result(true);
        res_red = ImageProcessing.DetectColoredBalls(image, 5, RED_BALL_LOW, RED_BALL_HIGH);
        process_result(false);
        //now do the processing we need on the results we just got

        //res = ImageProcessing.DetectColoredBalls(image, 5, new Scalar(0.0,53,75), new Scalar(254, 181,255));

        //res = ImageProcessing.DetectColoredBalls(image, 5, new Scalar(0,18,44), new Scalar(247, 233,201));

        //process_result(false);

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

        //return the selected image
        return previewBuffer;
    }


    private void process_result(boolean taken_or_not) {
        RectF nearest_ball = null;
        float nearest_distance = Float.MAX_VALUE;
        float cx_screen = height() / 2.0f; // (cx_screen, cy_screen) = center of screen
        float cy_screen = width() / 2.0f;
        float acceptable_range = 0.3f;     //

        for (RectF ball : res_red) {
            float cx_ball = (ball.top + ball.bottom) / 2; // notice the phone is in landscape, so X and Y are switched
            float cy_ball = (ball.right + ball.left) / 2; // notice the phone is in landscape, so X and Y are switched
            float dx = cx_ball - cx_screen;
            float dy = cy_ball - cy_screen;
            float distance = (float) Math.sqrt(dx * dx + dy * dy);

            if (distance < nearest_distance) {
                nearest_ball = ball;
                nearest_distance = distance;
            }
        }

        try {

            if (nearest_ball != null) {
                float cx_nearest = (nearest_ball.top + nearest_ball.bottom) / 2;
                float cy_nearest = (nearest_ball.right + nearest_ball.left) / 2;

                if (cx_nearest > cx_screen * (1 + acceptable_range)) { //right
                    curr_comm = "set_dir{d=1}";
                } else if (cx_nearest < cx_screen * (1 - acceptable_range)) { // left
                    curr_comm = "set_dir{d=2}";
                } else { //forward
                    curr_comm = "set_dir{d=3}";
                }
            } else {
                curr_comm = "set_dir{d=0}";
            }

            if(!curr_comm.equals(prev_comm)) {
                print("java c: " + curr_comm + "\n");
                sendCommand(Command.fromString(curr_comm));
                prev_comm=curr_comm;
            }

        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    //return the detection results to view it
    @Override
    public LinkedList<RectF> getDetectionResult() {
        return res_red;
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
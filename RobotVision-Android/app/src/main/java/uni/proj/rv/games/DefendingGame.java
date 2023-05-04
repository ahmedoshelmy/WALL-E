package uni.proj.rv.games;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;

import org.opencv.android.Utils;

import java.util.LinkedList;

import uni.proj.ec.Command;
import uni.proj.rv.ImageProcessing;
import uni.proj.rv.RobotGame;
import uni.proj.rv.fragments.CameraFragment;

public class DefendingGame extends RobotGame {

    /**
     * default constructor , you don't need to edit any of this
     * */
    public DefendingGame(CameraFragment context) {
        super(context);
    }

    /**
     * selected the Defending game , send a command to arduino to tell it to update the game
     * */
    @Override
    public void onGameSelected() {
        super.onGameSelected();
        try {
            //send the set game command to the arduino
            sendCommand(Command.fromString("set_game{g=2}"));
        }catch (Exception e){
            e.printStackTrace();
        } //between a try-catch because Command.fromString may return an error
    }

    @Override
    public void onGameNotSelected() {
        //we don't need to do anything here
        //because the newly selected game will tell the arduino ot update
        super.onGameNotSelected();
    }

    //if you have any vars that need to be initialized , you should put it here
    @Override
    public void onStart() {
        super.onStart();
        //enable custom display for this game
        enableCustomDisplay();
    }


    //received a frame , now we need to process it
    Bitmap previewBuffer;
    LinkedList<RectF> res;
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
        res = ImageProcessing.DetectColoredBalls(image , 5 , getSelectedColorLow() , getSelectedColorHigh());


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

        //now do the processing we need on the results we just got
        process_result(previewBuffer);

        //return the selected image
        return previewBuffer;
    }


    private void process_result(Bitmap image){

        //defending game should only see one ball
        if (res.size() > 1){
            print("ERROR: in a defending game we shouldn't have more than one result\n");
        }

        float center_line = -0.1f; //put it with negative value if you want to shift the center line left
        //positive for right
        //zero for no shift
        //range : [-0.5 , 0.5]

        float acceptable_range = 0.1f; //of screen width , 0.5 means the hole screen will be in the acceptable range
		
		//use a Canvas object to draw on the screen (image that will be displayed on the screen)
        Canvas canv = new Canvas(image);

        //center line position in coords
        float width2 = height() / 2.0f + height() * center_line; //again , width and height are switched

        Paint paint = new Paint();
        paint.setStrokeWidth(10);
		
		//draw the center line
        paint.setColor(Color.RED);
        paint.setAlpha(200);
        canv.drawLine(0 , width2 , width() , width2 , paint);
		
		//draw the right range indicator
        paint.setColor(Color.GREEN);
        paint.setAlpha(200);
        canv.drawLine(0 , width2 + acceptable_range * height() , width() , width2 + acceptable_range * height() , paint);
		
		//draw the left range indicator
        paint.setColor(Color.GREEN);
        paint.setAlpha(200);
        canv.drawLine(0 , width2 - acceptable_range * height() , width() , width2 - acceptable_range * height() , paint);


        //move according to the first detected ball
        if (res.size() >= 1){ //not the best way to do it .. but Im too lazy to think of something else
            RectF ball = res.get(0); //get the first result

            float cx = (ball.top + ball.bottom); //notice the phone is in landscape , so X and Y are switched

            if (cx > width2 + acceptable_range * height()){
                print("Robot should move right\n");
                //TODO: send the "move right" command to the arduino
                //Ex:
//                try {
//                    sendCommand(Command.fromString("move{direction = 1}"));
//                }catch (Exception e){
//                    e.printStackTrace();
//                }
                //but we didn't agree on commands yet ..

            } else if (cx < width2 - acceptable_range * height()){
                print("Robot should move left\n");
                //TODO: send the "move left" command to the arduino
                //Ex:
//                try {
//                    sendCommand(Command.fromString("move{direction = -1}"));
//                }catch (Exception e){
//                    e.printStackTrace();
//                }

            } else {
                print("Robot should stand still\n");
                //TODO: send the "stop" command to the arduino
                //Ex:
//                try {
//                    sendCommand(Command.fromString("move{direction = 0}"));
//                }catch (Exception e){
//                    e.printStackTrace();
//                }

            }
        }
    }

    //return the detection results to view it
    @Override
    public LinkedList<RectF> getDetectionResult() {
        return res;
    }


    @Override
    public void onStop() {
        super.onStop();
        //disable the custom display for this game
        //tbh you don't need to do that xDD
        //but to keep it consistent
        disableCustomDisplay();
    }


    @Override
    public void onCommand(Command c) {
        //TODO: if the arduino want to say something to us , we will receive it here ..
        print("Received a command: '" + c.cmd + "'\n");
    }
}

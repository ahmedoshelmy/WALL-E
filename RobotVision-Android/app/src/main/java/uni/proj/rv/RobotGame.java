package uni.proj.rv;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.RectF;

import org.opencv.core.Scalar;

import java.util.LinkedList;
import java.util.Objects;

import uni.proj.ec.Command;
import uni.proj.rv.fragments.CameraFragment;


/**
 * a class that represents a Robot Game
 * you may want to have a look at ImageProcessing.java , it may contain some handy functions
 * */
public abstract class RobotGame {

    private final CameraFragment context;
    private boolean running = false;
    private boolean selected = false;
    private boolean customDisplay = false;

    public RobotGame(CameraFragment context) {
        this.context = context;
    }

    /**
     * will be called when the user selects this game
     * */
    public void onGameSelected() {
        selected = true;
    }

    /**
     * will be called when the user selects another game
     * */
    public void onGameNotSelected() {
        selected = false;
    }

    /**
     * will be called when the start button is clicked
     * */
    public void onStart() {
        running = true;
    }

    /**
     * will be called when the Camera sends a frame
     * @param image the current frame
     * @return Bitmap (to preview , only when #enableCustomDisplay() was called before) or null if not a display
     * */
    public abstract Bitmap onCameraFeed(Bitmap image);

    /**
     * if the game want to view the detection results , then it should return it in this list
     * return null for disabling ..
     * this function will be called right after calling onCameraFeed(Bitmap) , it should only
     * return the result of the previous detection and not do any calculation
     * */
    public abstract LinkedList<RectF> getDetectionResult();

    /**
     * will be called when the user click the stop button
     * */
    public void onStop() {
        running = false;
    }

    /**
     * will be called when a command is received form arduino
     * */
    public abstract void onCommand(Command c) throws Exception;

    /**
     * returns if arduino is currently connected or not
     * */
    public boolean isConnected(){
        return General.communication.isConnected();
    }

    /**
     * sends a command to the arduino
     * */
    public boolean sendCommand(Command c){
        if (General.communication.isConnected())
            General.communication.send(c);
        return General.communication.isConnected();
    }

    /**
     * returns a ref to the current context object , you normally wouldn't need to use this function
     * */
    public Context getContext(){
        return context.getContext();
    }

    /**
     * returns a ref to the current CameraFragment object , you normally wouldn't need to use this function
     * */
    public CameraFragment getFragment(){
        return context;
    }

    /**
     * sends a raw text to Arduino
     * @param text the text to send
     * */
    @Deprecated
    public boolean send(String text){
        if (General.communication.isConnected())
            General.communication.send(text);
        return General.communication.isConnected();
    }



    /**
     * return the screen width
     * notice that we will use the phone in a 90 deg rotation
     * so you may need to take that into considuration
     * */
    public int width(){
        return context.rotatedBuffer.getWidth();
    }

    /**
     * return the screen Height
     * */
    public int height(){
        return context.rotatedBuffer.getHeight();
    }

    /**
     * print a string to the output
     * @param str the message to output
     * */
    public void print(String str){
        context.requireActivity().runOnUiThread(() -> {
                context.printSerial(str);
        });
    }

    /**
     * returns true if this game is the current selected game
     * false other-wise
     * */
    public boolean isSelected() {
        return selected;
    }

    /**
     * returns true if the game is on Running mode (aka: user clicked the run button)
     * */
    public boolean isRunning() {
        return running;
    }

    /**
     * this is used by the Fragment to determine what to do with the result of #onCameraFeed(Bitmap)
     * */
    public boolean isDisplayEnabled(){
        return customDisplay;
    }

    /**
     * enable display for this game
     * */
    public void enableCustomDisplay(){
        customDisplay = true;
    }

    /**
     * disable display for this game
     * */
    public void disableCustomDisplay(){
        customDisplay = false;
    }

    /**
     * returns the current low Color selected by the user
     * */
    public Scalar getSelectedColorLow(){
        return context.getSelectedColorLow();
    }

    /**
     * returns the current high Color selected by the user
     * */
    public Scalar getSelectedColorHigh(){
        return context.getSelectedColorHigh();
    }

    /**
     * returns the preview buffer that is selected by the user
     * switch (getCurrentPreview()) {
     *             case 0 : Utils.matToBitmap(ImageProcessing.src, previewBuffer); break;
     *             case 1 : Utils.matToBitmap(ImageProcessing.blur, previewBuffer); break;
     *             case 2 :Utils.matToBitmap(ImageProcessing.hsv, previewBuffer); break;
     *             case 3 : Utils.matToBitmap(ImageProcessing.mask, previewBuffer); break;
     *             case 4 :Utils.matToBitmap(ImageProcessing.dilate, previewBuffer); break;
     *             case 5 : Utils.matToBitmap(ImageProcessing.erode, previewBuffer); break;
     *             case 6 : Utils.matToBitmap(ImageProcessing.blob, previewBuffer); break;
     *         }
     * */
    public int getCurrentPreview(){
        return context.getCurrent_preview();
    }



}

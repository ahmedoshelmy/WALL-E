package uni.proj.rv.games;

import android.graphics.Bitmap;
import android.graphics.RectF;

import org.opencv.android.Utils;
import org.opencv.core.Scalar;

import java.util.LinkedList;

import uni.proj.ec.Command;
import uni.proj.rv.ImageProcessing;
import uni.proj.rv.RobotGame;
import uni.proj.rv.fragments.CameraFragment;

public class DartsGame extends RobotGame {
    private final Scalar BLUE_LOW = new Scalar(0, 57, 98);
    private final Scalar BLUE_HIGH = new Scalar(146, 255, 172);
    private final Scalar RED_LOW = new Scalar(0, 148, 176);
    private final Scalar RED_HIGH = new Scalar(69, 242, 255);
    Bitmap previewBuffer;
    LinkedList<RectF> redResult;
    LinkedList<RectF> blueResult;
    boolean stopped = false;
    boolean laserON = false;
    boolean laserXdone = false;
    boolean laserYdone = false;
    boolean shot = false;
    boolean ACK = true;

    public DartsGame(CameraFragment context) {
        super(context);
    }

    @Override
    public void onGameSelected() {
        try {
            sendCommand(Command.fromString("set_game{g=1}"));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onGameNotSelected() {
        super.onGameNotSelected();
    }

    @Override
    public void onStart() {
        super.onStart();
        try {
            sendCommand(Command.fromString(String.format("RELOAD{}")));
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        //enable custom display for this game
        enableCustomDisplay();
    }

    @Override
    public Bitmap onCameraFeed(Bitmap image) {

        //if this is the first frame ever , then create a bitmap matching the source
        if (previewBuffer == null) {
            previewBuffer = Bitmap.createBitmap(image);
        } else { //other wise , then we just need to make sure it has the same size as the frame
            if (previewBuffer.getWidth() != image.getWidth() || previewBuffer.getHeight() != image.getHeight()) {
                previewBuffer = Bitmap.createBitmap(image.getWidth(), image.getHeight(), Bitmap.Config.ARGB_8888);
            }
        }

        //do a color detection on the image from the camera
        //the blur value is just here to make the image smoother
        // getSelectedColorLow() -> the selected lower range color from the user (click the eye icon to view in-app)
        // getSelectedColorHigh() -> the selected upper range color from the user (click the eye icon to view in-app)
        blueResult = ImageProcessing.DetectColoredBalls(image, 5, BLUE_LOW, BLUE_HIGH);

        float center_line = 0.0f;
        float noise_margin = 0.01f;
        final double centerX = height() / 2.0f + height() * center_line;

        if (blueResult.size() > 0 && !stopped) {
            while (!ACK) ;
            try {
                // stop car rotation and turn on laser
                sendCommand(Command.fromString(String.format("STOP{}")));
                stopped = true;
                ACK = false;
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        } else if (stopped && !laserON) {
            RectF point = blueResult.get(0);
            double X = (point.top + point.bottom) / 2.0f;
            // in the center
            if (Math.abs(X - centerX) <= noise_margin) {
                // turn on the laser and stop servo movement
                laserON = true;
                while (!ACK) ;
                try {
                    sendCommand(Command.fromString(String.format("LO{}"))); // laser on
                    ACK = false;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
            // the bull eye is on left
            else if (X < centerX) {
                // rotate left the servo by 1 deg
                while (!ACK) ;
                try {
                    sendCommand(Command.fromString(String.format("RSL{}"))); // rotate servo left
                    ACK = false;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
            // the bull eye is on right
            else if (X > centerX) {
                //rotate servo right by 1 deg
                while (!ACK) ;
                try {
                    sendCommand(Command.fromString(String.format("RSR{}"))); // rotate servo right
                    ACK = false;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
        }
        // align laser x
        else if (laserON && !laserXdone) {
            redResult = ImageProcessing.DetectColoredBalls(image, 5, RED_LOW, RED_HIGH);
            RectF blue_point = blueResult.get(0);
            RectF red_point = redResult.get(0);
            double blueX = (blue_point.top + blue_point.bottom) / 2.0f;
            double redX = (red_point.top + red_point.bottom) / 2.0f;
            // in the center
            if (Math.abs(redX - blueX) <= noise_margin) {
                // x is adjusted
                laserXdone = true;
            }
            // the red is on left
            else if (redX < blueX) {
                // rotate left the servo by 1 deg
                try {
                    while (!ACK) ;
                    sendCommand(Command.fromString(String.format("RSL{}")));
                    ACK = false;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
            // the red is on right
            else if (redX > blueX) {
                try {
                    while (!ACK) ;
                    sendCommand(Command.fromString(String.format("RSR{}")));
                    ACK = false;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
        }
        //align laser y
        else if (laserYdone) {
            redResult = ImageProcessing.DetectColoredBalls(image, 5, RED_LOW, RED_HIGH);
            RectF blue_point = blueResult.get(0);
            RectF red_point = redResult.get(0);
            double blueY = (blue_point.left + blue_point.right) / 2.0f;
            double redY = (red_point.left + red_point.right) / 2.0f;
            // in the center
            if (Math.abs(redY - blueY) <= noise_margin) {
                // y is adjusted
                laserYdone = true;
            }
            // the red is on below
            else if (redY < blueY) {
                // rotate up the servo by 1 deg
                try {
                    while (!ACK) ;
                    sendCommand(Command.fromString(String.format("RGU{}")));
                    ACK = false;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
            // the red is on above
            else if (redY > blueY) {
                // rotate down the servo by 1 deg
                try {
                    while (!ACK) ;
                    sendCommand(Command.fromString(String.format("RGD{}")));
                    ACK = false;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
            // alignment done
        } else if (laserXdone && laserYdone && !shot) {
            try {
                while (!ACK) ;
                sendCommand(Command.fromString(String.format("SHOT{}")));
                ACK = false;
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
            stopped = false;
            laserON = false;
            laserXdone = false;
            laserYdone = false;
            shot = true;
        }

        //this game returns a image to the user because we enabled the displaying with enableCustomDisplay()
        //so , I just get the type of preview the user want to see , then return it
        //almost every time you will take this code copy-paste
        switch (getCurrentPreview()) {
            //copy the input image
            case 0:
                Utils.matToBitmap(ImageProcessing.src, previewBuffer);
                break;

            //copy the image after applying the blur
            case 1:
                Utils.matToBitmap(ImageProcessing.blur, previewBuffer);
                break;

            //copy the image after applying the hsv clipping
            case 2:
                Utils.matToBitmap(ImageProcessing.hsv, previewBuffer);
                break;

            //copy the image after converting it to a mask
            case 3:
                Utils.matToBitmap(ImageProcessing.mask, previewBuffer);
                break;

            //copy the image after applying the dilate filter (tbh idk wtf does this filter do , but hay , it was on StackOverflow so why not)
            case 4:
                Utils.matToBitmap(ImageProcessing.dilate, previewBuffer);
                break;

            //copy the image after applying the erode filter
            case 5:
                Utils.matToBitmap(ImageProcessing.erode, previewBuffer);
                break;

            //copy the final image after converting it to blobs
            case 6:
                Utils.matToBitmap(ImageProcessing.blob, previewBuffer);
                break;
        }

        //now do the processing we need on the results we just got
//        process_result(previewBuffer);

        //return the selected image
        return previewBuffer;
    }

    @Override
    public LinkedList<RectF> getDetectionResult() {
        return null;
    }
    
    @Override
    public void onStop() {
        while (!ACK) ;
        try {
            sendCommand(Command.fromString(String.format("SHOT{}")));
            ACK = false;
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        shot = false;
    }

    @Override
    public void onCommand(Command c) {
        ACK = true;
    }
}

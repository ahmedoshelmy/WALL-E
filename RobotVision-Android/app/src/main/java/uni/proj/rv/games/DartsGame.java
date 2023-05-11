package uni.proj.rv.games;

import android.app.NotificationManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.RectF;
import android.media.MediaPlayer;

import androidx.annotation.NonNull;
import androidx.lifecycle.DefaultLifecycleObserver;
import androidx.lifecycle.LifecycleOwner;

import org.opencv.android.Utils;
import org.opencv.core.Point;
import org.opencv.core.Scalar;

import java.util.LinkedList;

import uni.proj.ec.Command;
import uni.proj.rv.ImageProcessing;
import uni.proj.rv.R;
import uni.proj.rv.RobotGame;
import uni.proj.rv.fragments.CameraFragment;

public class DartsGame extends RobotGame {
    private final Scalar BLUE_LOW = new Scalar(42, 80, 87);
    private final Scalar BLUE_HIGH = new Scalar(94, 255, 160);

    private final Scalar RED_LOW = new Scalar(0, 148, 176);
    private final Scalar RED_HIGH = new Scalar(69, 242, 255);


    Bitmap previewBuffer;
    Point redResult;
    LinkedList<RectF> blueResult;
    boolean stopped = false;
    boolean laserON = false;
    boolean laserXdone = false;
    boolean laserYdone = false;
    boolean shot = false;
    boolean ACK = true;
    RectF _lastTarget;
    int _nullFrames = 0;
    final int MAX_NULL_FRAMES = 5;

    private MediaPlayer suii;

    public DartsGame(CameraFragment context) {
        super(context);

        suii = MediaPlayer.create(getContext() , R.raw.suii);
        getFragment().getLifecycle().addObserver(new DefaultLifecycleObserver() {
            @Override
            public void onDestroy(@NonNull LifecycleOwner owner) {
                DefaultLifecycleObserver.super.onDestroy(owner);
                suii.release();
            }
        });
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
            ACK = false;
            _lastTarget = null;
            _nullFrames = 0;
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
        // getSelectedColorLow()  -> the selected lower range color from the user (click the eye icon to view in-app)
        // getSelectedColorHigh() -> the selected upper range color from the user (click the eye icon to view in-app)
        blueResult = ImageProcessing.DetectColoredBalls(image, 5, BLUE_LOW, BLUE_HIGH , 30 , 400);
        _lastTarget = null;
        if (_lastTarget != null) {
            RectF min = null;
            for (RectF r : blueResult) {
                if (min == null || dis2(r, _lastTarget) < dis2(min, _lastTarget)) {
                    min = r;
                }
            }
            _lastTarget = min;
        } else {
            if (blueResult.size() == 0) _lastTarget = null;
            else _lastTarget = blueResult.get(0);
        }

        if (ACK && !shot) {

            float center_line = 0.0f;
            float noise_margin = 2; //in pixels
            final double centerX = height() / 2.0f + height() * center_line;

            if (_lastTarget == null){
                _nullFrames++;
                if (_nullFrames > MAX_NULL_FRAMES){
                    stopped = true;
                    laserON = true;
                    laserXdone = true;
                    laserYdone = true;
                }
            }

            if (_lastTarget != null && !stopped) {
                try {
                    // stop car rotation and turn on laser
                    sendCommand(Command.fromString(String.format("STOP{}")));
                    print("stop\n");
                    stopped = true;
                    ACK = false;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            } else if (_lastTarget != null && stopped && !laserON) {
                RectF point = _lastTarget;
                double X = (point.top + point.bottom) / 2.0f;
                // in the center
                if (Math.abs(X - centerX) <= noise_margin) {
                    // turn on the laser and stop servo movement
                    laserON = true;
                    try {
                        sendCommand(Command.fromString(String.format("LO{}"))); // laser on
                        print("Laser on\n");
                        ACK = false;
                    } catch (Exception e) {
                        throw new RuntimeException(e);
                    }
                }
                // the bull eye is on left
                else if (X < centerX) {
                    // rotate left the servo by 1 deg
                    try {
                        sendCommand(Command.fromString(String.format("RSL{}"))); // rotate servo left
                        print("rotate servo left, before laser\n");
                        ACK = false;
                    } catch (Exception e) {
                        throw new RuntimeException(e);
                    }
                }
                // the bull eye is on right
                else if (X > centerX) {
                    //rotate servo right by 1 deg
                    try {
                        sendCommand(Command.fromString(String.format("RSR{}"))); // rotate servo right
                        ACK = false;
                        print("rotate servo right,before laser\n");
                    } catch (Exception e) {
                        throw new RuntimeException(e);
                    }
                }
            }
            //align laser y
            else if (_lastTarget != null && laserON && !laserYdone) {
                redResult = ImageProcessing.DetectLaser(image);

                //redResult = ImageProcessing.DetectColoredBalls(image, 5, RED_LOW, RED_HIGH);
                if (redResult.x != 0 && redResult.y != 0) {
                    RectF blue_point = _lastTarget;
                    double blueY = (blue_point.left + blue_point.right) / 2.0f;
                    double redY = redResult.x;
                    // in the center
                    if (Math.abs(redY - blueY) <= noise_margin) {
                        // y is adjusted
                        laserYdone = true;
                    }
                    // the red is on below
                    else if (redY < blueY) {
                        // rotate up the servo by 1 deg
                        try {
                            sendCommand(Command.fromString(String.format("RGU{}")));
                            print("rotate gun up\n");
                            ACK = false;
                        } catch (Exception e) {
                            throw new RuntimeException(e);
                        }
                    }
                    // the red is on above
                    else if (redY > blueY) {
                        // rotate down the servo by 1 deg
                        try {
                            sendCommand(Command.fromString(String.format("RGD{}")));
                            print("rotate gun down\n");
                            ACK = false;
                        } catch (Exception e) {
                            throw new RuntimeException(e);
                        }
                    }
                }
                // alignment done

            }
            // align laser x
            else if (_lastTarget != null && laserON && !laserXdone) {
                redResult = ImageProcessing.DetectLaser(image);
                //laserXdone = true;
                if (redResult.x != 0 && redResult.y != 0) {
                    RectF blue_point = _lastTarget;
                    //RectF red_point = redResult.get(0);
                    double blueX = (blue_point.top + blue_point.bottom) / 2.0f;
                    double redX = redResult.y;
                    // in the center
                    if (Math.abs(redX - blueX) <= noise_margin) {
                        // x is adjusted
                        laserXdone = true;
                    }
                    // the red is on left
                    else if (redX < blueX) {
                        // rotate left the servo by 1 deg
                        try {
                            sendCommand(Command.fromString(String.format("RSR{}")));
                            print("rotate servo left, after laser\n");
                            ACK = false;
                        } catch (Exception e) {
                            throw new RuntimeException(e);
                        }
                    }
                    // the red is on right
                    else if (redX > blueX) {
                        try {
                            sendCommand(Command.fromString(String.format("RSL{}")));
                            print("rotate servo right,after laser\n");
                            ACK = false;
                        } catch (Exception e) {
                            throw new RuntimeException(e);
                        }
                    }
                }
            } else if (laserXdone && laserYdone && !shot) {
                try {
                    sendCommand(Command.fromString(String.format("SHOT{}")));
                    print("shot\n");
                    suii.start();
                    ACK = false;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
                shot = true;
            }
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

        //return the selected image
        return previewBuffer;
    }

    @Override
    public LinkedList<RectF> getDetectionResult() {
        LinkedList<RectF> res = new LinkedList<>();
        if (blueResult != null)
            res.addAll(blueResult);
        if (redResult != null)
            res.add(new RectF((float) redResult.x - 5 , (float)redResult.y - 5 , (float)redResult.x + 5 , (float)redResult.y + 5));

        return res;
    }

    @Override
    public void onStop() {

        try {
            sendCommand(Command.fromString(String.format("SHOT{}")));
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        shot = false;
        stopped = false;
        laserON = false;
        laserXdone = false;
        laserYdone = false;
        ACK = true;
        super.onStop();
    }

    @Override
    public void onCommand(Command c) {
        print("Received command : " + c.cmd + "\n");
        ACK = true;
    }


    private static float dis2(RectF a , RectF b){
        float ax = (a.left + a.right) / 2;
        float ay = (a.top + a.bottom) / 2;

        float bx = (b.left + b.right) / 2;
        float by = (b.top + b.bottom) / 2;

        return (ax - bx) * (ax - bx) + (ay - by) * (ay - by);
    }
}
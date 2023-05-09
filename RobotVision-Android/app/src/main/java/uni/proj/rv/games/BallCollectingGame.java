package uni.proj.rv.games;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;

import org.opencv.android.Utils;
import org.opencv.core.Scalar;

import java.util.LinkedList;

import uni.proj.ec.Command;
import uni.proj.rv.ImageProcessing;
import uni.proj.rv.RobotGame;
import uni.proj.rv.fragments.CameraFragment;


public class BallCollectingGame extends RobotGame {

    enum  Color {
        RED,
        BLUE
    }

    enum  DIRECTION {
        FRONT,
        LEFT,
        RIGHT
    }

    enum ACTIONS {
        NOTHING,
        TK_FORWARD,
        TK_RIGHT,
        TK_LEFT,
        AVD_FROWARD,

        STP_TURN
    }



    Bitmap previewBuffer;
    private final Scalar RED_BALL_LOW = new Scalar(0, 70, 50);
    private final Scalar RED_BALL_HIGH = new Scalar(10, 255, 255);
    private final Scalar BLUE_BALL_LOW = new Scalar(90, 70, 50);
    private final Scalar BLUE_BALL_HIGH = new Scalar(130, 255, 255);
    LinkedList<RectF> res_red;
    LinkedList<RectF> res_blue;

    // TODO : orientation  adding more parameter to enhance
    private boolean is_turning ;
    private float prev_turn_deg;


    private float cx_screen; // (cx_screen, cy_screen) = center of screen
    private float cy_screen;
    //TODO: choose proper value for acceptable_range
    private final float acceptable_range = 0.5f;
    private float right_border;
    private float left_border;

    private ACTIONS curr_action;
    private ACTIONS prev_action;


    private class PingBall {
        public RectF ball;
        public Color color;
        public float cx  ;
        public float cy ;

        public PingBall(RectF bd, Color c) {
            ball = bd;
            color = c;
            cx = (ball.top + ball.bottom) / 2; // notice the phone is in landscape, so X and Y are switched
            cy = (ball.right + ball.left) / 2; // notice the phone is in landscape, so X and Y are switched
        }
        public float getDistanceFromCenterScreen() {
            float dx = cx - cx_screen;
            float dy = cy - cy_screen;
            return (float) Math.sqrt(dx * dx + dy * dy);
        }

        public DIRECTION getDirection() {
            if (cx > right_border) { //right
                return DIRECTION.RIGHT;
            } else if (cx < left_border) { // left
                return DIRECTION.LEFT;

            } else { //forward
                return DIRECTION.FRONT;
            }
        }

        public String toString() {

            return "Ball \t" + color.toString() + "\t" + getDirection().toString() + "\n";
        }

    }



    public BallCollectingGame(CameraFragment context) {
        super(context);
        is_turning = false;
        curr_action = ACTIONS.NOTHING;
        prev_action = ACTIONS.NOTHING;
    }
    @Override
    public void onGameSelected() {
        enableCustomDisplay();
        print("Test: onGameStarted\n");

        try {
            sendCommand(Command.fromString("set_game{g=0}"));
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


        cx_screen = height() / 2.0f; // (cx_screen, cy_screen) = center of screen
        cy_screen = 0;

        left_border = cx_screen * (1 - acceptable_range);
        right_border  = cx_screen * (1 + acceptable_range);


        DrawLine(image);

        res_blue = ImageProcessing.DetectColoredBalls(image, 5, BLUE_BALL_LOW, BLUE_BALL_HIGH);
        res_red = ImageProcessing.DetectColoredBalls(image, 5, RED_BALL_LOW, RED_BALL_HIGH);
        process_result();

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


    private void process_result() {
        float [] mx_dist = {Float.MAX_VALUE, Float.MAX_VALUE}; // {front, left_or_right}
        PingBall [] nearest_balls = {null, null};// {front, left_or_right}

        getNearestBall(mx_dist, nearest_balls, res_red, Color.RED);
        getNearestBall(mx_dist, nearest_balls, res_blue, Color.BLUE);

        //* The priority goes to OUR FRONT
        if(nearest_balls[DIRECTION.FRONT.ordinal()] != null) {
            switch (nearest_balls[DIRECTION.FRONT.ordinal()].color) {
                case RED:
                    curr_action = ACTIONS.AVD_FROWARD;
                    break;
                case BLUE:
                    curr_action = ACTIONS.TK_FORWARD;
                    break;
            }
        }
        else if(nearest_balls[1] != null && nearest_balls[1].color == Color.BLUE) {
            curr_action =  (nearest_balls[1].getDirection() == DIRECTION.LEFT) ?
                        ACTIONS.TK_LEFT : ACTIONS.TK_RIGHT;
        }
        else {
            curr_action = ACTIONS.NOTHING;
        }
       // print("Curr action = " + curr_action + "\n");
        //print("prev action = " + prev_action + "\n");
        if(curr_action != prev_action) {

            switch (curr_action) {
                case AVD_FROWARD:
                    print("Avoid Forward\n");
                    break;
                case TK_FORWARD:
                    print("Move Forward\n");
                    break;
                case TK_RIGHT:
                    print("Move Right\n");
                    break;
                case TK_LEFT:
                    print("Move Left\n");
                    break;
                default:
                    print("nothing\n");
                    break;
            }

            try {
                sendCommand(Command.fromString("set_act{a="+curr_action.ordinal()+"}"));
            } catch (Exception e) {
                throw new RuntimeException(e);
            }

            prev_action = curr_action;
        }

    }

    private void getNearestBall(float [] mx_dist, PingBall [] nearest_balls, LinkedList<RectF> res, Color color) {
        for (RectF ball : res) {
            PingBall tmp = new PingBall(ball, color);
            //print(tmp.toString());
            float dist = tmp.getDistanceFromCenterScreen();
            //as i need left & right = 1
            int index = tmp.getDirection() == DIRECTION.FRONT ? DIRECTION.FRONT.ordinal() : DIRECTION.LEFT.ordinal();
            if(dist < mx_dist[index] || (dist == mx_dist[index] && color == Color.RED) ) {
                mx_dist[index] = dist;
                nearest_balls[index] = tmp;
            }
        }
    }

    private void DrawLine(Bitmap image){
        Canvas canv = new Canvas(image);

        //center line position in coords

        Paint paint = new Paint();
        paint.setStrokeWidth(10);

        paint.setColor(android.graphics.Color.RED);
        paint.setAlpha(200);
        canv.drawLine(0 , left_border , width() , left_border , paint);

        paint.setColor(android.graphics.Color.GREEN);
        paint.setAlpha(200);
        canv.drawLine(0 , right_border , width() , right_border , paint);

        drawSquare(canv, cx_screen, cy_screen, 50);


    }

    public void drawSquare(Canvas canvas, float cx, float cy, float size) {
        Paint paint = new Paint();
        paint.setColor(android.graphics.Color.BLUE);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(5);

        float left = cy - size / 2;
        float top = cx - size / 2;
        float right = cy + size / 2;
        float bottom = cx + size / 2;

        RectF rect = new RectF(left, top, right, bottom);
        canvas.drawRect(rect, paint);
    }

    //return the detection results to view it
    @Override
    public LinkedList<RectF> getDetectionResult() {
        return res_red;
    }


    // TODO : orientation
    public void handelDegree(float deg) {
        if(is_turning) {
            print("deg: " + (deg-prev_turn_deg));
        }
        if(is_turning && Math.abs( deg - prev_turn_deg) >= 90) {
            try {
                sendCommand(Command.fromString("set_act{a="+ 5+"}"));
            } catch (Exception e) {
                throw new RuntimeException(e);
            }

            is_turning = false;
        }

        if(!is_turning ){
            prev_turn_deg = deg;
        }
    }
    @Override
    public void onStop() {
        print("Test: onStop\n");
    }



    @Override
    public void onCommand(Command c) {
        // TODO : orientation set prev_deg with it 
        /*if(c.cmd.equals("start_moving")) {

         }*/
        // if arduino send start turning
        // set it true
        if(c.cmd.equals("start_turn")) {
            is_turning = true;
        }
        print("<< " + c.toString()  + "\n");

    }
}
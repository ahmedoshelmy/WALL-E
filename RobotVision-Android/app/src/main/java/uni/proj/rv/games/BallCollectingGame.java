package uni.proj.rv.games;

import android.graphics.Bitmap;
import android.graphics.RectF;
import android.widget.Toast;

import java.util.LinkedList;

import uni.proj.ec.Command;
import uni.proj.rv.RobotGame;
import uni.proj.rv.fragments.CameraFragment;

public class BallCollectingGame extends RobotGame {
    public BallCollectingGame(CameraFragment context) {
        super(context);
    }

    @Override
    public void onGameSelected() {
        try {
            Toast.makeText(getContext() , "Selected" , Toast.LENGTH_SHORT).show();
            sendCommand(Command.fromString("set_game{g=0}"));
        }catch (Exception e){
            e.printStackTrace();
            Toast.makeText(getContext() , "Error:" + e.getMessage() , Toast.LENGTH_SHORT).show();

        }
    }

    @Override
    public void onGameNotSelected() {

    }

    @Override
    public void onStart() {

    }

    @Override
    public Bitmap onCameraFeed(Bitmap image) {

        return null;
    }

    @Override
    public LinkedList<RectF> getDetectionResult() {
        return null;
    }

    @Override
    public void onStop() {

    }

    @Override
    public void onCommand(Command c) {

    }
}

package uni.proj.rv.games;

import android.graphics.Bitmap;
import android.graphics.RectF;

import java.util.LinkedList;

import uni.proj.ec.Command;
import uni.proj.rv.RobotGame;
import uni.proj.rv.fragments.CameraFragment;

public class ShootingGame extends RobotGame {
    public ShootingGame(CameraFragment context) {
        super(context);
    }

    @Override
    public void onGameSelected() {
        try {
            sendCommand(Command.fromString("set_game{g=4}"));
        }catch (Exception e){
            e.printStackTrace();
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

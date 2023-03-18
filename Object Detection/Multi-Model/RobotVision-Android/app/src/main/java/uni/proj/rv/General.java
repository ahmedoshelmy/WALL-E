package uni.proj.rv;

import android.app.Activity;

import uni.proj.ec.SerialCommunication;

public class General {
    public static SerialCommunication communication;

    public void init(Activity c){
        communication = new SerialCommunication(c, new SerialCommunication.SerialCommunicationListener() {
            @Override
            public void onConnected() {

            }

            @Override
            public void onConnectionFailed(int reason) {

            }

            @Override
            public void onDisconnected() {

            }

            @Override
            public void onPermissionFailed() {

            }

            @Override
            public void onDataReceived(byte[] data) {

            }

            @Override
            public void onDeviceAttached() {

            }

            @Override
            public void onDeviceDetached() {

            }
        });
    }
}

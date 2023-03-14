package uni.proj.ec;

import android.os.Bundle;

import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AppCompatActivity;

import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import java.nio.charset.StandardCharsets;

public class MainActivity extends AppCompatActivity {
    private EditText command_input;
    private TextView serial;
    private SerialCommunication communication;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        command_input = findViewById(R.id.command_input);
        serial = findViewById(R.id.serial);

        findViewById(R.id.fab).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (communication.isConnected()){
                    try {
                        Command c = Command.fromString(command_input.getText().toString());
                        Snackbar.make(findViewById(R.id.fab) , "Sending.. " + c.arguments.length , Snackbar.LENGTH_SHORT).show();

                        communication.send(c);

                    } catch (Exception e) {
                        e.printStackTrace();
                        Snackbar.make(findViewById(R.id.fab) , "Err: " + e.getMessage() , Snackbar.LENGTH_SHORT).show();
                    }

                }else{
                    Snackbar.make(findViewById(R.id.fab) , "not connected" , Snackbar.LENGTH_SHORT).show();
                }
            }
        });

        communication = new SerialCommunication(this, new SerialCommunication.SerialCommunicationListener() {
            @Override
            public void onConnected() {
                Snackbar.make(findViewById(R.id.fab) , "Connected" , Snackbar.LENGTH_SHORT).show();
            }

            @Override
            public void onConnectionFailed(int reason) {
                Snackbar.make(findViewById(R.id.fab) , "Failed to connect : " + reason , Snackbar.LENGTH_SHORT).show();
            }

            @Override
            public void onDisconnected() {
                Snackbar.make(findViewById(R.id.fab) , "Disconnected" , Snackbar.LENGTH_SHORT).show();
            }

            @Override
            public void onPermissionFailed() {
                Snackbar.make(findViewById(R.id.fab) , "Permission Failed" , Snackbar.LENGTH_SHORT).show();
            }

            @Override
            public void onDataReceived(final byte[] data) {
                serial.post(
                        () -> {serial.setText(serial.getText() + new String(data , StandardCharsets.UTF_8));}
                );
            }

            @Override
            public void onDeviceAttached() {
                communication.connect();
                Snackbar.make(findViewById(R.id.fab) , "Connecting..." , Snackbar.LENGTH_SHORT).show();
            }

            @Override
            public void onDeviceDetached() {
                Snackbar.make(findViewById(R.id.fab) , "Removed" , Snackbar.LENGTH_SHORT).show();
            }
        });
    }
}
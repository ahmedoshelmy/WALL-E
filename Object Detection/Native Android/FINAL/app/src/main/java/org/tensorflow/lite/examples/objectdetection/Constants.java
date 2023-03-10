package org.tensorflow.lite.examples.objectdetection;


import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import com.felhr.usbserial.UsbSerialDevice;
import com.felhr.usbserial.UsbSerialInterface;

import java.util.HashMap;
import java.util.Map;

public class Constants {
    public static Context AppContext;
    public static Activity Act;

    public static UsbSerialDevice serialPort;
    public static UsbManager usbManager;
    public static UsbDevice device;
    public static UsbDeviceConnection connection;

    public static final BroadcastReceiver recv = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals("org.tensorflow.lite.examples.objectdetection.USB_PERMISSION")) {
                boolean granted =
                        intent.getExtras().getBoolean(UsbManager.EXTRA_PERMISSION_GRANTED);
                if (granted) {
                    Toast.makeText(Act, "Request granted", Toast.LENGTH_SHORT).show();
                    connection = usbManager.openDevice(device);
                    serialPort = UsbSerialDevice.createUsbSerialDevice(device, connection);
                    if (serialPort != null) {
                        if (serialPort.open()) { //Set Serial Connection Parameters.
                            serialPort.setBaudRate(9600);
                            serialPort.setDataBits(UsbSerialInterface.DATA_BITS_8);
                            serialPort.setStopBits(UsbSerialInterface.STOP_BITS_1);
                            serialPort.setParity(UsbSerialInterface.PARITY_NONE);
                            serialPort.setFlowControl(UsbSerialInterface.FLOW_CONTROL_OFF);
                            Toast.makeText(Act, "Connected", Toast.LENGTH_SHORT).show();


                            serialPort.read(data -> Act.runOnUiThread(() -> {
                                TextView tv = Act.findViewById(R.id.fragment_logger);
                                tv.setText(tv.getText() + new String(data));
                            }));

//                            new Thread(() -> {
//                                try {
//                                    Thread.sleep(5000);
//                                } catch (InterruptedException e) {
//                                    e.printStackTrace();
//                                }
//
//                                while (true) {
//                                    serialPort.write("a".getBytes());
//                                    try {
//                                        Thread.sleep(2000);
//                                    } catch (InterruptedException e) {
//                                        e.printStackTrace();
//                                    }
//                                }
//                            }).start(); //for debugging only

                        } else {
                            Log.d("SERIAL", "PORT NOT OPEN");
                        }
                    } else {
                        Log.d("SERIAL", "PORT IS NULL");
                    }
                } else {
                    Log.d("SERIAL", "PERM NOT GRANTED");
                }
            } else if (intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_ATTACHED)) {
                Toast.makeText(Act, "Connection in", Toast.LENGTH_SHORT).show();
                CreateConnection(context);
            } else if (intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_DETACHED)) {
                connection = null;
                serialPort = null;
            }
        }
    };

    public static void init() {
        usbManager = (UsbManager) Act.getSystemService(Context.USB_SERVICE);
        IntentFilter i = new IntentFilter();
        i.addAction("org.tensorflow.lite.examples.objectdetection.USB_PERMISSION");
        i.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
        i.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
        Act.registerReceiver(recv, i);
        Toast.makeText(Act, "Registered receiver", Toast.LENGTH_SHORT).show();
    }

    public static void CreateConnection(Context c) {
        HashMap<String, UsbDevice> usbDevices = usbManager.getDeviceList();
        if (!usbDevices.isEmpty()) {
            boolean keep = true;
            for (Map.Entry<String, UsbDevice> entry : usbDevices.entrySet()) {
                device = entry.getValue();
                int deviceVID = device.getVendorId();
                if (deviceVID == 0x2341)//Arduino Vendor ID
                {
                    Toast.makeText(c, "Requesting permission", Toast.LENGTH_SHORT).show();
                    PendingIntent pi = PendingIntent.getBroadcast(c, 0,
                            new Intent("org.tensorflow.lite.examples.objectdetection.USB_PERMISSION"), PendingIntent.FLAG_MUTABLE);
                    usbManager.requestPermission(device, pi);
                    Toast.makeText(c, "Sending request", Toast.LENGTH_SHORT).show();
                    keep = false;
                } else {
                    connection = null;
                    device = null;
                }

                if (!keep)
                    break;
            }
        }
    }
}

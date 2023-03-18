package uni.proj.ec;

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

import com.felhr.usbserial.UsbSerialDevice;
import com.felhr.usbserial.UsbSerialInterface;

import java.util.HashMap;
import java.util.Map;

public class SerialCommunication {
    private static final String TAG = "SerialCommunication";
    private static final String ACTION_USB_PERMISSION = "uni.proj.ec.usb";
    Activity act;
    private final UsbManager usbManager;
    private UsbSerialDevice serialPort;

    public static final int ERR_PORT_NULL   = 0;
    public static final int ERR_PORT_CLOSED = 1;

    SerialCommunicationListener mSCL;

    public SerialCommunication(Activity act , SerialCommunicationListener listener){
        this.act = act;
        mSCL = listener;
        usbManager = (UsbManager) act.getSystemService(Context.USB_SERVICE);
        if (usbManager == null){
            throw new NullPointerException("Usb manager == null");
        }

        IntentFilter f = new IntentFilter();
        f.addAction(ACTION_USB_PERMISSION);
        f.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
        f.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);

        act.registerReceiver(new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                if (intent.getAction().equals(ACTION_USB_PERMISSION)) {
                    boolean granted =
                            intent.getExtras().getBoolean(UsbManager.EXTRA_PERMISSION_GRANTED);

                    if (serialPort != null)
                        mSCL.onDisconnected();

                    if (granted) {
                        connection = usbManager.openDevice(device);
                        serialPort = UsbSerialDevice.createUsbSerialDevice(device, connection);
                        if (serialPort != null) {
                            if (serialPort.open()) { //Set Serial Connection Parameters.
                                serialPort.setBaudRate(31250);
                                serialPort.setDataBits(UsbSerialInterface.DATA_BITS_8);
                                serialPort.setStopBits(UsbSerialInterface.STOP_BITS_1);
                                serialPort.setParity(UsbSerialInterface.PARITY_NONE);
                                serialPort.setFlowControl(UsbSerialInterface.FLOW_CONTROL_OFF);
                                serialPort.read(mReadCallback); //
                                mSCL.onConnected();
                            } else {
                                Log.w(TAG, "onReceive: failed to open port");
                                mSCL.onConnectionFailed(ERR_PORT_CLOSED);
                            }
                        } else {
                            Log.w(TAG, "onReceive: Port was null");
                            mSCL.onConnectionFailed(ERR_PORT_NULL);
                        }
                    } else {
                        Log.w(TAG, "onReceive: Permission not granted");
                        mSCL.onPermissionFailed();
                    }
                } else if (intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_ATTACHED)) {
                    mSCL.onDeviceAttached();
                } else if (intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_DETACHED)) {
                    mSCL.onDeviceDetached();
                }
            }
        } , f);
    }

    private final UsbSerialInterface.UsbReadCallback mReadCallback = new UsbSerialInterface.UsbReadCallback() {
        @Override
        public void onReceivedData(byte[] data) {
            mSCL.onDataReceived(data);
        }
    };

    private UsbDevice device;
    private UsbDeviceConnection connection;

    public void connect(){
        HashMap<String, UsbDevice> usbDevices = usbManager.getDeviceList();
        if (!usbDevices.isEmpty()) {
            boolean keep = true;
            for (Map.Entry<String, UsbDevice> entry : usbDevices.entrySet()) {
                device = entry.getValue();
                int deviceVID = device.getVendorId();
                if (deviceVID == 0x2341)//Arduino Vendor ID
                {
                    PendingIntent pi = PendingIntent.getBroadcast(act, 0,
                            new Intent(ACTION_USB_PERMISSION), PendingIntent.FLAG_MUTABLE);
                    usbManager.requestPermission(device, pi);
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

    public void close(){
        if (serialPort == null)
            throw new IllegalStateException("not connected");
        serialPort.close();
        serialPort = null;
        mSCL.onDisconnected();
    }

    public void send(String text){
        if (serialPort == null)
            throw new IllegalStateException("not connected");
        serialPort.write(text.getBytes());
    }

    public void write(byte b){
        if (serialPort == null)
            throw new IllegalStateException("not connected");

        serialPort.write(new byte[] {b});
    }

    public void write(byte[] b){
        if (serialPort == null)
            throw new IllegalStateException("not connected");
        serialPort.write(b);
    }

    public void send(Command c){
        byte n = (byte) c.cmd.length();
        write(n);
        send(c.cmd);
        write((byte) c.arguments.length);
        for (int i = 0;i < c.arguments.length;i++){
            write((byte) c.arguments[i].name.length());
            send(c.arguments[i].name);
            if (c.arguments[i].value instanceof Float){ //float
                int bits = Float.floatToIntBits((float) c.arguments[i].value);
                byte[] bytes = new byte[4];
                write((byte) 4);
                bytes[0] = (byte)(bits & 0xff);
                bytes[1] = (byte)((bits >> 8) & 0xff);
                bytes[2] = (byte)((bits >> 16) & 0xff);
                bytes[3] = (byte)((bits >> 24) & 0xff);
                write(bytes);
            }else if (c.arguments[i].value instanceof Integer){ //int
                int bits = (int) c.arguments[i].value;
                byte[] bytes = new byte[4];
                write((byte) 4);
                bytes[0] = (byte)(bits & 0xff);
                bytes[1] = (byte)((bits >> 8) & 0xff);
                bytes[2] = (byte)((bits >> 16) & 0xff);
                bytes[3] = (byte)((bits >> 24) & 0xff);
                write(bytes);
            } else { //string
                String str_v = c.arguments[i].value.toString();
                write((byte) str_v.length());
                send(str_v);
            }
        }

    }


    public boolean isConnected(){
        return serialPort != null;
    }

    public interface SerialCommunicationListener {
        void onConnected();
        void onConnectionFailed(int reason);
        void onDisconnected();
        void onPermissionFailed();
        void onDataReceived(byte[] data);
        void onDeviceAttached();
        void onDeviceDetached();
    }
}

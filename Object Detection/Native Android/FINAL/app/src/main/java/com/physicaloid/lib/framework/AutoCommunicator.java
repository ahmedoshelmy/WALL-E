/*
 * Copyright (C) 2013 Keisuke SUZUKI
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * Distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.physicaloid.lib.framework;

import android.content.Context;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.widget.TextView;
import android.widget.Toast;

import com.physicaloid.lib.UsbVidList;
import com.physicaloid.lib.usb.UsbAccessor;
import com.physicaloid.lib.usb.driver.uart.UartCdcAcm;
import com.physicaloid.lib.usb.driver.uart.UartCp210x;
import com.physicaloid.lib.usb.driver.uart.UartFtdi;

import org.tensorflow.lite.examples.objectdetection.Constants;
import org.tensorflow.lite.examples.objectdetection.R;

public class AutoCommunicator {
    @SuppressWarnings("unused")
    private static final String TAG = AutoCommunicator.class.getSimpleName();

    public AutoCommunicator() {
    }

    public SerialCommunicator getSerialCommunicator(Context context) {
        UsbAccessor usbAccess = UsbAccessor.INSTANCE;
        usbAccess.init(context);
        String ids = "";
        for(UsbDevice device : usbAccess.manager().getDeviceList().values()) {
            int vid = device.getVendorId();
            ids = vid + "\n";
            for(UsbVidList usbVid : UsbVidList.values()) {
                if(vid == usbVid.getVid()) {
                    if(vid == UsbVidList.FTDI.getVid()) {
                        return new UartFtdi(context);
                    } else if(vid == UsbVidList.CP210X.getVid()) {
                        return new UartCp210x(context);
                    }
                }
            }
        }

        ((TextView) Constants.Act.findViewById(R.id.fragment_logger)).setText(ids);



        return new UartCdcAcm(context);
    }
}

package com.pk.bluetoothinterfaceandroid;

import static android.Manifest.permission.BLUETOOTH;
import static android.Manifest.permission.BLUETOOTH_CONNECT;

import androidx.activity.result.ActivityResultLauncher;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MyActivity";
    Spinner spinnerBTPairedDevices;
    Button bluetoothConnect;
    Button FWD;
    Button RIGHT;
    Button LEFT;
    Button REV;
    Button STOP;
    FloatingActionButton SHARE;
    Button popUpSave;
    Button popUpCancel;
    EditText editValue;
    LinearLayout linearLayoutPopUpSaveData;
    PopupWindow popupWindowSaveData;
    FloatingActionButton DELETE;
    TextView textView;
    BluetoothAdapter bluetoothAdapter = null;
    BluetoothSocket BTSocket = null;
    BluetoothDevice BTDevice = null;
    Set<BluetoothDevice> bluetoothDevice = null;

//    static public final int REQUEST_ENABLE_BT = 10;
    static public String FwdData = "1";
    static public String RevData = "2";
    static public String RightData = "3";
    static public String LeftData = "4";
    static public String StopData = "5";
    static public final int BT_CON_STATUS_NOT_CONNECTED = 0;
    static public final int BT_CON_STATUS_CONNECTING = 1;
    static public final int BT_CON_STATUS_CONNECTED = 2;
    static public final int BT_CON_STATUS_FAILED = 3;
    static public final int BT_CON_STATUS_CONNECTiON_LOST = 4;
    static public int iBTConnectionStatus = BT_CON_STATUS_NOT_CONNECTED;
    static final int BT_STATE_LISTENING = 1;
    static final int BT_STATE_CONNECTING = 2;
    static final int BT_STATE_CONNECTED = 3;
    static final int BT_STATE_CONNECTION_FAILED = 4;
    static final int BT_STATE_MESSAGE_RECEIVED = 5;
    boolean BTConnected = false;
    static final UUID my_uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    classBTInitDataCommunication cBTInitSendReceive = null;
    private static final int ReqCode = 100;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d(TAG, "onCreate: OnCreate-Start ");
        textView = findViewById(R.id.idMATextViewReceiver);
        textView.setMovementMethod(new ScrollingMovementMethod());
        spinnerBTPairedDevices = findViewById(R.id.idMASpinnerBT);
        bluetoothConnect = findViewById(R.id.idMAButtonConnect);
        FWD = findViewById(R.id.idMAButtonFWD);
        REV = findViewById(R.id.idMAButtonREV);
        RIGHT = findViewById(R.id.idMAButtonRight);
        LEFT = findViewById(R.id.idMAButtonLeft);
        STOP = findViewById(R.id.idMAButtonSTOP);
        DELETE = findViewById(R.id.idMADelete);
        SHARE = findViewById(R.id.idMAShare);



        FWD.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String val = getData("FwdData", "1");
                sendMessage(val, "FWD");
            }
        });
        FWD.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {

                Log.d(TAG, "Button Long Press FWD");
                LayoutInflater layoutInflater = (LayoutInflater) MainActivity.this.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                View customView = layoutInflater.inflate(R.layout.pop_up_layout, null);
                popUpSave = customView.findViewById(R.id.SaveButton);
                popUpCancel = customView.findViewById(R.id.CancelButton);
                editValue = customView.findViewById(R.id.EditButtonValue);
                linearLayoutPopUpSaveData = customView.findViewById(R.id.popUpLayout);
                popupWindowSaveData = new PopupWindow(customView, LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                popupWindowSaveData.setFocusable(true);
                popupWindowSaveData.update();
                //display the popup window
                popupWindowSaveData.showAtLocation(linearLayoutPopUpSaveData, Gravity.CENTER, 0, 0);

                popUpCancel.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        popupWindowSaveData.dismiss();
                    }
                });

                popUpSave.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {

                        if (editValue.getText().length() < 1) {
                            Toast.makeText(getApplicationContext(), "Please enter the Data", Toast.LENGTH_SHORT).show();
                            return;
                        }
                        FwdData = editValue.getText().toString();
                        SaveData("FwdData", FwdData);
                        Log.d(TAG, "onClick: " + editValue.getText().toString());
                        Log.d(TAG, "onClick: " + FwdData);
                        popupWindowSaveData.dismiss();
                    }
                });

                return false;
            }
        });

        REV.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String val = getData("RevData", "2");
                sendMessage(val, "REV");

            }
        });
        REV.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {

                Log.d(TAG, "Button Long Press REV");
                LayoutInflater layoutInflater = (LayoutInflater) MainActivity.this.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                View customView = layoutInflater.inflate(R.layout.pop_up_layout, null);
                popUpSave = customView.findViewById(R.id.SaveButton);
                popUpCancel = customView.findViewById(R.id.CancelButton);
                editValue = customView.findViewById(R.id.EditButtonValue);
                linearLayoutPopUpSaveData = customView.findViewById(R.id.popUpLayout);
                popupWindowSaveData = new PopupWindow(customView, LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                popupWindowSaveData.setFocusable(true);
                popupWindowSaveData.update();
                //display the popup window
                popupWindowSaveData.showAtLocation(linearLayoutPopUpSaveData, Gravity.CENTER, 0, 0);

                popUpCancel.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        popupWindowSaveData.dismiss();
                    }
                });

                popUpSave.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {

                        if (editValue.getText().length() < 1) {
                            Toast.makeText(getApplicationContext(), "Please enter Data", Toast.LENGTH_SHORT).show();
                            return;
                        }
                        RevData = editValue.getText().toString();
                        SaveData("RevData", RevData);
                        Log.d(TAG, "onClick: " + RevData);
                        popupWindowSaveData.dismiss();
                    }
                });

                return false;
            }
        });

        RIGHT.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String val = getData("RightData", "3");
                sendMessage(val, "RIGHT");

            }
        });
        RIGHT.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {

                Log.d(TAG, "Button Long Press RIGHT");
                LayoutInflater layoutInflater = (LayoutInflater) MainActivity.this.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                View customView = layoutInflater.inflate(R.layout.pop_up_layout, null);
                popUpSave = customView.findViewById(R.id.SaveButton);
                popUpCancel = customView.findViewById(R.id.CancelButton);
                editValue = customView.findViewById(R.id.EditButtonValue);
                linearLayoutPopUpSaveData = customView.findViewById(R.id.popUpLayout);
                popupWindowSaveData = new PopupWindow(customView, LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                popupWindowSaveData.setFocusable(true);
                popupWindowSaveData.update();
                //display the popup window
                popupWindowSaveData.showAtLocation(linearLayoutPopUpSaveData, Gravity.CENTER, 0, 0);

                popUpCancel.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        popupWindowSaveData.dismiss();
                    }
                });

                popUpSave.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {

                        if (editValue.getText().length() < 1) {
                            Toast.makeText(getApplicationContext(), "Please enter Data", Toast.LENGTH_SHORT).show();
                            return;
                        }
                        RightData = editValue.getText().toString();
                        SaveData("RightData", RightData);
                        Log.d(TAG, "onClick: " + RightData);
                        popupWindowSaveData.dismiss();
                    }
                });

                return false;
            }
        });
        LEFT.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String val = getData("LeftData", "4");
                sendMessage(val, "LEFT");

            }
        });
        LEFT.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {

                Log.d(TAG, "Button Long Press LEFT");
                LayoutInflater layoutInflater = (LayoutInflater) MainActivity.this.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                View customView = layoutInflater.inflate(R.layout.pop_up_layout, null);
                popUpSave = customView.findViewById(R.id.SaveButton);
                popUpCancel = customView.findViewById(R.id.CancelButton);
                editValue = customView.findViewById(R.id.EditButtonValue);
                linearLayoutPopUpSaveData = customView.findViewById(R.id.popUpLayout);
                popupWindowSaveData = new PopupWindow(customView, LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                popupWindowSaveData.setFocusable(true);
                popupWindowSaveData.update();
                //display the popup window
                popupWindowSaveData.showAtLocation(linearLayoutPopUpSaveData, Gravity.CENTER, 0, 0);

                popUpCancel.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        popupWindowSaveData.dismiss();
                    }
                });

                popUpSave.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {

                        if (editValue.getText().length() < 1) {
                            Toast.makeText(getApplicationContext(), "Please enter Data", Toast.LENGTH_SHORT).show();
                            return;
                        }
                        LeftData = editValue.getText().toString();
                        SaveData("LeftData", LeftData);
                        Log.d(TAG, "onClick: " + LeftData);
                        popupWindowSaveData.dismiss();
                    }
                });

                return false;
            }
        });
        STOP.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String val = getData("StopData", "5");
                sendMessage(val, "STOP");
            }
        });
        STOP.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {

                Log.d(TAG, "Button Long Press STOP");
                LayoutInflater layoutInflater = (LayoutInflater) MainActivity.this.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                View customView = layoutInflater.inflate(R.layout.pop_up_layout, null);
                popUpSave = customView.findViewById(R.id.SaveButton);
                popUpCancel = customView.findViewById(R.id.CancelButton);
                editValue = customView.findViewById(R.id.EditButtonValue);
                linearLayoutPopUpSaveData = customView.findViewById(R.id.popUpLayout);
                popupWindowSaveData = new PopupWindow(customView, LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                popupWindowSaveData.setFocusable(true);
                popupWindowSaveData.update();
                //display the popup window
                popupWindowSaveData.showAtLocation(linearLayoutPopUpSaveData, Gravity.CENTER, 0, 0);

                popUpCancel.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        popupWindowSaveData.dismiss();
                    }
                });

                popUpSave.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {

                        if (editValue.getText().length() < 1) {
                            Toast.makeText(getApplicationContext(), "Please enter Data", Toast.LENGTH_SHORT).show();
                            return;
                        }
                        StopData = editValue.getText().toString();
                        SaveData("StopData", StopData);
                        Log.d(TAG, "onClick: " + StopData);
                        popupWindowSaveData.dismiss();
                    }
                });

                return false;
            }
        });

        DELETE.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                textView.setText("");
            }
        });

        SHARE.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intentShare = new Intent(Intent.ACTION_SEND);
                intentShare.setType("text/plain");
                intentShare.putExtra(Intent.EXTRA_SUBJECT, "Share BTTerminal message");
                intentShare.putExtra(Intent.EXTRA_TEXT, textView.getText().toString());
                startActivity(Intent.createChooser(intentShare, "Sharing BT Terminal"));
            }
        });

        bluetoothConnect.setOnClickListener(new View.OnClickListener() {
            @SuppressLint("MissingPermission")
            @Override
            public void onClick(View view) {
                Log.d(TAG, "Bluetooth connect button clicked");
                if (!bluetoothAdapter.isEnabled()) {
                    Toast.makeText(MainActivity.this, "Please enable buetooth", Toast.LENGTH_SHORT).show();
                    textView.append("\n--> Please enable bluetooth");
                    return;
                }
                else{
                    if (!BTConnected) {
                        if (spinnerBTPairedDevices.getSelectedItemPosition() == 0) {
                            Log.d(TAG, "onClick: Please select BT device");
                            Toast.makeText(getApplicationContext(), "Please select Bluetooth Device", Toast.LENGTH_SHORT).show();
                            return;
                        }
                        String sSelectedDevice = spinnerBTPairedDevices.getSelectedItem().toString();
                        Log.d(TAG, "Selected Device : " + sSelectedDevice);
                        for (BluetoothDevice BTDev : bluetoothDevice) {
                            if (sSelectedDevice.equals(BTDev.getName())) {
                                BTDevice = BTDev;
                                Log.d(TAG, "Selected device UUID = : " + BTDevice.getAddress());
                                cBluetoothConnect cBTConnect = new cBluetoothConnect(BTDevice);
                                cBTConnect.start();
                                BTConnected = true;
                            } else {
                                BTConnected = false;
                                bluetoothConnect.setText("CONNECT");
                            }
                        }


                    } else if (BTConnected) {
                        BTConnected = false;
                        try {
                            BTSocket.close();
                        } catch (IOException e) {
                            e.printStackTrace();
                            Log.d(TAG, "BTDisconnect : " + e.getMessage());
                        }
                        bluetoothConnect.setText("CONNECT");


                    }
                }

            }
        });
    }


    void SaveData(String button, String value) {
        SharedPreferences sharedPreferences = getSharedPreferences("db", MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putString(button, value);
        editor.apply();
    }

    String getData(String button, String defVal) {
        SharedPreferences getShared = getSharedPreferences("db", MODE_PRIVATE);
        String val = getShared.getString(button, defVal);
        return val;
    }

    @SuppressLint("MissingPermission")
    void getBluetoothPairedDevices() {
        Log.d(TAG, "getBluetoothPairedDevices: start");
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            textView.append("\n--> No Bluetooth in the phone");
            return;
        } else if (!bluetoothAdapter.isEnabled()) {
            textView.append("\n--> Please turn on bluetooth");
            return;
        }
            bluetoothDevice = bluetoothAdapter.getBondedDevices();

            Log.d(TAG, "getBluetoothPairedDevices: " + bluetoothDevice.size());
            for (BluetoothDevice BTDev : bluetoothDevice) {
                Log.d(TAG, BTDev.getName() + " ," + BTDev.getAddress());
            }
    }

    @SuppressLint("MissingPermission")
    void populateSpinnerWithPairedDevices() {
        ArrayList<String> allPairedDevices = new ArrayList<>();
        allPairedDevices.add("Select");
        if(bluetoothDevice == null){
            Toast.makeText(this, "Bluetooth is off", Toast.LENGTH_SHORT).show();
        }
        else{
            for (BluetoothDevice BTDev : bluetoothDevice) {
                allPairedDevices.add(BTDev.getName());
            }
        }
        final ArrayAdapter<String> aaPairedDevices = new ArrayAdapter<>(this, androidx.constraintlayout.widget.R.layout.support_simple_spinner_dropdown_item, allPairedDevices);
        spinnerBTPairedDevices.setAdapter(aaPairedDevices);
    }

    public class cBluetoothConnect extends Thread {
        private BluetoothDevice device;

        @SuppressLint("MissingPermission")
        public cBluetoothConnect(BluetoothDevice BTDevice) {
            Log.i(TAG, "cBluetoothConnect: start");
            device = BTDevice;
            try {
                BTSocket = device.createInsecureRfcommSocketToServiceRecord(my_uuid);

            } catch (Exception exp) {
                Log.e(TAG, "cBluetoothConnect: " + exp.getMessage());
            }


        }


        @SuppressLint("MissingPermission")
        public void run() {
            try {
                BTSocket.connect();
                Message message = Message.obtain();
                message.what = BT_STATE_CONNECTED;
                handler.sendMessage(message);
            } catch (IOException e) {
                e.printStackTrace();
                Message message = Message.obtain();
                message.what = BT_STATE_CONNECTION_FAILED;
                handler.sendMessage(message);
            }
        }

    }

//    Handler handler = new Handler(new Handler.Callback() {
//        @Override
//        public boolean handleMessage(@NonNull Message msg) {
//            switch (msg.what){
//                case BT_STATE_LISTENING:
//                    Log.d(TAG, "handleMessage: BT_STATE_LISTENING");
//                    break;
//                case BT_STATE_CONNECTING:
//                    iBTConnectionStatus = BT_CON_STATUS_CONNECTED;
//                    bluetoothConnect.setText("CONNECTING");
//                    Log.d(TAG, "handleMessage: Connecting");
//                    break;
//                case BT_STATE_CONNECTED:
//                    iBTConnectionStatus = BT_CON_STATUS_CONNECTED;
//                    Log.d(TAG, "handleMessage: BT_CON_STATUS_CONNECTED");
//                    bluetoothConnect.setText("DISCONNECT");
//                    BTConnected = true;
//                    break;
//                case BT_STATE_CONNECTION_FAILED:
//                    iBTConnectionStatus = BT_CON_STATUS_FAILED;
//                    Log.d(TAG, "handleMessage: BT_CON_STATUS_FAILED");
//                    BTConnected = false;
//                    break;
//
//            }
//            return true;
//        }
//    });

    public class classBTInitDataCommunication extends Thread {
        private final BluetoothSocket bluetoothSocket;
        private InputStream inputStream = null;
        private OutputStream outputStream = null;

        public classBTInitDataCommunication(BluetoothSocket socket) {
            Log.i(TAG, "classBTInitDataCommunication-start");

            bluetoothSocket = socket;


            try {
                inputStream = bluetoothSocket.getInputStream();
                outputStream = bluetoothSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG, "classBTInitDataCommunication-start, exp " + e.getMessage());
            }


        }

        public void run() {
            byte[] buffer = new byte[1024];
            int bytes;

            while (BTSocket.isConnected()) {
                try {
                    bytes = inputStream.read(buffer);
                    handler.obtainMessage(BT_STATE_MESSAGE_RECEIVED, bytes, -1, buffer).sendToTarget();
                } catch (IOException e) {
                    e.printStackTrace();
                    Log.e(TAG, "BT disconnect from decide end, exp " + e.getMessage());
                    iBTConnectionStatus = BT_CON_STATUS_CONNECTiON_LOST;
                    try {
                        //disconnect bluetooth
                        Log.d(TAG, "Disconnecting BTConnection");
                        if (BTSocket != null && BTSocket.isConnected()) {

                            BTSocket.close();
                        }
                        bluetoothConnect.setText("Connect");
                        BTConnected = false;
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }

                }
            }
        }

        public void write(byte[] bytes) {
            try {
                outputStream.write(bytes);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


    Handler handler = new Handler(new Handler.Callback() {
        @Override
        public boolean handleMessage(Message msg) {

            switch (msg.what) {
                case BT_STATE_LISTENING:
                    Log.d(TAG, "BT_STATE_LISTENING");
                    break;
                case BT_STATE_CONNECTING:
                    iBTConnectionStatus = BT_CON_STATUS_CONNECTING;
                    bluetoothConnect.setText("Connecting..");
                    Log.d(TAG, "BT_STATE_CONNECTING");
                    break;
                case BT_STATE_CONNECTED:

                    iBTConnectionStatus = BT_CON_STATUS_CONNECTED;

                    Log.d(TAG, "BT_CON_STATUS_CONNECTED");
                    bluetoothConnect.setText("Disconnect");

                    cBTInitSendReceive = new classBTInitDataCommunication(BTSocket);
                    cBTInitSendReceive.start();

                    BTConnected = true;
                    break;
                case BT_STATE_CONNECTION_FAILED:
                    iBTConnectionStatus = BT_CON_STATUS_FAILED;
                    Log.d(TAG, "BT_STATE_CONNECTION_FAILED");
                    BTConnected = false;
                    break;

                case BT_STATE_MESSAGE_RECEIVED:
                    byte[] readBuff = (byte[]) msg.obj;
                    String tempMsg = new String(readBuff, 0, msg.arg1);
                    Log.d(TAG, "Message receive ( " + tempMsg.length() + " )  data : " + tempMsg);

                    textView.append(tempMsg);


                    break;

            }
            return true;
        }
    });


    public void sendMessage(String sMessage, String name) {
        if (BTSocket != null && iBTConnectionStatus == BT_CON_STATUS_CONNECTED) {
            if (BTSocket.isConnected()) {
                try {
                    cBTInitSendReceive.write(sMessage.getBytes());
                    textView.append("\r\n--> " + name);
                } catch (Exception exp) {

                }
            }
        } else {
            Toast.makeText(getApplicationContext(), "Please connect to bluetooth", Toast.LENGTH_SHORT).show();
            textView.append("\r\n --> Please select device and connect.");
        }

    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume: start");
        super.onResume();
        getBluetoothPairedDevices();
        populateSpinnerWithPairedDevices();
    }

}



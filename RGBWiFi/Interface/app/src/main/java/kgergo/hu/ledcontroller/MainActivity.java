package kgergo.hu.ledcontroller;

//package com.example.application.colorpicker;

import android.app.VoiceInteractor;
import android.graphics.ColorSpace;
import android.support.constraint.ConstraintLayout;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import yuku.ambilwarna.AmbilWarnaDialog;

public class MainActivity extends AppCompatActivity {
    ConstraintLayout mLayout;
    int mDefaultColor;
    Button mButton;
    String IP = "192.168.0.103";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mLayout = (ConstraintLayout) findViewById(R.id.layout);
        mDefaultColor = ContextCompat.getColor(MainActivity.this, R.color.colorPrimary);
        mButton = (Button) findViewById(R.id.button);
        mButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openColorPicker();
            }
        });

        Button btnRainbow = (Button) findViewById(R.id.btnRainbow);
        btnRainbow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                    Rainbow();
            }
        });

        Button btnWave = (Button) findViewById(R.id.btnWave);
        btnWave.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Wave();
            }
        });

        Button btnTurnOff = (Button)findViewById(R.id.btnTurnOff);
        btnTurnOff.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                TurnOffLeds();
            }
        });



        Button btnSetIP = (Button)findViewById(R.id.btnSetIP);
        btnSetIP.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                EditText txtIP = findViewById(R.id.txtIP);
                String temp = txtIP.getText().toString();
                if(!temp.isEmpty()){
                    IP = temp;
                }
                else{
                    Toast.makeText(getApplicationContext(), "Invalid IP Address",Toast.LENGTH_LONG).show();
                }
            }
        });

        final EditText txtIP = findViewById(R.id.txtIP);
        txtIP.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                txtIP.setText("");
            }
        });
    }

    public void openColorPicker() {
        AmbilWarnaDialog colorPicker = new AmbilWarnaDialog(this, mDefaultColor, new AmbilWarnaDialog.OnAmbilWarnaListener() {
            @Override
            public void onCancel(AmbilWarnaDialog dialog) {

            }

            @Override
            public void onOk(AmbilWarnaDialog dialog, int color) {
                mDefaultColor = color;
                mLayout.setBackgroundColor(mDefaultColor);
                setColor(color);
            }
        });
        colorPicker.show();
    }

    public void setColor(int color){

        int argb = color;
        int red = 0xFF & ( argb >> 16);
        int blue = 0xFF & (argb >> 0 );
        int green = 0xFF & (argb >> 8 );
        //int alpha = 0xFF & (argb >> 24);

        OkHttpClient client = new OkHttpClient();
        String url = "http://"+ IP + "/setleds?r=" + red + "&g=" + green +"&b=" + blue + "&fade=1000";
        Request request = new Request.Builder().url(url).build();
        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                //Toast.makeText(getApplicationContext(), "Color setting failed!",Toast.LENGTH_LONG).show();
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                //Toast.makeText(getApplicationContext(), "Color set!",Toast.LENGTH_LONG).show();
            }
        });
    }

    public void Rainbow(){
        OkHttpClient client = new OkHttpClient();
        String url = "http://"+ IP + "/rainbow?fade=3000";
        Request request = new Request.Builder().url(url).build();
        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                //Toast.makeText(getApplicationContext(), "Rainbow effect failed!",Toast.LENGTH_LONG).show();
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                //Toast.makeText(getApplicationContext(), "Rainbow effect started!",Toast.LENGTH_LONG).show();
            }
        });
    }

    public void Wave(){
        OkHttpClient client = new OkHttpClient();
        String url = "http://"+ IP + "/wave?r=255&g=32&b=10&fade=5000";
        Request request = new Request.Builder().url(url).build();
        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                //Toast.makeText(getApplicationContext(), "Wave effect failed!",Toast.LENGTH_LONG).show();
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                //Toast.makeText(getApplicationContext(), "Wave effect started!",Toast.LENGTH_LONG).show();
            }
        });
    }

    public void TurnOffLeds(){
        OkHttpClient client = new OkHttpClient();
        String url = "http://"+ IP + "/ledsoff?fade=500";
        Request request = new Request.Builder().url(url).build();
        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                //Toast.makeText(getApplicationContext(), "Turning off failed!",Toast.LENGTH_LONG).show();
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                //Toast.makeText(getApplicationContext(), "Leds are turned off!",Toast.LENGTH_LONG).show();
            }
        });
    }
}
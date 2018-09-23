package com.people.health.player;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements PeoplePlayer.OnPrepareListener {

    private SurfaceView video_cavas;
    PeoplePlayer mPeoplePlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        video_cavas = findViewById(R.id.video_cavas);
        mPeoplePlayer = new PeoplePlayer().setOnPrepareListener(this);
        mPeoplePlayer.setSurfaceView(video_cavas);
        mPeoplePlayer.setDataSource("http://60.10.58.20:6713/mag/hls/a7a94c27826b4cafa7a6711e3d3bb41d/1/live.m3u8");
//        native_string();
//        Log.d("native_string", "onCreate: "+native_string());
    }

    public void start(View view) {
        mPeoplePlayer.prepare();
    }

    public native String native_string();

    @Override
    public void onPrepare() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(MainActivity.this,"视频准备好了",0).show();
            }
        });
        mPeoplePlayer.start();
    }
}

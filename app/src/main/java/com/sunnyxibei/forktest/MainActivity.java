package com.sunnyxibei.forktest;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.sdsmdg.tastytoast.TastyToast;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("JniTest");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        String stringFromJni = getStringFromJni();
        if (null != stringFromJni) {
            TastyToast.makeText(this,stringFromJni,TastyToast.LENGTH_SHORT,TastyToast.DEFAULT);
            TastyToast.makeText(this,"交叉编译出的.so动态链接库已经成功运行！",TastyToast.LENGTH_LONG,TastyToast.SUCCESS);
        }
        getFork();
    }

    public native void getFork();

    public native String getStringFromJni();
}

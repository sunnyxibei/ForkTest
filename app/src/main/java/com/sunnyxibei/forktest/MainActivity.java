package com.sunnyxibei.forktest;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import com.sdsmdg.tastytoast.TastyToast;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";
    private static final int MY_PERMISSIONS_REQUEST_READ_CONTACTS = 100;

    static {
        System.loadLibrary("JniTest");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Android6.0权限动态适配
        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
                    MY_PERMISSIONS_REQUEST_READ_CONTACTS);
        } else if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    MY_PERMISSIONS_REQUEST_READ_CONTACTS);
        }


        String stringFromJni = getStringFromJni();
        if (null != stringFromJni) {
            TastyToast.makeText(this, stringFromJni, TastyToast.LENGTH_SHORT, TastyToast.DEFAULT);
            TastyToast.makeText(this, "交叉编译出的.so动态链接库已经成功运行！", TastyToast.LENGTH_LONG, TastyToast.SUCCESS);
        }
        getFork();
    }

    public void openBrowser(View view) {
        String PATH = Environment.getExternalStorageDirectory().getAbsolutePath();
        final String oldApkPath = PATH + File.separator + "Weibo5.5.apk";
        final String newApkPath = PATH + File.separator + "Weibo5.6.apk";
        final String patchPath = PATH + File.separator + "weibo.patch";
        new Thread(new Runnable() {
            @Override
            public void run() {
                int patch = patch(oldApkPath, newApkPath, patchPath);
                Log.e(TAG, "openBrowser: " + patch);
//                int diff = diff(oldApkPath, newApkPath, patchPath);
//                Log.e(TAG, "openBrowser: " + diff);
            }
        }).start();
    }

    public native void getFork();

    public native String getStringFromJni();

    public native void testExeclp();

    public static native int patch(String oldApkPath, String newApkPath, String patchPath);

    public static native int diff(String oldApkPath, String newApkPath, String patchPath);
}

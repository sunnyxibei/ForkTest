package com.sunnyxibei.forktest;

import android.app.Activity;
import android.os.Bundle;

import com.sdsmdg.tastytoast.TastyToast;

/**
 * Created by jiayuanbin on 2016/10/28.
 */

public class NewActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TastyToast.makeText(this,"NewActivity",TastyToast.LENGTH_LONG,TastyToast.INFO);
    }
}

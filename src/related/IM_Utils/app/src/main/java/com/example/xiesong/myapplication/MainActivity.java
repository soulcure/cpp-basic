package com.example.xiesong.myapplication;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Iterator;

import proto.youmai.com.IMContentType;
import proto.youmai.com.IMContentUtil;


public class MainActivity extends ActionBarActivity {


    private void Test(){


        IMContentUtil imContentUtil = new IMContentUtil();
        imContentUtil.appendText("hello text info.");
        imContentUtil.appendImage("http://www.bing.com/logo.png");
        imContentUtil.appendAT("@121212121");
        imContentUtil.appendText("here is second part of string");
        System.out.println(imContentUtil.serializeToString());


        IMContentUtil parser = new IMContentUtil(imContentUtil.serializeToString());
        parser.parseBody();
        IMContentType _tType ;
        while ( ( _tType = parser.hasNext() )!=null ) {
            System.out.println( "print info"+ _tType.toString() + ":" + parser.readNext());
        }
        System.out.println("print finished.打印结束");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Test();
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}

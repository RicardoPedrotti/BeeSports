package com.example.pedropifre.logregapp;

import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.app.Activity;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.listener.OnChartValueSelectedListener;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

public class graficodelinha extends AppCompatActivity {
    private ProgressDialog pd;

    ArrayList<LineDataSet> yAxis;
    ArrayList<BarEntry> yValues;
    ArrayList<String> xAxis1;
    BarEntry values ;
    LineChart chart;



    BarData data;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_line_chart);

        pd = new ProgressDialog(graficodelinha.this);
        pd.setMessage("loading");


        // Log.d("array",Arrays.toString(fullData));
        chart = (LineChart) findViewById(R.id.linechar);
        load_data_from_server();



    }
    public void load_data_from_server() {
        pd.show();
        String url = "http://54.94.143.174/login_dados2.php";
        xAxis1 = new ArrayList<>();
        yAxis = null;
        yValues = new ArrayList<>();


        StringRequest stringRequest = new StringRequest(Request.Method.POST,
                url,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        Log.d("string",response);

                        try {

                            JSONArray jsonarray = new JSONArray(response);

                            for(int i=0; i < jsonarray.length(); i++) {

                                JSONObject jsonobject = jsonarray.getJSONObject(i);


                                String score = jsonobject.getString("DADO2").trim();
                                String name = jsonobject.getString("").trim();

                                xAxis1.add(name);

                                values = new BarEntry(Float.valueOf(score),i);
                                yValues.add(values);

                            }
                        } catch (JSONException e) {
                            e.printStackTrace();


                        }







                        yAxis = new ArrayList<>();
                        String names[]= xAxis1.toArray(new String[xAxis1.size()]);

                        chart.setDescription("");
                        chart.animateXY(2000, 2000);
                        chart.invalidate();
                        pd.hide();
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        if(error != null){

                            Toast.makeText(getApplicationContext(), "Something went wrong.", Toast.LENGTH_LONG).show();
                            pd.hide();
                        }
                    }
                }

        );

        MySingleton.getInstance(getApplicationContext()).addToRequestQueue(stringRequest);

    }


}

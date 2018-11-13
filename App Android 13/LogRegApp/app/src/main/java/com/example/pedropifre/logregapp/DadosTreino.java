package com.example.pedropifre.logregapp;

import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

public class DadosTreino extends AppCompatActivity {
    private ProgressDialog pd;

    ArrayList<BarDataSet> yAxis;
    ArrayList<BarEntry> yValues;
    ArrayList<String> xAxis1;
    BarEntry values ;
    BarChart chart;
    TextView teste;


    BarData data;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_dados_treino);
        teste = (TextView)findViewById(R.id.txt_nome);
        pd = new ProgressDialog(DadosTreino.this);
        pd.setMessage("loading");


        // Log.d("array",Arrays.toString(fullData));
        chart = (BarChart) findViewById(R.id.chart);
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

                            JSONObject objeto = jsonarray.getJSONObject(1);

                            String nome_usuario = objeto.getString("nome").trim();
                            teste.setText("BPM de "+nome_usuario +"");


                            for(int i=0; i < jsonarray.length(); i++) {

                                JSONObject jsonobject = jsonarray.getJSONObject(i);


                                String score = jsonobject.getString("DADO1").trim();
                                String name = jsonobject.getString("DADO3").trim();
                                xAxis1.add(name);

                                values = new BarEntry(Float.valueOf(score),i);
                                yValues.add(values);

                            }
                        } catch (JSONException e) {
                            e.printStackTrace();


                        }



                        BarDataSet barDataSet1 = new BarDataSet(yValues, "Km corridos" +
                                "");
                        barDataSet1.setColor(Color.rgb(255, 157, 24));



                        yAxis = new ArrayList<>();
                        yAxis.add(barDataSet1);
                        String names[]= xAxis1.toArray(new String[xAxis1.size()]);
                        data = new BarData(names,yAxis);

                        chart.setData(data);
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

    public void voltar2(View view) {
        Intent intent = new Intent(DadosTreino.this, comecar_treino.class);
        startActivity(intent);
    }
}

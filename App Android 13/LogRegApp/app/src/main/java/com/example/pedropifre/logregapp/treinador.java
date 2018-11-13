package com.example.pedropifre.logregapp;


import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import com.github.mikephil.charting.data.BarData;


public class treinador extends AppCompatActivity {
    TextView name;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_treinador);




        // Log.d("array",Arrays.toString(fullData));



        name = (TextView)findViewById(R.id.name);
        Bundle bundle = getIntent().getExtras();

        name.setText("Bem-vindo professor "+bundle.getString("name2")+", o que deseja fazer?");


    }
    public void sendMessage(View view) {
        Intent intent = new Intent(treinador.this, comecar_treino.class);
        startActivity(intent);
    }

}
package com.example.pedropifre.logregapp;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;

public class comecar_treino extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_comecar_treino);
    }
    public void sendMessage(View view) {
        Intent intent = new Intent(comecar_treino.this, escolha_jogador.class);
        startActivity(intent);
    }

    public void voltar(View view) {
        Intent intent = new Intent(comecar_treino.this, treinador.class);
        startActivity(intent);
    }


}

package com.example.pedropifre.logregapp;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;

public class escolha_jogador extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_escolha_jogador);
    }
    public void ataque(View view) {
        Intent intent = new Intent(escolha_jogador.this, DadosTreino.class);
        startActivity(intent);
    }
    public void defesa(View view) {
        Intent intent = new Intent(escolha_jogador.this, graficodelinha.class);
        startActivity(intent);
    }
}

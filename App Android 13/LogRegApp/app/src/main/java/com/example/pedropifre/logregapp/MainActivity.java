package com.example.pedropifre.logregapp;

import android.content.DialogInterface;
import android.content.Intent;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

public class MainActivity extends AppCompatActivity {
    TextView textView;
    Button login_button;
    EditText UserName,Password;
    String username, password;
    String login_url ="http://54.94.143.174/login.php";
    AlertDialog.Builder builder;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        builder = new AlertDialog.Builder(MainActivity.this);
        login_button = (Button)findViewById(R.id.bn_login);
        UserName = (EditText)findViewById(R.id.login_name);
        Password = (EditText)findViewById(R.id.login_password);
        login_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                username = UserName.getText().toString();
                    password = Password.getText().toString();


                if (username.equals("")||password.equals(""))
                {
                    builder.setTitle("Login invalido");
                    displayAlert("Usuário e senha não");

                }
                else{
                    StringRequest stringRequest = new StringRequest(Request.Method.POST, login_url,
                            new Response.Listener<String>() {
                                @Override
                                public void onResponse(String response) {
                                    try {
                                        JSONArray jsonArray = new JSONArray(response);
                                        JSONObject jsonObject = jsonArray.getJSONObject(0);
                                        String code = jsonObject.getString("code");
                                        if (code.equals("login_failed")){
                                            builder.setTitle("Login Error...");
                                            displayAlert(jsonObject.getString("message"));
                                        }
                                        else{
                                            Intent intent = new Intent(MainActivity.this, treinador.class);
                                            Bundle bundle = new Bundle();
                                            bundle.putString("name2", jsonObject.getString("name2"));

                                            intent.putExtras(bundle);
                                            startActivity(intent);

                                        }
                                    } catch (JSONException e) {
                                        e.printStackTrace();
                                    }
                                }
                            }, new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            Toast.makeText(MainActivity.this, "Error",Toast.LENGTH_LONG).show();
                            error.printStackTrace();
                        }
                    })
                    {
                        @Override
                        protected Map<String, String> getParams() throws AuthFailureError {
                            Map<String, String> params = new HashMap<String, String>();
                            params.put("username", username);
                            params.put("password", password);

                            return params;
                        }
                    };
                    MySingleton.getInstance(MainActivity.this).addToRequestQueue(stringRequest);
                }
            }
        });
    }
    public void displayAlert(String message){
        builder.setMessage(message);
        builder.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                UserName.setText("");
                Password.setText("");
            }
        });
        AlertDialog alertDialog = builder.create();
        alertDialog.show();
    }

    public void teste2(View view) {
        Intent intent = new Intent(MainActivity.this, treinador.class);
        startActivity(intent);
    }
 }

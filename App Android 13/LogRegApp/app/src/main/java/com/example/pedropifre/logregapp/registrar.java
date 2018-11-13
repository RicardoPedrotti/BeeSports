package com.example.pedropifre.logregapp;

import android.content.DialogInterface;
import android.os.Bundle;
import android.os.StrictMode;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class registrar extends AppCompatActivity {
    Button reg_bn;
    EditText Name,Email,UserName,Password,ConPassword;
    String name,email,username,password,conpass;
    HttpURLConnection urlConnection=null;
    AlertDialog.Builder builder;
    String reg_url = "";
    Spinner spinner1;
    InputStream is=null;
    String result=null;
    String line=null;

    String[] nome;








    @Override
    protected void onCreate(Bundle savedInstanceState) {
        StrictMode.ThreadPolicy policy = new
        StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registrar);


        reg_bn = (Button) findViewById(R.id.bn_reg);
        Name = (EditText)findViewById(R.id.reg_name);
        Email = (EditText)findViewById(R.id.reg_email);
        UserName = (EditText)findViewById(R.id.reg_user_name);
        Password = (EditText)findViewById(R.id.reg_password);
        ConPassword = (EditText)findViewById(R.id.reg_con_password);
        builder = new AlertDialog.Builder(registrar.this);
        spinner1 =(Spinner) findViewById(R.id.spinner1);



        /** spinner code*/
        final List<String> list1 = new ArrayList<String>();

        try {
            URL url = new URL("http://172.31.6.250/demo_spinner.php");
            urlConnection = (HttpURLConnection) url.openConnection();
            urlConnection.connect();
            is = urlConnection.getInputStream();
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            Log.e("Falhou parça", e.toString());
            Toast.makeText(getApplicationContext(),"endereço de ip invalido", Toast.LENGTH_LONG).show();
            finish();
        }
        try{
            BufferedReader reader = new BufferedReader(new InputStreamReader(is, "iso-8859-1"),8);
            StringBuilder sb = new StringBuilder();
            while ((line = reader.readLine()) != null)
            {
                sb.append(line+"\n");
            }
            is.close();
            result = sb.toString();
        }
        catch (Exception e){
            Log.e("Falhou", e.toString());
        }

        try {
            JSONArray JA=new JSONArray(result);
            JSONObject json = null;

            nome = new String[JA.length()];

            for (int i = 0; i<JA.length(); i++)
            {
               json = JA.getJSONObject(i);
               nome[i]=json.getString("nome");
            }

            Toast.makeText(getApplicationContext(), "sss",Toast.LENGTH_LONG).show();

            for (int i=0;i<nome.length;i++)
            {
                list1.add(nome[i]);
            }

            Toast.makeText(getApplicationContext(),"len", Toast.LENGTH_LONG).show();

            spinner_fn();
        }
        catch (Exception e){
            Log.e("Falhou",e.toString());
        }


        reg_bn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                name = Name.getText().toString();
                email = Email.getText().toString();
                username = UserName.getText().toString();
                password = Password.getText().toString();
                conpass = ConPassword.getText().toString();

                if (name.equals("")||email.equals("")||username.equals("")||password.equals("")||conpass.equals(""))
                {
                    builder.setTitle("deu ruim");
                        builder.setMessage("preenche esses negocios");
                        displayAlert("input_error");
                }
                    else{
                    if (!(password.equals(conpass)))
                    {
                        builder.setTitle("deu ruim");
                        builder.setMessage("nao deu match ai");
                        displayAlert("input_error");
                    }
                    else{
                        StringRequest stringRequest = new StringRequest(Request.Method.POST, reg_url,
                                new Response.Listener<String>() {
                                    @Override
                                    public void onResponse(String response) {
                                        try {
                                            JSONArray jsonArray = new JSONArray(response);
                                            JSONObject jsonObject = jsonArray.getJSONObject(0);
                                            String code = jsonObject.getString("code");
                                            String message = jsonObject.getString("message");
                                            builder.setTitle("server response");
                                            builder.setMessage(message);


                                            displayAlert(code);
                                        } catch (JSONException e) {
                                            e.printStackTrace();
                                        }
                                    }
                                }, new Response.ErrorListener() {
                            @Override
                            public void onErrorResponse(VolleyError error) {

                            }
                        }){
                            @Override
                            protected Map<String, String> getParams() throws AuthFailureError {
                                Map<String, String> params = new HashMap<String, String>();
                                params.put("name",name);
                                params.put("email",email);
                                params.put("user_name",username);
                                params.put("password",password);

                                return params;
                            }
                        };
                        MySingleton.getInstance(registrar.this).addToRequestQueue(stringRequest);
                    }
                }
            }

        });



    }
    public void spinner_fn()
    {
        ArrayAdapter<String> dataAdapter1 = new ArrayAdapter<String>(registrar.this,
                android.R.layout.simple_spinner_item, nome);
        spinner1.setAdapter(dataAdapter1);

        spinner1.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                spinner1.setSelection(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }
        public void displayAlert(final String code)
        {
            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialogInterface, int i) {
                     if(code.equals("input_error"))
                     {
                         Password.setText("");
                         ConPassword.setText("");
                     }
                     else if (code.equals("reg_success")){
                         finish();
                     }
                     else if (code.equals("reg_failed")){
                         Name.setText("");
                         Email.setText("");
                         UserName.setText("");
                         Password.setText("");
                         ConPassword.setText("");

                     }
                }
            });
            AlertDialog alertDialog = builder.create();
            alertDialog.show();
        }
}

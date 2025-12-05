package com.elitemagic.calculadorawifi;

import android.graphics.Typeface;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;

public class MainActivity extends AppCompatActivity {

    private TextView tvDisplay;
    private TextView tvConvertidor;
    private String currentInput = "";
    private String operator = "";
    private double firstValue = 0;
    private boolean isNewOperation = true;

    // Variables para el modo mágico
    private boolean magicModeActive = false;
    private Handler longPressHandler = new Handler();
    private Runnable longPressRunnable;
    private static final long LONG_PRESS_DURATION = 2000; // 2 segundos

    // Dirección IP del ESP32 (la red WiFi que crea el ESP32)
    private static final String ESP32_IP = "192.168.4.1";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvDisplay = findViewById(R.id.tvDisplay);
        tvConvertidor = findViewById(R.id.tvConvertidor);

        setupNumberButtons();
        setupOperationButtons();
        setupConvertidorLongPress();
    }

    private void setupConvertidorLongPress() {
        longPressRunnable = new Runnable() {
            @Override
            public void run() {
                toggleMagicMode();
            }
        };

        tvConvertidor.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        longPressHandler.postDelayed(longPressRunnable, LONG_PRESS_DURATION);
                        return true;
                    case MotionEvent.ACTION_UP:
                    case MotionEvent.ACTION_CANCEL:
                        longPressHandler.removeCallbacks(longPressRunnable);
                        return true;
                }
                return false;
            }
        });
    }

    private void toggleMagicMode() {
        magicModeActive = !magicModeActive;

        if (magicModeActive) {
            // Modo mágico activado: blanco y negrita
            tvConvertidor.setTextColor(0xFFFFFFFF);
            tvConvertidor.setTypeface(null, Typeface.BOLD);
            Toast.makeText(this, "🎴 Modo Mágico Activado\nPalo: 1=♥ 2=♠ 3=♣ 4=♦\nNúmero: 1-13", Toast.LENGTH_LONG).show();
            clearDisplay();
        } else {
            // Modo normal: gris y normal
            tvConvertidor.setTextColor(0xFF888888);
            tvConvertidor.setTypeface(null, Typeface.NORMAL);
            Toast.makeText(this, "Modo Normal", Toast.LENGTH_SHORT).show();
            clearDisplay();
        }
    }

    private void setupNumberButtons() {
        int[] numberButtonIds = {
            R.id.btn0, R.id.btn1, R.id.btn2, R.id.btn3, R.id.btn4,
            R.id.btn5, R.id.btn6, R.id.btn7, R.id.btn8, R.id.btn9
        };

        View.OnClickListener numberClickListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Button button = (Button) v;
                String number = button.getText().toString();

                if (magicModeActive) {
                    // En modo mágico, solo permitir máximo 3 dígitos (ej: 113)
                    if (currentInput.length() < 3) {
                        if (isNewOperation) {
                            currentInput = number;
                            isNewOperation = false;
                        } else {
                            currentInput += number;
                        }
                        tvDisplay.setText(currentInput);
                    }
                } else {
                    // Modo calculadora normal
                    if (isNewOperation) {
                        currentInput = number;
                        isNewOperation = false;
                    } else {
                        if (currentInput.equals("0") && !number.equals(".")) {
                            currentInput = number;
                        } else {
                            currentInput += number;
                        }
                    }
                    tvDisplay.setText(currentInput);
                }
            }
        };

        for (int id : numberButtonIds) {
            findViewById(id).setOnClickListener(numberClickListener);
        }

        // Botón decimal
        findViewById(R.id.btnDot).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!magicModeActive && !currentInput.contains(".")) {
                    currentInput += ".";
                    tvDisplay.setText(currentInput);
                }
            }
        });
    }

    private void setupOperationButtons() {
        // Botón Clear
        findViewById(R.id.btnClear).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                clearDisplay();
            }
        });

        // Botón Backspace
        findViewById(R.id.btnBackspace).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (currentInput.length() > 0) {
                    currentInput = currentInput.substring(0, currentInput.length() - 1);
                    if (currentInput.isEmpty()) {
                        currentInput = "0";
                        isNewOperation = true;
                    }
                    tvDisplay.setText(currentInput);
                }
            }
        });

        // Botón Igual
        findViewById(R.id.btnEquals).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (magicModeActive) {
                    // Modo mágico: enviar carta al ESP32
                    sendCardToESP32();
                } else {
                    // Modo normal: calcular resultado
                    calculateResult();
                }
            }
        });

        // Operadores matemáticos (solo en modo normal)
        setupMathOperator(R.id.btnAdd, "+");
        setupMathOperator(R.id.btnSubtract, "-");
        setupMathOperator(R.id.btnMultiply, "×");
        setupMathOperator(R.id.btnDivide, "÷");
        setupMathOperator(R.id.btnPercent, "%");
    }

    private void setupMathOperator(int buttonId, final String op) {
        findViewById(buttonId).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!magicModeActive && !currentInput.isEmpty()) {
                    firstValue = Double.parseDouble(currentInput);
                    operator = op;
                    isNewOperation = true;
                }
            }
        });
    }

    private void calculateResult() {
        if (operator.isEmpty() || currentInput.isEmpty()) return;

        double secondValue = Double.parseDouble(currentInput);
        double result = 0;

        switch (operator) {
            case "+":
                result = firstValue + secondValue;
                break;
            case "-":
                result = firstValue - secondValue;
                break;
            case "×":
                result = firstValue * secondValue;
                break;
            case "÷":
                if (secondValue != 0) {
                    result = firstValue / secondValue;
                } else {
                    Toast.makeText(this, "Error: División por cero", Toast.LENGTH_SHORT).show();
                    clearDisplay();
                    return;
                }
                break;
            case "%":
                result = firstValue % secondValue;
                break;
        }

        currentInput = String.valueOf(result);
        if (currentInput.endsWith(".0")) {
            currentInput = currentInput.substring(0, currentInput.length() - 2);
        }
        tvDisplay.setText(currentInput);
        operator = "";
        isNewOperation = true;
    }

    private void sendCardToESP32() {
        if (currentInput.isEmpty() || currentInput.equals("0")) {
            Toast.makeText(this, "Introduce una combinación válida", Toast.LENGTH_SHORT).show();
            return;
        }

        // Parsear la combinación
        // Formato: XYZ donde X = palo (1-4), YZ = número (1-13)
        int combination = 0;
        try {
            combination = Integer.parseInt(currentInput);
        } catch (NumberFormatException e) {
            Toast.makeText(this, "Combinación inválida", Toast.LENGTH_SHORT).show();
            return;
        }

        // Extraer palo y número
        int suit = combination / 100; // Primer dígito
        int cardNumber = combination % 100; // Últimos dos dígitos

        // Validar
        if (suit < 1 || suit > 4) {
            Toast.makeText(this, "Palo inválido (1-4)\n1=♥ 2=♠ 3=♣ 4=♦", Toast.LENGTH_LONG).show();
            return;
        }

        if (cardNumber < 1 || cardNumber > 13) {
            Toast.makeText(this, "Número inválido (1-13)\n1=As, 11=J, 12=Q, 13=K", Toast.LENGTH_LONG).show();
            return;
        }

        // Convertir a carta
        String cardString = getCardString(suit, cardNumber);

        Toast.makeText(this, "Enviando: " + cardString, Toast.LENGTH_SHORT).show();

        // Enviar al ESP32 en segundo plano
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    String urlString = "http://" + ESP32_IP + "/startTransmission?cards=" +
                                     URLEncoder.encode(cardString, "UTF-8") + "&count=1";

                    URL url = new URL(urlString);
                    HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                    connection.setRequestMethod("GET");
                    connection.setConnectTimeout(5000);
                    connection.setReadTimeout(5000);

                    int responseCode = connection.getResponseCode();

                    if (responseCode == HttpURLConnection.HTTP_OK) {
                        BufferedReader reader = new BufferedReader(
                            new InputStreamReader(connection.getInputStream()));
                        StringBuilder response = new StringBuilder();
                        String line;
                        while ((line = reader.readLine()) != null) {
                            response.append(line);
                        }
                        reader.close();

                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(MainActivity.this,
                                    "✅ Carta enviada: " + cardString,
                                    Toast.LENGTH_LONG).show();
                                clearDisplay();
                            }
                        });
                    } else {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(MainActivity.this,
                                    "❌ Error al enviar (Código: " + responseCode + ")",
                                    Toast.LENGTH_LONG).show();
                            }
                        });
                    }
                    connection.disconnect();

                } catch (Exception e) {
                    e.printStackTrace();
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(MainActivity.this,
                                "❌ Error de conexión\n¿Conectado a Mm_wifi?",
                                Toast.LENGTH_LONG).show();
                        }
                    });
                }
            }
        }).start();
    }

    private String getCardString(int suit, int cardNumber) {
        // Convertir número a rango
        String rank;
        switch (cardNumber) {
            case 1:
                rank = "A";
                break;
            case 11:
                rank = "J";
                break;
            case 12:
                rank = "Q";
                break;
            case 13:
                rank = "K";
                break;
            default:
                rank = String.valueOf(cardNumber);
                break;
        }

        // Convertir palo a símbolo
        String suitSymbol;
        switch (suit) {
            case 1:
                suitSymbol = "♥"; // Corazones
                break;
            case 2:
                suitSymbol = "♠"; // Picas
                break;
            case 3:
                suitSymbol = "♣"; // Tréboles
                break;
            case 4:
                suitSymbol = "♦"; // Diamantes
                break;
            default:
                suitSymbol = "?";
                break;
        }

        return rank + " " + suitSymbol;
    }

    private void clearDisplay() {
        currentInput = "0";
        operator = "";
        firstValue = 0;
        isNewOperation = true;
        tvDisplay.setText(currentInput);
    }
}

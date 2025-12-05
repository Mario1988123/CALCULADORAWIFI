# 🎴 Calculadora WiFi - Magic Card Trick

Una calculadora Android con un truco mágico secreto que se comunica con un ESP32 para mostrar cartas en las redes WiFi.

## 📱 Características

### Modo Normal
- Calculadora completa y funcional
- Operaciones básicas: suma, resta, multiplicación, división, porcentaje
- Interfaz similar a la calculadora de iOS

### Modo Mágico 🎩
- **Activación**: Mantén presionado "Convertidor" durante 2 segundos
- **Indicador**: El texto "Convertidor" se vuelve blanco y en negrita cuando está activo
- **Función**: Envía cartas al ESP32 para mostrarlas como redes WiFi

## 🎯 Cómo Usar el Modo Mágico

### 1. Conectar al ESP32
- Conecta tu móvil a la red WiFi: **Mm_wifi**
- Contraseña: **12345678**

### 2. Activar Modo Mágico
- Mantén presionado "Convertidor" 2 segundos
- Verás que cambia a blanco y negrita

### 3. Introducir la Combinación
La combinación es de 3 dígitos: **XYZ**

- **X** = Palo (primer dígito):
  - 1 = ♥ Corazones
  - 2 = ♠ Picas
  - 3 = ♣ Tréboles
  - 4 = ♦ Diamantes

- **YZ** = Número de carta (2 últimos dígitos):
  - 01 = As
  - 02-10 = Números
  - 11 = J (Jota)
  - 12 = Q (Reina)
  - 13 = K (Rey)

### 4. Ejemplos de Combinaciones

| Combinación | Carta Resultante |
|------------|------------------|
| 101 | A ♥ (As de Corazones) |
| 112 | Q ♥ (Reina de Corazones) |
| 113 | K ♥ (Rey de Corazones) |
| 207 | 7 ♠ (7 de Picas) |
| 311 | J ♣ (Jota de Tréboles) |
| 406 | 6 ♦ (6 de Diamantes) |

### 5. Enviar al ESP32
- Introduce la combinación (ej: 112)
- Pulsa el botón **=**
- La carta aparecerá 30 veces en las listas de WiFi de los espectadores

## 🔧 Instalación

### Requisitos
- Android Studio
- Android SDK 24 o superior
- Dispositivo Android físico o emulador

### Pasos
1. Clona el repositorio:
```bash
git clone https://github.com/Mario1988123/CALCULADORAWIFI.git
```

2. Abre el proyecto en Android Studio

3. Sincroniza Gradle

4. Conecta tu dispositivo Android o inicia un emulador

5. Ejecuta la aplicación

## 🎭 Funcionamiento del Truco

1. El mago conecta su móvil al ESP32 (red Mm_wifi)
2. Activa el modo mágico (long press en Convertidor)
3. Un espectador elige una carta
4. El mago introduce discretamente la combinación (ej: 112 para Q♥)
5. Pulsa =
6. El ESP32 crea 30 redes WiFi con el nombre de esa carta usando caracteres invisibles
7. Los espectadores ven la carta "aparecer mágicamente" en sus listas de WiFi

## 🛠️ Configuración del ESP32

El código del ESP32 debe estar ejecutándose con:
- SSID del AP: **Mm_wifi**
- Contraseña: **12345678**
- IP Local: **192.168.4.1**

## 📡 Comunicación

La app se comunica con el ESP32 mediante HTTP GET:
```
http://192.168.4.1/startTransmission?cards=Q%20%E2%99%A5&count=1
```

## 🎨 Interfaz

- **Fondo negro** estilo iOS
- **Botones grises oscuros** con esquinas redondeadas
- **Botón igual naranja** (#FF9500)
- **Display blanco** grande y claro
- **Indicador visual** del modo mágico en "Convertidor"

## 🔐 Permisos

La app requiere:
- `INTERNET` - Para comunicarse con el ESP32
- `ACCESS_WIFI_STATE` - Para verificar la conexión WiFi
- `ACCESS_NETWORK_STATE` - Para verificar el estado de la red

## 📝 Notas

- El modo mágico solo permite números (no operaciones matemáticas)
- Máximo 3 dígitos en modo mágico
- Las combinaciones inválidas muestran un mensaje de error
- Si no estás conectado a Mm_wifi, aparecerá un error de conexión

## 👨‍💻 Desarrollador

**Elitemagic**

## 📄 Licencia

Este proyecto es de código abierto para fines educativos y de entretenimiento.

---

**¡Disfruta del truco! 🎩✨**

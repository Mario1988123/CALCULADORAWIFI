# 📱 Cómo Descargar e Instalar la APK

## 🎯 Repositorio Actual

📂 **Repositorio:** `Mario1988123/CALCULADORAWIFI`
🌿 **Rama:** `claude/add-magic-tricks-01H9Ym49RVqadEHFvmse6Cvi`
🔗 **GitHub:** https://github.com/Mario1988123/CALCULADORAWIFI

---

## 📥 Método 1: GitHub Actions (Recomendado)

### Paso 1: Ir a la pestaña Actions

1. Ve a: https://github.com/Mario1988123/CALCULADORAWIFI/actions
2. Busca el workflow llamado **"Build Android APK"**
3. Haz clic en la ejecución más reciente (la primera de la lista)

### Paso 2: Descargar la APK

4. Desplázate hasta la sección **"Artifacts"** al final de la página
5. Descarga el archivo: **CalculadoraWiFi-APK**
6. Extrae el archivo ZIP descargado
7. Dentro encontrarás el archivo `.apk`

### Paso 3: Instalar en Android

8. Transfiere el archivo APK a tu móvil Android
9. En tu móvil, ve a: **Ajustes → Seguridad**
10. Activa: **"Instalar aplicaciones de fuentes desconocidas"** o **"Permitir desde esta fuente"**
11. Abre el archivo APK con un explorador de archivos
12. Toca **"Instalar"**
13. Espera a que termine la instalación
14. ¡Listo! Abre la app "Calculadora WiFi"

---

## 📥 Método 2: Releases de GitHub

Si se ha creado un Release:

1. Ve a: https://github.com/Mario1988123/CALCULADORAWIFI/releases
2. Descarga el archivo APK de la última versión
3. Sigue los pasos 8-14 del Método 1

---

## 📥 Método 3: Compilar Manualmente

Si prefieres compilar la app tú mismo:

### Requisitos:
- Android Studio instalado
- JDK 17
- Android SDK

### Pasos:

```bash
# 1. Clonar el repositorio
git clone https://github.com/Mario1988123/CALCULADORAWIFI.git
cd CALCULADORAWIFI

# 2. Cambiar a la rama correcta
git checkout claude/add-magic-tricks-01H9Ym49RVqadEHFvmse6Cvi

# 3. Abrir en Android Studio
# Archivo → Abrir → Selecciona la carpeta CALCULADORAWIFI

# 4. Sincronizar Gradle
# Android Studio lo hará automáticamente

# 5. Compilar APK
# Build → Build Bundle(s) / APK(s) → Build APK(s)

# 6. La APK estará en:
# app/build/outputs/apk/debug/app-debug.apk
```

---

## 🔧 Descargar el código ESP32

### Desde GitHub:

1. Ve al repositorio: https://github.com/Mario1988123/CALCULADORAWIFI
2. Descarga el archivo: **`ESP32_CODE.ino`**
3. Lee las instrucciones en: **`LIBRERIAS_ESP32.txt`**

### O descarga todo el repositorio:

```bash
git clone https://github.com/Mario1988123/CALCULADORAWIFI.git
```

Dentro encontrarás:
- `ESP32_CODE.ino` - Código para el ESP32
- `LIBRERIAS_ESP32.txt` - Instrucciones de instalación completas

---

## 📊 Estado de la Build

Puedes verificar si la última build fue exitosa en:

🔗 https://github.com/Mario1988123/CALCULADORAWIFI/actions

Busca el ✅ verde junto a "Build Android APK"

---

## ⚠️ Notas Importantes

### Seguridad:
- La APK no está firmada con una key de Play Store oficial
- Es completamente segura, pero Android mostrará advertencias
- Puedes revisar el código fuente completo en GitHub

### Permisos Requeridos:
- ✅ INTERNET - Para comunicarse con el ESP32
- ✅ ACCESS_WIFI_STATE - Para verificar conexión WiFi
- ✅ ACCESS_NETWORK_STATE - Para verificar estado de red

### Compatibilidad:
- ✅ Android 7.0 (API 24) o superior
- ✅ Orientación vertical (portrait)
- ✅ Funciona en todos los tamaños de pantalla

---

## 🆘 Problemas Comunes

### "No se puede instalar la aplicación"
**Solución:** Asegúrate de haber habilitado "Fuentes desconocidas" en Ajustes

### "La aplicación está bloqueada"
**Solución:** Toca "Más detalles" y luego "Instalar de todos modos"

### "Archivo APK dañado"
**Solución:** Descarga nuevamente el archivo desde GitHub Actions

### "La build falló en Actions"
**Solución:** Espera unos minutos y vuelve a intentar, o usa el Método 3 (compilación manual)

---

## 📞 Soporte

Si tienes problemas:
1. Revisa esta guía completamente
2. Verifica los requisitos de tu dispositivo
3. Lee el archivo `README.md` para más información
4. Revisa los logs de GitHub Actions si la build falla

---

## 🎬 Siguiente Paso

Una vez instalada la app:
1. Sube el código `ESP32_CODE.ino` a tu ESP32
2. Lee la **GUIA_RAPIDA.md** para aprender a usar el truco
3. ¡Disfruta de la magia! 🎩✨

---

**Desarrollado por Elitemagic**
📱 Calculadora WiFi - Truco de Cartas Mágico

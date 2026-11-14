package com.cyberrelay.app;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Build;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.webkit.JavascriptInterface;
import android.widget.Toast;

public class WebAppInterface {
    private static final String PREF_NAME = "cyberrelay_app_prefs";
    private static final String KEY_LOGGED_IN = "is_logged_in";
    private static final String KEY_USERNAME = "session_username";
    private static final String KEY_ROLE = "session_role";
    private static final String KEY_TOKEN = "session_token";
    private static final String KEY_USERS_JSON = "saved_users_json";
    private static final String KEY_FIREBASE_HOST = "firebase_host";
    private static final String KEY_FIREBASE_AUTH = "firebase_auth";
    private static final String KEY_ESP_HOST = "esp_host";

    private final Context context;
    private final SharedPreferences prefs;

    public WebAppInterface(Context context) {
        this.context = context;
        this.prefs = context.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
    }

    @JavascriptInterface
    public void showToast(String message) {
        if (message != null && !message.isEmpty()) {
            Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
        }
    }

    @JavascriptInterface
    public void vibrateAlert(int milliseconds) {
        int duration = (milliseconds > 0 && milliseconds <= 2000) ? milliseconds : 200;
        try {
            Vibrator vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
            if (vibrator != null && vibrator.hasVibrator()) {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    vibrator.vibrate(VibrationEffect.createOneShot(duration, VibrationEffect.DEFAULT_AMPLITUDE));
                } else {
                    vibrator.vibrate(duration);
                }
            }
        } catch (Exception ignored) {
        }
    }

    @JavascriptInterface
    public void vibrateAlert() {
        vibrateAlert(200);
    }

    @JavascriptInterface
    public String getPlatform() {
        return "ANDROID_NATIVE";
    }

    @JavascriptInterface
    public boolean isNativeApp() {
        return true;
    }

    /**
     * Store User Session Permanently onto Phone Storage (Forever until explicit logout)
     */
    @JavascriptInterface
    public boolean saveSession(String username, String role, String token) {
        try {
            SharedPreferences.Editor editor = prefs.edit();
            editor.putBoolean(KEY_LOGGED_IN, true);
            editor.putString(KEY_USERNAME, username != null ? username : "");
            editor.putString(KEY_ROLE, role != null ? role : "");
            editor.putString(KEY_TOKEN, token != null ? token : "");
            editor.putLong("session_saved_timestamp", System.currentTimeMillis());
            return editor.commit();
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * Retrieve the permanently stored session JSON string
     */
    @JavascriptInterface
    public String getSession() {
        try {
            boolean isLoggedIn = prefs.getBoolean(KEY_LOGGED_IN, false);
            if (!isLoggedIn) {
                return "{}";
            }
            String username = prefs.getString(KEY_USERNAME, "");
            String role = prefs.getString(KEY_ROLE, "");
            String token = prefs.getString(KEY_TOKEN, "");
            long timestamp = prefs.getLong("session_saved_timestamp", 0);

            // Return clean JSON
            return "{\"isLoggedIn\":true,\"username\":\"" + escapeJson(username) +
                   "\",\"role\":\"" + escapeJson(role) +
                   "\",\"token\":\"" + escapeJson(token) +
                   "\",\"timestamp\":" + timestamp + "}";
        } catch (Exception e) {
            return "{}";
        }
    }

    /**
     * Clear the stored session on logout
     */
    @JavascriptInterface
    public boolean clearSession() {
        try {
            SharedPreferences.Editor editor = prefs.edit();
            editor.remove(KEY_LOGGED_IN);
            editor.remove(KEY_USERNAME);
            editor.remove(KEY_ROLE);
            editor.remove(KEY_TOKEN);
            editor.remove("session_saved_timestamp");
            return editor.commit();
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * Save user accounts registered on phone
     */
    @JavascriptInterface
    public boolean saveUsersJson(String usersJson) {
        try {
            return prefs.edit().putString(KEY_USERS_JSON, usersJson).commit();
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * Get user accounts registered on phone
     */
    @JavascriptInterface
    public String getUsersJson() {
        return prefs.getString(KEY_USERS_JSON, "[]");
    }

    /**
     * Save custom Firebase Configuration into Phone Preferences
     */
    @JavascriptInterface
    public boolean saveFirebaseConfig(String host, String auth) {
        try {
            SharedPreferences.Editor editor = prefs.edit();
            editor.putString(KEY_FIREBASE_HOST, host != null ? host.trim() : "");
            editor.putString(KEY_FIREBASE_AUTH, auth != null ? auth.trim() : "");
            return editor.commit();
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * Retrieve saved Firebase Configuration
     */
    @JavascriptInterface
    public String getFirebaseConfig() {
        String host = prefs.getString(KEY_FIREBASE_HOST, "");
        String auth = prefs.getString(KEY_FIREBASE_AUTH, "");
        return "{\"host\":\"" + escapeJson(host) + "\",\"auth\":\"" + escapeJson(auth) + "\"}";
    }

    /**
     * Save custom ESP Host IP (e.g., http://192.168.4.1 or custom local IP)
     */
    @JavascriptInterface
    public boolean saveEspHost(String host) {
        try {
            return prefs.edit().putString(KEY_ESP_HOST, host != null ? host.trim() : "").commit();
        } catch (Exception e) {
            return false;
        }
    }

    @JavascriptInterface
    public String getEspHost() {
        return prefs.getString(KEY_ESP_HOST, "http://192.168.4.1");
    }

    private String escapeJson(String s) {
        if (s == null) return "";
        return s.replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r");
    }
}

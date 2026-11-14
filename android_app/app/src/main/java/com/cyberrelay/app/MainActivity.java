package com.cyberrelay.app;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowInsetsController;
import android.view.WindowManager;
import android.webkit.ConsoleMessage;
import android.webkit.WebChromeClient;
import android.webkit.WebResourceError;
import android.webkit.WebResourceRequest;
import android.webkit.WebResourceResponse;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

public class MainActivity extends Activity {
    private static final String TAG = "CyberRelay-App";
    private WebView webView;

    @SuppressLint("SetJavaScriptEnabled")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Global crash protection to avoid abrupt app force-close
        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread thread, Throwable throwable) {
                Log.e(TAG, "Uncaught Exception intercepted: ", throwable);
                try {
                    Toast.makeText(MainActivity.this, "System Alert: " + throwable.getLocalizedMessage(), Toast.LENGTH_LONG).show();
                } catch (Exception ignored) {
                }
            }
        });

        setupWindowAppearance();

        try {
            webView = new WebView(this);
            setContentView(webView);
            configureWebView();
            webView.loadUrl("file:///android_asset/index.html");
        } catch (Exception e) {
            Log.e(TAG, "Fatal error initializing WebView: ", e);
            Toast.makeText(this, "WebView Error: " + e.getMessage(), Toast.LENGTH_LONG).show();
        }
    }

    private void setupWindowAppearance() {
        try {
            Window window = getWindow();
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
                window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
                window.setStatusBarColor(Color.parseColor("#07090E"));
            }

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                WindowInsetsController controller = window.getInsetsController();
                if (controller != null) {
                    controller.setSystemBarsAppearance(0, WindowInsetsController.APPEARANCE_LIGHT_STATUS_BARS);
                }
            } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                View decor = window.getDecorView();
                if (decor != null) {
                    decor.setSystemUiVisibility(0);
                }
            }
        } catch (Exception e) {
            Log.w(TAG, "Could not set custom status bar appearance: " + e.getMessage());
        }
    }

    @SuppressLint("SetJavaScriptEnabled")
    private void configureWebView() {
        if (webView == null) return;

        WebSettings settings = webView.getSettings();
        settings.setJavaScriptEnabled(true);
        settings.setDomStorageEnabled(true);
        settings.setDatabaseEnabled(true);
        settings.setAllowFileAccess(true);
        settings.setAllowContentAccess(true);

        // CRITICAL: Allow CORS requests from file:///android_asset to 192.168.4.1 and Firebase
        try {
            settings.setAllowFileAccessFromFileURLs(true);
            settings.setAllowUniversalAccessFromFileURLs(true);
        } catch (Exception e) {
            Log.w(TAG, "Universal access flag error: " + e.getMessage());
        }

        settings.setLoadWithOverviewMode(true);
        settings.setUseWideViewPort(true);
        settings.setCacheMode(WebSettings.LOAD_DEFAULT);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            settings.setMixedContentMode(WebSettings.MIXED_CONTENT_ALWAYS_ALLOW);
        }

        webView.setBackgroundColor(Color.parseColor("#07090E"));
        // Hardware acceleration for 60-120fps butter-smooth rendering
        webView.setLayerType(View.LAYER_TYPE_HARDWARE, null);

        // Register Native Android Bridge
        WebAppInterface nativeBridge = new WebAppInterface(this);
        webView.addJavascriptInterface(nativeBridge, "AndroidNative");

        webView.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, WebResourceRequest request) {
                return false;
            }

            @Override
            public void onReceivedError(WebView view, WebResourceRequest request, WebResourceError error) {
                super.onReceivedError(view, request, error);
                if (request != null && request.isForMainFrame()) {
                    String url = request.getUrl() != null ? request.getUrl().toString() : "";
                    Log.w(TAG, "Navigation Error on " + url);
                    // Do not infinitely reload if asset has an issue
                    if (!url.startsWith("file:///android_asset/")) {
                        view.loadUrl("file:///android_asset/index.html");
                    }
                }
            }
        });

        webView.setWebChromeClient(new WebChromeClient() {
            @Override
            public boolean onConsoleMessage(ConsoleMessage consoleMessage) {
                Log.d("CyberRelay-JS", "[" + consoleMessage.messageLevel() + "] " +
                        consoleMessage.message() + " -- From line " +
                        consoleMessage.lineNumber() + " of " +
                        consoleMessage.sourceId());
                return true;
            }
        });
    }

    @Override
    public void onBackPressed() {
        if (webView != null && webView.canGoBack()) {
            webView.goBack();
        } else {
            super.onBackPressed();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (webView != null) {
            webView.onResume();
        }
    }

    @Override
    protected void onPause() {
        if (webView != null) {
            webView.onPause();
        }
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        if (webView != null) {
            webView.destroy();
            webView = null;
        }
        super.onDestroy();
    }
}

package in.namai.iitmdiy.controller;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Insets;
import android.net.ConnectivityManager;
import android.net.LinkProperties;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.view.WindowInsets;
import android.webkit.WebResourceRequest;
import android.webkit.WebResourceResponse;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.Proxy;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.Collections;

public final class ControllerActivity extends Activity {
    private static final String ASSET_HOST = "appassets.androidplatform.net";
    private static final String PAGE = "/controller.html";
    private static final String BOT_IP = "192.168.4.1";
    private static final Uri PROJECTS = Uri.parse("https://www.youtube.com/@-MRFUN");
    private static final String MOVES = "F|B|SL|SR|FL|FR|BL|BR|RL|RR|S";
    private WebView controller;

    @Override
    protected void onCreate(Bundle state) {
        super.onCreate(state);
        getWindow().getDecorView().setBackgroundColor(Color.rgb(16, 21, 33));
        if (Build.VERSION.SDK_INT >= 30) {
            getWindow().setDecorFitsSystemWindows(false);
            getWindow().getDecorView().setOnApplyWindowInsetsListener((view, insets) -> {
                Insets safe = insets.getInsets(
                    WindowInsets.Type.systemBars() | WindowInsets.Type.displayCutout());
                view.setPadding(safe.left, safe.top, safe.right, safe.bottom);
                return insets;
            });
        }

        controller = new WebView(this);
        controller.setBackgroundColor(Color.rgb(16, 21, 33));
        controller.getSettings().setJavaScriptEnabled(true);
        controller.getSettings().setAllowFileAccess(false);
        controller.getSettings().setAllowContentAccess(false);
        controller.setWebViewClient(new ControllerClient());
        setContentView(controller);
        controller.loadUrl("https://" + ASSET_HOST + PAGE);
    }

    @Override
    protected void onPause() {
        controller.evaluateJavascript("if (typeof active !== 'undefined' && active) halt();", null);
        super.onPause();
    }

    private final class ControllerClient extends WebViewClient {
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, WebResourceRequest request) {
            if (request.isForMainFrame() && PROJECTS.equals(request.getUrl())) {
                try {
                    startActivity(new Intent(Intent.ACTION_VIEW, PROJECTS));
                } catch (ActivityNotFoundException ignored) {
                    Toast.makeText(ControllerActivity.this, "No browser available", Toast.LENGTH_SHORT).show();
                }
            }
            return true;
        }

        @Override
        public WebResourceResponse shouldInterceptRequest(WebView view, WebResourceRequest request) {
            Uri uri = request.getUrl();
            if (!"https".equals(uri.getScheme()) || !ASSET_HOST.equals(uri.getAuthority())
                    || !"GET".equals(request.getMethod())) {
                return reply(403, "Forbidden");
            }

            if (PAGE.equals(uri.getPath())) {
                try {
                    return new WebResourceResponse("text/html", "UTF-8", getAssets().open("controller.html"));
                } catch (IOException ignored) {
                    return reply(500, "Controller unavailable");
                }
            }

            if ("/cmd".equals(uri.getPath())) {
                String move = uri.getQueryParameter("move");
                if (move != null && move.matches(MOVES)) {
                    return sendMove(move);
                }
                return reply(400, "Unknown movement");
            }

            return reply(404, "Not found");
        }
    }

    @SuppressWarnings("deprecation")
    private WebResourceResponse sendMove(String move) {
        ConnectivityManager manager = getSystemService(ConnectivityManager.class);
        HttpURLConnection connection = null;
        try {
            InetAddress address = InetAddress.getByName(BOT_IP);
            // The bot's Wi-Fi has no internet. Android may keep mobile data as its default network.
            for (Network network : manager.getAllNetworks()) {
                NetworkCapabilities capabilities = manager.getNetworkCapabilities(network);
                LinkProperties link = manager.getLinkProperties(network);
                if (capabilities == null || !capabilities.hasTransport(NetworkCapabilities.TRANSPORT_WIFI)
                        || link == null || link.getRoutes().stream()
                            .noneMatch(route -> {
                                InetAddress gateway = route.getGateway();
                                return (gateway == null || gateway.isAnyLocalAddress())
                                    && route.matches(address);
                            })) {
                    continue;
                }

                URL target = new URL("http://" + BOT_IP + "/cmd?move=" + move);
                connection = (HttpURLConnection) network.openConnection(target, Proxy.NO_PROXY);
                connection.setInstanceFollowRedirects(false);
                connection.setConnectTimeout(225);
                connection.setReadTimeout(275);
                connection.setUseCaches(false);
                return connection.getResponseCode() == 200
                    ? reply(200, "OK") : reply(502, "Bot rejected command");
            }
        } catch (IOException ignored) {
            return reply(503, "Bot unavailable");
        } finally {
            if (connection != null) connection.disconnect();
        }
        return reply(503, "Join the bot Wi-Fi");
    }

    private static WebResourceResponse reply(int code, String message) {
        return new WebResourceResponse("text/plain", "UTF-8", code, message,
            Collections.singletonMap("Cache-Control", "no-store"),
            new ByteArrayInputStream(message.getBytes(StandardCharsets.UTF_8)));
    }
}

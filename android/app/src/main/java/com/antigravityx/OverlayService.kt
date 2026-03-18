package com.antigravityx

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Intent
import android.graphics.PixelFormat
import android.os.IBinder
import android.view.Gravity
import android.view.LayoutInflater
import android.view.WindowManager
import androidx.core.app.NotificationCompat
import com.antigravityx.ui.FloatingButtonView
import com.antigravityx.ui.OverlayMenuView

class OverlayService : Service() {

    private lateinit var windowManager: WindowManager
    private lateinit var fabView: FloatingButtonView
    private lateinit var menuView: OverlayMenuView
    private var menuAdded = false

    companion object {
        private const val CHANNEL_ID = "agx_overlay_channel"
        private const val NOTIF_ID    = 42
    }

    override fun onCreate() {
        super.onCreate()
        windowManager = getSystemService(WINDOW_SERVICE) as WindowManager
        createNotificationChannel()
        startForeground(NOTIF_ID, buildNotification())
        addFloatingButton()
    }

    private fun addFloatingButton() {
        fabView = FloatingButtonView(this)

        val params = WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
            PixelFormat.TRANSLUCENT
        ).apply {
            gravity = Gravity.TOP or Gravity.END
            x = 16
            y = 200
        }

        fabView.onToggleMenu = { show ->
            if (show) showMenu() else hideMenu()
        }
        fabView.windowParams = params
        fabView.windowManager = windowManager

        windowManager.addView(fabView, params)
    }

    private fun showMenu() {
        if (!menuAdded) {
            menuView = OverlayMenuView(this)
            menuView.onClose = { hideMenu() }

            val params = WindowManager.LayoutParams(
                WindowManager.LayoutParams.MATCH_PARENT,
                WindowManager.LayoutParams.MATCH_PARENT,
                WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
                WindowManager.LayoutParams.FLAG_DIM_BEHIND or WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
                PixelFormat.TRANSLUCENT
            ).apply {
                dimAmount = 0.4f
                // Android 12+ Blur Behind
                if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.S) {
                    setBlurBehindRadius(25)
                }
            }

            windowManager.addView(menuView, params)
            menuAdded = true
            menuView.animateIn()
        }
    }

    private fun hideMenu() {
        if (menuAdded) {
            menuView.animateOut {
                windowManager.removeView(menuView)
                menuAdded = false
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        if (::fabView.isInitialized) windowManager.removeView(fabView)
        if (menuAdded && ::menuView.isInitialized) windowManager.removeView(menuView)
    }

    override fun onBind(intent: Intent?): IBinder? = null

    private fun createNotificationChannel() {
        val channel = NotificationChannel(
            CHANNEL_ID,
            "AntiGravity X Overlay",
            NotificationManager.IMPORTANCE_LOW
        ).apply {
            description = "Keeps the overlay active"
            setShowBadge(false)
        }
        (getSystemService(NOTIFICATION_SERVICE) as NotificationManager)
            .createNotificationChannel(channel)
    }

    private fun buildNotification(): Notification =
        NotificationCompat.Builder(this, CHANNEL_ID)
            .setContentTitle("AntiGravity X")
            .setContentText("Overlay active — tap to manage")
            .setSmallIcon(android.R.drawable.ic_dialog_info)
            .setPriority(NotificationCompat.PRIORITY_LOW)
            .setOngoing(true)
            .build()
}

package com.antigravityx

import android.app.ActivityManager
import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Context
import android.content.Intent
import android.graphics.PixelFormat
import android.os.Handler
import android.os.IBinder
import android.os.Looper
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

    private val handler = Handler(Looper.getMainLooper())
    private val checkGameRunnable = object : Runnable {
        override fun run() {
            checkIfGameIsRunning()
            handler.postDelayed(this, 3000)
        }
    }

    private var detectorView: View? = null

    override fun onCreate() {
        super.onCreate()
        windowManager = getSystemService(WINDOW_SERVICE) as WindowManager
        createNotificationChannel()
        startForeground(NOTIF_ID, buildNotification())
        
        // Wait 60 seconds before showing anything or enabling detection
        handler.postDelayed({
            addGestureDetector()
            handler.post(checkGameRunnable)
        }, 60000)
    }

    private fun addGestureDetector() {
        detectorView = View(this).apply {
            setBackgroundColor(Color.TRANSPARENT)
            setOnTouchListener { v, event ->
                if (event.pointerCount == 3 && event.actionMasked == MotionEvent.ACTION_POINTER_DOWN) {
                    if (!menuAdded) showMenu()
                    else hideMenu()
                    true
                } else false
            }
        }

        val params = WindowManager.LayoutParams(
            150, // Small trigger area to avoid blocking game too much
            150,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
            PixelFormat.TRANSLUCENT
        ).apply {
            gravity = Gravity.TOP or Gravity.START
            x = 0
            y = 0
        }
        windowManager.addView(detectorView, params)
    }

    private fun checkIfGameIsRunning() {
        val am = getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        val runningProcesses = am.runningAppProcesses
        var found = false
        if (runningProcesses != null) {
            for (processInfo in runningProcesses) {
                if (processInfo.processName == MenuState.TARGET_PACKAGE) {
                    found = true
                    break
                }
            }
        }
        MenuState.isGameRunning = found
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
        detectorView?.let { windowManager.removeView(it) }
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
            .setContentText("Status: Standby")
            .setSmallIcon(android.R.drawable.ic_dialog_info)
            .setPriority(NotificationCompat.PRIORITY_LOW)
            .setOngoing(true)
            .build()
}

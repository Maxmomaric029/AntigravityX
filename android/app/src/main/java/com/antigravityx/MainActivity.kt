package com.antigravityx

import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.provider.Settings
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.antigravityx.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private val OVERLAY_PERMISSION_REQ = 1001

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.btnLaunchOverlay.setOnClickListener {
            if (hasOverlayPermission()) {
                startOverlay()
            } else {
                requestOverlayPermission()
            }
        }

        binding.btnStopOverlay.setOnClickListener {
            stopService(Intent(this, OverlayService::class.java))
            Toast.makeText(this, "Overlay stopped", Toast.LENGTH_SHORT).show()
        }

        // Show permission status
        updatePermissionStatus()
    }

    override fun onResume() {
        super.onResume()
        updatePermissionStatus()
    }

    private fun hasOverlayPermission(): Boolean =
        Settings.canDrawOverlays(this)

    private fun requestOverlayPermission() {
        val intent = Intent(
            Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
            Uri.parse("package:$packageName")
        )
        startActivityForResult(intent, OVERLAY_PERMISSION_REQ)
    }

    private fun startOverlay() {
        val intent = Intent(this, OverlayService::class.java)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            startForegroundService(intent)
        } else {
            startService(intent)
        }
        Toast.makeText(this, "AntiGravity X overlay started!", Toast.LENGTH_SHORT).show()
        // Optionally move app to background
        moveTaskToBack(true)
    }

    private fun updatePermissionStatus() {
        val granted = hasOverlayPermission()
        binding.tvPermissionStatus.text = if (granted)
            "✓ Overlay Permission Granted"
        else
            "✗ Overlay Permission Required"
        binding.tvPermissionStatus.setTextColor(
            if (granted) 0xFF00E5FF.toInt() else 0xFFFF4444.toInt()
        )
        binding.btnLaunchOverlay.isEnabled = true
    }

    @Deprecated("Deprecated in Java")
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == OVERLAY_PERMISSION_REQ) {
            if (hasOverlayPermission()) startOverlay()
            else Toast.makeText(this, "Permission denied", Toast.LENGTH_SHORT).show()
        }
    }
}

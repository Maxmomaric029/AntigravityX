package com.antigravityx.ui

import android.content.Context
import android.graphics.*
import android.view.MotionEvent
import android.view.View
import android.view.WindowManager
import kotlin.math.abs

/**
 * Floating circular FAB that stays always-on-top.
 * Draggable. Tap to toggle the overlay menu.
 */
class FloatingButtonView(context: Context) : View(context) {

    var onToggleMenu: ((Boolean) -> Unit)? = null
    var windowParams: WindowManager.LayoutParams? = null
    var windowManager: WindowManager? = null

    private var menuOpen = false

    // Colors
    private val accentColor  = Color.parseColor("#8B2BE2")
    private val accentGlow   = Color.parseColor("#5A1496")
    private val bgColor      = Color.parseColor("#0A0A10")

    private val bgPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = bgColor
        style = Paint.Style.FILL
    }
    private val ringPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = accentColor
        style = Paint.Style.STROKE
        strokeWidth = 2.5f
    }
    private val glowPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        style = Paint.Style.FILL
        maskFilter = BlurMaskFilter(18f, BlurMaskFilter.Blur.NORMAL)
        color = accentColor
    }
    private val textPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = Color.WHITE
        textSize = 11f
        typeface = Typeface.DEFAULT_BOLD
        textAlign = Paint.Align.CENTER
    }
    private val labelPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = Color.parseColor("#8B2BE2")
        textSize = 9f
        typeface = Typeface.DEFAULT_BOLD
        textAlign = Paint.Align.CENTER
    }

    private val SIZE = 64
    private var lastX = 0f
    private var lastY = 0f
    private var isDragging = false
    private var downRawX = 0f
    private var downRawY = 0f

    init {
        setLayerType(LAYER_TYPE_SOFTWARE, null)
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        val dp = (SIZE * resources.displayMetrics.density).toInt()
        setMeasuredDimension(dp, dp)
    }

    override fun onDraw(canvas: Canvas) {
        val cx = width / 2f
        val cy = height / 2f
        val r  = (width / 2f) - 4f

        // Glow
        canvas.drawCircle(cx, cy, r - 4f, glowPaint)
        // Background
        canvas.drawCircle(cx, cy, r, bgPaint)
        // Ring
        ringPaint.color = if (menuOpen) Color.parseColor("#00E5FF") else accentColor
        canvas.drawCircle(cx, cy, r, ringPaint)
        // Text
        textPaint.color = if (menuOpen) Color.parseColor("#00E5FF") else Color.WHITE
        canvas.drawText("AGX", cx, cy + 4f, textPaint)
        canvas.drawText(if (menuOpen) "CLOSE" else "OPEN", cx, cy + 16f, labelPaint)
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        val wm = windowManager ?: return false
        val params = windowParams ?: return false

        when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                lastX = params.x.toFloat()
                lastY = params.y.toFloat()
                downRawX = event.rawX
                downRawY = event.rawY
                isDragging = false
            }
            MotionEvent.ACTION_MOVE -> {
                val dx = event.rawX - downRawX
                val dy = event.rawY - downRawY
                if (!isDragging && (abs(dx) > 8 || abs(dy) > 8)) isDragging = true
                if (isDragging) {
                    params.x = (lastX - dx).toInt()
                    params.y = (lastY + dy).toInt()
                    wm.updateViewLayout(this, params)
                }
            }
            MotionEvent.ACTION_UP -> {
                if (!isDragging) {
                    menuOpen = !menuOpen
                    onToggleMenu?.invoke(menuOpen)
                    invalidate()
                }
            }
        }
        return true
    }
}

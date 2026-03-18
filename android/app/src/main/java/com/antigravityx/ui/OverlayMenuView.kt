package com.antigravityx.ui

import android.animation.ValueAnimator
import android.content.Context
import android.graphics.*
import android.view.MotionEvent
import android.view.View
import android.view.animation.DecelerateInterpolator
import android.view.animation.OvershootInterpolator
import kotlin.math.max
import kotlin.math.min

/**
 * Full-screen overlay menu drawn entirely with Canvas.
 * Mirrors the desktop GUI: 5 sections in a slide-in panel.
 */
class OverlayMenuView(context: Context) : View(context) {

    var onClose: (() -> Unit)? = null

    // ── Sections ──────────────────────────────────────────────────
    private val sections = listOf("COMBAT", "VISUALS", "HITBOX", "SYSTEM", "AGX")
    private val sectionIcons = listOf(">>", "[]", "<>", "**", "AX")
    private var activeSection = 0

    // ── Animation ─────────────────────────────────────────────────
    private var panelSlide = 0f       // 0..1
    private var fadeAlpha  = 0f       // 0..1

    // ── COMBAT state ──────────────────────────────────────────────
    private val combatToggles = mutableMapOf(
        "AimLock Head"     to false,
        "AimLock Smooth"   to false,
        "Silent Aim"       to false,
        "FOV Circle"       to false,
        "No Recoil 100%"   to false,
    )
    private var combatFOV     = 80f
    private var combatSmooth  = 4f

    // ── VISUALS state ─────────────────────────────────────────────
    private val visualsToggles = mutableMapOf(
        "ESP Box 2D"       to false,
        "ESP Box 3D"       to false,
        "ESP Distance"     to false,
        "ESP Name & Clan"  to false,
        "ESP Skeleton"     to false,
        "ESP Line"         to false,
    )

    // ── HITBOX state ──────────────────────────────────────────────
    private val hitboxToggles = mutableMapOf(
        "Hitbox Expand"    to false,
        "Hitbox Pescoco"   to false,
        "NPC Big Head"     to false,
        "Triggerbot"       to false,
    )
    private var hitboxScale = 0   // 0=x2, 1=x5, 2=x10
    private var trigDelay   = 30f

    // ── SYSTEM state ──────────────────────────────────────────────
    private val systemToggles = mutableMapOf(
        "120 FPS Unlocker"    to false,
        "RAM Purge"           to false,
        "GPU Overclock (Sim)" to false,
        "Anti-Report System"  to false,
        "Log Cleaner"         to false,
    )
    private var fpsTarget = 120f

    // ── AGX state ─────────────────────────────────────────────────
    private val agxToggles = mutableMapOf(
        "Auto Clean"          to false,
        "Disable Startup Bloat" to false,
        "Kill BG Processes"   to false,
        "Registry Optimizer"  to false,
    )
    private val cpuUsage = 0.23f
    private val ramUsage = 0.55f

    // ── Dimensions (set in onSizeChanged) ─────────────────────────
    private val dp get() = resources.displayMetrics.density
    private val panelW get() = min(width * 0.82f, 340 * dp)
    private val sidebarW get() = 70 * dp
    private val topBarH get() = 52 * dp

    // ── Colors ────────────────────────────────────────────────────
    private val accent    = Color.parseColor("#8B2BE2")
    private val bgPanel   = Color.parseColor("#EE0D0D18") // Added alpha for glass effect
    private val bgSidebar = Color.parseColor("#F50A0A14")
    private val textPri   = Color.parseColor("#EAEAF5")
    private val textSec   = Color.parseColor("#7070A0")
    private val divColor  = Color.parseColor("#308B2BE2")

    // ── Paints ────────────────────────────────────────────────────
    private val bgPaint      = Paint(Paint.ANTI_ALIAS_FLAG)
    private val panelPaint   = Paint(Paint.ANTI_ALIAS_FLAG)
    private val sidebarPaint = Paint(Paint.ANTI_ALIAS_FLAG)
    private val accentPaint  = Paint(Paint.ANTI_ALIAS_FLAG)
    private val textPriPaint = Paint(Paint.ANTI_ALIAS_FLAG)
    private val textSecPaint = Paint(Paint.ANTI_ALIAS_FLAG)
    private val textAccPaint = Paint(Paint.ANTI_ALIAS_FLAG)
    private val linePaint    = Paint(Paint.ANTI_ALIAS_FLAG)
    private val glowPaint    = Paint(Paint.ANTI_ALIAS_FLAG)
    private val trackPaint   = Paint(Paint.ANTI_ALIAS_FLAG)

    // Slider drag state: map label -> active drag
    private data class SliderDrag(val label: String, var startX: Float, var startVal: Float)
    private var activeDrag: SliderDrag? = null

    // Scroll state per section
    private val scrollY = FloatArray(5) { 0f }
    private var lastScrollY = 0f
    private var scrollStartY = 0f

    // Hit-test helpers: store rects for interactive elements
    private data class ToggleHit(val label: String, val map: MutableMap<String, Boolean>, val rect: RectF)
    private data class SliderHit(val label: String, val getter: () -> Float, val setter: (Float) -> Unit,
                                  val min: Float, val max: Float, val rect: RectF)
    private data class TabHit(val idx: Int, val rect: RectF)
    private data class BtnHit(val label: String, val rect: RectF, val action: () -> Unit)

    private val toggleHits = mutableListOf<ToggleHit>()
    private val sliderHits = mutableListOf<SliderHit>()
    private val tabHits    = mutableListOf<TabHit>()
    private val btnHits    = mutableListOf<BtnHit>()

    init {
        setLayerType(LAYER_TYPE_SOFTWARE, null)

        bgPaint.color       = Color.BLACK
        bgPaint.alpha       = 200
        panelPaint.color    = bgPanel
        sidebarPaint.color  = bgSidebar
        accentPaint.color   = accent
        accentPaint.style   = Paint.Style.STROKE
        accentPaint.strokeWidth = 1.5f
        linePaint.color     = divColor
        linePaint.strokeWidth = 1f
        glowPaint.style     = Paint.Style.FILL
        glowPaint.maskFilter = BlurMaskFilter(12f * dp, BlurMaskFilter.Blur.NORMAL)
        glowPaint.color     = accent
        trackPaint.color    = Color.parseColor("#23233A")
        trackPaint.style    = Paint.Style.FILL

        textPriPaint.color    = textPri
        textPriPaint.isAntiAlias = true
        textPriPaint.textSize = 13f * dp

        textSecPaint.color    = textSec
        textSecPaint.isAntiAlias = true
        textSecPaint.textSize = 11f * dp

        textAccPaint.color    = accent
        textAccPaint.isAntiAlias = true
        textAccPaint.textSize = 11f * dp
        textAccPaint.typeface = Typeface.DEFAULT_BOLD
    }

    // ── Animation ─────────────────────────────────────────────────
    fun animateIn() {
        ValueAnimator.ofFloat(0f, 1f).apply {
            duration = 350
            interpolator = OvershootInterpolator(0.8f)
            addUpdateListener { panelSlide = it.animatedValue as Float; invalidate() }
            start()
        }
        ValueAnimator.ofFloat(0f, 1f).apply {
            duration = 200
            addUpdateListener { fadeAlpha = it.animatedValue as Float; invalidate() }
            start()
        }
    }

    fun animateOut(onDone: () -> Unit) {
        ValueAnimator.ofFloat(1f, 0f).apply {
            duration = 250
            interpolator = DecelerateInterpolator()
            addUpdateListener {
                panelSlide = it.animatedValue as Float
                fadeAlpha  = it.animatedValue as Float
                invalidate()
            }
            addListener(object : android.animation.AnimatorListenerAdapter() {
                override fun onAnimationEnd(a: android.animation.Animator) = onDone()
            })
            start()
        }
    }

    // ── Draw ──────────────────────────────────────────────────────
    override fun onDraw(canvas: Canvas) {
        toggleHits.clear(); sliderHits.clear(); tabHits.clear(); btnHits.clear()

        val panelX = -panelW * (1f - panelSlide)

        // Dim background
        bgPaint.alpha = (fadeAlpha * 170).toInt()
        canvas.drawRect(0f, 0f, width.toFloat(), height.toFloat(), bgPaint)

        canvas.save()
        canvas.translate(panelX, 0f)

        // Panel background
        val rounding = 14f * dp
        val panelRect = RectF(0f, 0f, panelW, height.toFloat())
        val panelPaintFill = Paint(Paint.ANTI_ALIAS_FLAG).apply { color = bgPanel }
        canvas.drawRoundRect(RectF(0f, 0f, panelW, height.toFloat()), rounding, rounding, panelPaintFill)

        // Sidebar
        val sidebarPaintFill = Paint(Paint.ANTI_ALIAS_FLAG).apply { color = bgSidebar }
        canvas.drawRect(0f, 0f, sidebarW, height.toFloat(), sidebarPaintFill)

        // Sidebar border
        linePaint.color = Color.parseColor("#8B2BE230")
        canvas.drawLine(sidebarW, 0f, sidebarW, height.toFloat(), linePaint)

        drawSidebar(canvas)
        drawTopBar(canvas)
        drawContent(canvas)

        // Panel outer border/glow
        val glowP = Paint(Paint.ANTI_ALIAS_FLAG).apply {
            style = Paint.Style.STROKE
            strokeWidth = 1f
            color = Color.parseColor("#8B2BE270")
        }
        canvas.drawRoundRect(panelRect, rounding, rounding, glowP)

        canvas.restore()

        // Close tap area (outside panel) — tap right side
        btnHits.add(BtnHit("__close__",
            RectF(panelX + panelW, 0f, width.toFloat(), height.toFloat())
        ) { onClose?.invoke() })
    }

    private fun drawSidebar(canvas: Canvas) {
        // Logo
        val logoPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
            color  = accent
            textSize = 10f * dp
            typeface = Typeface.DEFAULT_BOLD
        }
        canvas.drawText("AGX", sidebarW / 2f - logoPaint.measureText("AGX") / 2f,
            38f * dp, logoPaint)

        linePaint.color = Color.parseColor("#8B2BE230")
        canvas.drawLine(8f * dp, 50f * dp, sidebarW - 8f * dp, 50f * dp, linePaint)

        // Section tabs
        val tabH = 56f * dp
        val tabStartY = 60f * dp

        for (i in sections.indices) {
            val ty = tabStartY + i * tabH
            val selected = (i == activeSection)

            // Highlight bg
            if (selected) {
                val hlPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
                    color = Color.parseColor("#8B2BE225")
                }
                canvas.drawRoundRect(RectF(4f * dp, ty, sidebarW - 4f * dp, ty + tabH - 4*dp),
                    6f * dp, 6f * dp, hlPaint)
                // Left accent bar
                val barPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply { color = accent }
                canvas.drawRoundRect(RectF(0f, ty + 6*dp, 3f*dp, ty + tabH - 10*dp),
                    2f*dp, 2f*dp, barPaint)
            }

            // Icon
            val iP = Paint(Paint.ANTI_ALIAS_FLAG).apply {
                textSize  = 9f * dp
                color     = if (selected) Color.WHITE else textSec
                typeface  = Typeface.DEFAULT_BOLD
                textAlign = Paint.Align.CENTER
            }
            canvas.drawText(sectionIcons[i], sidebarW / 2f, ty + 24f * dp, iP)

            // Label
            val lP = Paint(Paint.ANTI_ALIAS_FLAG).apply {
                textSize  = 7f * dp
                color     = if (selected) Color.parseColor("#B070FF") else textSec
                textAlign = Paint.Align.CENTER
            }
            canvas.drawText(sections[i], sidebarW / 2f, ty + 38f * dp, lP)

            tabHits.add(TabHit(i, RectF(0f, ty, sidebarW, ty + tabH)))
        }
    }

    private fun drawTopBar(canvas: Canvas) {
        val topPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
            color = Color.parseColor("#0B0B16")
        }
        canvas.drawRect(sidebarW, 0f, panelW, topBarH, topPaint)
        linePaint.color = Color.parseColor("#8B2BE230")
        canvas.drawLine(sidebarW, topBarH, panelW, topBarH, linePaint)

        // Title
        val titP = Paint(Paint.ANTI_ALIAS_FLAG).apply {
            color    = accent
            textSize = 14f * dp
            typeface = Typeface.DEFAULT_BOLD
        }
        canvas.drawText(sections[activeSection], sidebarW + 16f * dp,
            topBarH / 2f + titP.textSize / 3f, titP)

        // Close X
        val xP = Paint(Paint.ANTI_ALIAS_FLAG).apply {
            color    = textSec
            textSize = 14f * dp
            typeface = Typeface.DEFAULT_BOLD
            textAlign = Paint.Align.RIGHT
        }
        canvas.drawText("×", panelW - 16f * dp, topBarH / 2f + xP.textSize / 3f, xP)
        btnHits.add(BtnHit("__close_x__",
            RectF(panelW - 50f*dp, 0f, panelW, topBarH)
        ) { onClose?.invoke() })
    }

    private fun drawContent(canvas: Canvas) {
        val contentX = sidebarW + 14f * dp
        val contentY = topBarH + 10f * dp
        val contentW = panelW - contentX - 14f * dp

        canvas.save()
        canvas.clipRect(sidebarW, topBarH, panelW, height.toFloat())
        canvas.translate(0f, -scrollY[activeSection])

        var y = contentY
        when (activeSection) {
            0 -> y = drawCombatSection(canvas, contentX, y, contentW)
            1 -> y = drawVisualsSection(canvas, contentX, y, contentW)
            2 -> y = drawHitboxSection(canvas, contentX, y, contentW)
            3 -> y = drawSystemSection(canvas, contentX, y, contentW)
            4 -> y = drawAGXSection(canvas, contentX, y, contentW)
        }

        canvas.restore()
    }

    // ── Section renderers ─────────────────────────────────────────

    private fun drawCombatSection(canvas: Canvas, x: Float, startY: Float, w: Float): Float {
        var y = startY
        y = drawSectionHeader(canvas, x, y, w, "Aimbot & AimLock")
        combatToggles.forEach { (label, _) ->
            y = drawToggle(canvas, x, y, w, label, combatToggles)
        }
        y += 8f * dp
        y = drawSectionHeader(canvas, x, y, w, "Parameters")
        y = drawSlider(canvas, x, y, w, "Field of View", { combatFOV }, { combatFOV = it }, 10f, 360f, "%.0f°")
        y = drawSlider(canvas, x, y, w, "Smoothing", { combatSmooth }, { combatSmooth = it }, 1f, 20f, "%.1f")
        return y
    }

    private fun drawVisualsSection(canvas: Canvas, x: Float, startY: Float, w: Float): Float {
        var y = startY
        y = drawSectionHeader(canvas, x, y, w, "ESP & Wallhack")
        visualsToggles.forEach { (label, _) ->
            y = drawToggle(canvas, x, y, w, label, visualsToggles)
        }
        return y
    }

    private fun drawHitboxSection(canvas: Canvas, x: Float, startY: Float, w: Float): Float {
        var y = startY
        y = drawSectionHeader(canvas, x, y, w, "Trigger & Scale")
        hitboxToggles.forEach { (label, _) ->
            y = drawToggle(canvas, x, y, w, label, hitboxToggles)
        }
        y += 8f * dp
        y = drawSectionHeader(canvas, x, y, w, "Hitbox Scale")
        val scaleLabels = listOf("x2", "x5", "x10")
        val btnW = (w - 2*8f*dp) / 3f
        for (i in scaleLabels.indices) {
            val bx = x + i * (btnW + 8f * dp)
            val by = y
            val selected = hitboxScale == i
            val bPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
                color = if (selected) Color.parseColor("#6010AA") else Color.parseColor("#16162A")
            }
            val rect = RectF(bx, by, bx + btnW, by + 30f * dp)
            canvas.drawRoundRect(rect, 6f * dp, 6f * dp, bPaint)
            val tP = Paint(Paint.ANTI_ALIAS_FLAG).apply {
                color    = if (selected) Color.WHITE else textSec
                textSize = 12f * dp
                typeface = if (selected) Typeface.DEFAULT_BOLD else Typeface.DEFAULT
                textAlign = Paint.Align.CENTER
            }
            canvas.drawText(scaleLabels[i], bx + btnW/2f, by + 20f*dp, tP)
            val idx = i
            btnHits.add(BtnHit("scale$i", rect) { hitboxScale = idx; invalidate() })
        }
        y += 38f * dp
        y = drawSlider(canvas, x, y, w, "Trigger Delay", { trigDelay }, { trigDelay = it }, 0f, 300f, "%.0f ms")
        return y
    }

    private fun drawSystemSection(canvas: Canvas, x: Float, startY: Float, w: Float): Float {
        var y = startY
        y = drawSectionHeader(canvas, x, y, w, "Performance & Bypass")
        systemToggles.forEach { (label, _) ->
            y = drawToggle(canvas, x, y, w, label, systemToggles)
        }
        y += 8f * dp
        y = drawSectionHeader(canvas, x, y, w, "Parameters")
        y = drawSlider(canvas, x, y, w, "Target FPS", { fpsTarget }, { fpsTarget = it }, 60f, 240f, "%.0f fps")
        return y
    }

    private fun drawAGXSection(canvas: Canvas, x: Float, startY: Float, w: Float): Float {
        var y = startY
        y = drawSectionHeader(canvas, x, y, w, "System Dashboard")
        y = drawProgressBar(canvas, x, y, w, "CPU Usage", cpuUsage, accent)
        y = drawProgressBar(canvas, x, y, w, "RAM Usage", ramUsage, Color.parseColor("#00E5FF"))
        y += 8f * dp
        y = drawSectionHeader(canvas, x, y, w, "Automation")
        agxToggles.forEach { (label, _) ->
            y = drawToggle(canvas, x, y, w, label, agxToggles)
        }
        y += 12f * dp
        // Force Boost button
        val bRect = RectF(x, y, x + w, y + 46f * dp)
        val bPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply { color = Color.parseColor("#CC1a1a") }
        canvas.drawRoundRect(bRect, 8f * dp, 8f * dp, bPaint)
        val btP = Paint(Paint.ANTI_ALIAS_FLAG).apply {
            color    = Color.WHITE
            textSize = 14f * dp
            typeface = Typeface.DEFAULT_BOLD
            textAlign = Paint.Align.CENTER
        }
        canvas.drawText("FORCE BOOST", x + w/2f, y + 28f*dp, btP)
        btnHits.add(BtnHit("force_boost", bRect) { /* action */ })
        y += 20f * dp
        val versionPaint = Paint(textSecPaint).apply { textAlign = Paint.Align.CENTER }
        canvas.drawText("Version: 2.4.0 Premium Edition", x + w/2f, y, versionPaint)
        y += 16f * dp
        canvas.drawText("Status: Secure & Verified", x + w/2f, y, versionPaint)
        
        y += 20f * dp
        return y
    }

    // ── Widget draw helpers ───────────────────────────────────────

    private fun drawSectionHeader(canvas: Canvas, x: Float, y: Float, w: Float, label: String): Float {
        val p = Paint(textAccPaint)
        canvas.drawText(label, x, y + 14f * dp, p)
        val lW = p.measureText(label)
        linePaint.color = Color.parseColor("#8B2BE235")
        canvas.drawLine(x + lW + 8f*dp, y + 8f*dp, x + w, y + 8f*dp, linePaint)
        return y + 24f * dp
    }

    private fun drawProgressBar(canvas: Canvas, x: Float, y: Float, w: Float, label: String,
                                 value: Float, color: Int): Float {
        canvas.drawText(label, x, y + 12f*dp, textSecPaint)
        val barY = y + 18f * dp
        val trackR = RectF(x, barY, x + w, barY + 6f*dp)
        canvas.drawRoundRect(trackR, 3f*dp, 3f*dp, trackPaint)
        if (value > 0f) {
            val fillP = Paint(Paint.ANTI_ALIAS_FLAG).apply { this.color = color }
            canvas.drawRoundRect(RectF(x, barY, x + w * value, barY + 6f*dp),
                3f*dp, 3f*dp, fillP)
        }
        return y + 30f * dp
    }

    private fun drawToggle(canvas: Canvas, x: Float, y: Float, w: Float,
                            label: String, map: MutableMap<String, Boolean>): Float {
        val checked = map[label] ?: false
        val tw = 38f * dp; val th = 20f * dp
        val tx = x + w - tw
        val ty = y + 2f * dp

        // Track
        val trackP = Paint(Paint.ANTI_ALIAS_FLAG).apply {
            color = if (checked) Color.parseColor("#6A1AB2") else Color.parseColor("#1A1A2E")
        }
        canvas.drawRoundRect(RectF(tx, ty, tx + tw, ty + th), th/2f, th/2f, trackP)

        // Thumb
        val thumbR = th / 2f - 2f * dp
        val thumbX = if (checked) tx + tw - thumbR - 2f*dp else tx + thumbR + 2f*dp
        val thumbP = Paint(Paint.ANTI_ALIAS_FLAG).apply {
            color = if (checked) Color.WHITE else Color.parseColor("#606070")
        }
        canvas.drawCircle(thumbX, ty + th/2f, thumbR, thumbP)

        // Label
        canvas.drawText(label, x, y + 14f*dp, textPriPaint)

        val rect = RectF(x, y, x + w, y + 28f*dp)
        toggleHits.add(ToggleHit(label, map, rect))
        return y + 34f * dp
    }

    private fun drawSlider(canvas: Canvas, x: Float, y: Float, w: Float, label: String,
                            getter: () -> Float, setter: (Float) -> Unit,
                            min: Float, max: Float, fmt: String): Float {
        val v = getter()
        // Value
        val valStr = when {
            fmt.contains("%.0f") -> "%.0f".format(v) + fmt.removePrefix("%.0f")
            fmt.contains("%.1f") -> "%.1f".format(v) + fmt.removePrefix("%.1f")
            else -> "%.0f".format(v)
        }
        canvas.drawText(label, x, y + 12f*dp, textSecPaint)
        val vP = Paint(textAccPaint)
        vP.textAlign = Paint.Align.RIGHT
        canvas.drawText(valStr, x + w, y + 12f*dp, vP)

        val barY = y + 18f * dp; val barH = 4f * dp
        val t = (v - min) / (max - min)
        val trackR = RectF(x, barY, x + w, barY + barH)
        canvas.drawRoundRect(trackR, 2f*dp, 2f*dp, trackPaint)
        if (t > 0f) {
            val fP = Paint(Paint.ANTI_ALIAS_FLAG).apply { color = accent }
            canvas.drawRoundRect(RectF(x, barY, x + w * t, barY + barH), 2f*dp, 2f*dp, fP)
        }
        // Thumb
        val thumbX = x + w * t
        val tP = Paint(Paint.ANTI_ALIAS_FLAG).apply { color = accent }
        canvas.drawCircle(thumbX, barY + barH/2f, 7f*dp, tP)
        val tInner = Paint(Paint.ANTI_ALIAS_FLAG).apply { color = Color.WHITE }
        canvas.drawCircle(thumbX, barY + barH/2f, 4f*dp, tInner)

        val rect = RectF(x, barY - 8f*dp, x + w, barY + barH + 8f*dp)
        sliderHits.add(SliderHit(label, getter, setter, min, max, rect))
        return y + 36f * dp
    }

    // ── Touch ─────────────────────────────────────────────────────
    override fun onTouchEvent(event: MotionEvent): Boolean {
        // Translate to panel coordinates
        val panelX = -panelW * (1f - panelSlide)
        val px = event.x - panelX
        val py = event.y + scrollY[activeSection]

        when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                lastScrollY  = event.y
                scrollStartY = scrollY[activeSection]
                activeDrag   = null

                // Check sliders
                for (sh in sliderHits) {
                    val rect = RectF(sh.rect.left, sh.rect.top - scrollY[activeSection],
                        sh.rect.right, sh.rect.bottom - scrollY[activeSection])
                    if (rect.contains(px - panelX + panelX, py - scrollY[activeSection])) {
                        activeDrag = SliderDrag(sh.label, event.rawX, sh.getter())
                        break
                    }
                }
            }
            MotionEvent.ACTION_MOVE -> {
                val drag = activeDrag
                if (drag != null) {
                    val dx = event.rawX - drag.startX
                    for (sh in sliderHits) {
                        if (sh.label == drag.label) {
                            val range = sh.max - sh.min
                            val newVal = (drag.startVal + dx / (panelW * 0.6f) * range)
                                .coerceIn(sh.min, sh.max)
                            sh.setter(newVal)
                            invalidate()
                            break
                        }
                    }
                } else {
                    // Scroll
                    val dy = lastScrollY - event.y
                    scrollY[activeSection] = (scrollY[activeSection] + dy).coerceAtLeast(0f)
                    lastScrollY = event.y
                    invalidate()
                }
            }
            MotionEvent.ACTION_UP -> {
                if (activeDrag == null) {
                    // Check toggles
                    for (th in toggleHits) {
                        val rect = RectF(th.rect.left + panelX, th.rect.top - scrollY[activeSection],
                            th.rect.right + panelX, th.rect.bottom - scrollY[activeSection])
                        if (rect.contains(event.x, event.y)) {
                            th.map[th.label] = !(th.map[th.label] ?: false)
                            invalidate()
                            return true
                        }
                    }
                    // Check section tabs
                    for (tab in tabHits) {
                        val rect = RectF(tab.rect.left + panelX, tab.rect.top,
                            tab.rect.right + panelX, tab.rect.bottom)
                        if (rect.contains(event.x, event.y)) {
                            scrollY[activeSection] = 0f
                            activeSection = tab.idx
                            invalidate()
                            return true
                        }
                    }
                    // Check buttons
                    for (btn in btnHits) {
                        val offsetX = if (btn.label.startsWith("__close")) 0f else panelX
                        val rect = RectF(btn.rect.left + offsetX, btn.rect.top - scrollY[activeSection],
                            btn.rect.right + offsetX, btn.rect.bottom - scrollY[activeSection])
                        val checkRect = if (btn.label.startsWith("__close")) btn.rect else rect
                        if (checkRect.contains(event.x, event.y)) {
                            btn.action()
                            return true
                        }
                    }
                }
                activeDrag = null
            }
        }
        return true
    }
}

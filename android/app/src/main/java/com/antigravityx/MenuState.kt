package com.antigravityx

import androidx.compose.runtime.mutableStateMapOf
import androidx.compose.runtime.mutableStateOf

/**
 * Global state for the AntiGravity X Android overlay.
 * Preserves settings even when the MenuView is detached.
 */
object MenuState {
    
    // ── Sections ──────────────────────────────────────────────────
    var activeSection = 0

    // ── COMBAT state ──────────────────────────────────────────────
    val combatToggles = mutableMapOf(
        "AimLock Head"     to false,
        "AimLock Smooth"   to false,
        "Silent Aim"       to false,
        "FOV Circle"       to false,
        "No Recoil 100%"   to false
    )
    var combatFOV     = 80f
    var combatSmooth  = 4f

    // ── VISUALS state ─────────────────────────────────────────────
    val visualsToggles = mutableMapOf(
        "ESP Box 2D"       to false,
        "ESP Box 3D"       to false,
        "ESP Distance"     to false,
        "ESP Name & Clan"  to false,
        "ESP Skeleton"     to false,
        "ESP Line"         to false,
        "Antena View"      to false
    )

    // ── HITBOX state ──────────────────────────────────────────────
    val hitboxToggles = mutableMapOf(
        "Hitbox Expand"    to false,
        "Hitbox Pescoco"   to false,
        "NPC Big Head"     to false,
        "Triggerbot"       to false
    )
    var hitboxScale = 0   // 0=x2, 1=x5, 2=x10
    var trigDelay   = 30f

    // ── SYSTEM state ──────────────────────────────────────────────
    val systemToggles = mutableMapOf(
        "120 FPS Unlocker"    to false,
        "RAM Purge"           to false,
        "GPU Overclock (Sim)" to false,
        "Anti-Report System"  to false,
        "Log Cleaner"         to false
    )
    var fpsTarget = 120f

    // ── AGX state ─────────────────────────────────────────────────
    val agxToggles = mutableMapOf(
        "Auto Clean"          to false,
        "Disable Startup Bloat" to false,
        "Kill BG Processes"   to false,
        "Registry Optimizer"  to false
    )

    // ── Game Verification ─────────────────────────────────────────
    const val TARGET_PACKAGE = "com.dts.freefireth"
    var isGameRunning = false

    init {
        try {
            System.loadLibrary("antigravity")
            init() // Call native init
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    // Native bridge methods
    private external fun init()
    external fun getInitBase(): Long
    external fun applyNoRecoil(enable: Boolean)
    external fun applySpeedHack(enable: Boolean)
    external fun applyAntena(enable: Boolean)
}

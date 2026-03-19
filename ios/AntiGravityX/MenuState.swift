import Foundation
import Combine

/// Global state for the AntiGravity X iOS overlay.
/// Mirrors MenuState.kt — preserves settings even when overlay is hidden.
class MenuState: ObservableObject {

    static let shared = MenuState()
    private init() {}

    // ── Sections ──────────────────────────────────────────────────
    @Published var activeSection = 0

    // ── COMBAT ────────────────────────────────────────────────────
    @Published var combatToggles: [String: Bool] = [
        "AimLock Head":   false,
        "AimLock Smooth": false,
        "Silent Aim":     false,
        "FOV Circle":     false,
        "No Recoil 100%": false,
        "Speedhack x5":   false
    ]
    @Published var combatFOV:    Float = 80
    @Published var combatSmooth: Float = 4

    // ── VISUALS ───────────────────────────────────────────────────
    @Published var visualsToggles: [String: Bool] = [
        "ESP Box 2D":     false,
        "ESP Box 3D":     false,
        "ESP Distance":   false,
        "ESP Name & Clan":false,
        "ESP Skeleton":   false,
        "ESP Line":       false,
        "Antena View":    false
    ]

    // ── HITBOX ────────────────────────────────────────────────────
    @Published var hitboxToggles: [String: Bool] = [
        "Hitbox Expand":  false,
        "Hitbox Pescoco": false,
        "NPC Big Head":   false,
        "Triggerbot":     false
    ]
    @Published var hitboxScale = 0   // 0=x2, 1=x5, 2=x10
    @Published var trigDelay:  Float = 30

    // ── SYSTEM ────────────────────────────────────────────────────
    @Published var systemToggles: [String: Bool] = [
        "120 FPS Unlocker":   false,
        "RAM Purge":          false,
        "GPU Overclock (Sim)":false,
        "Anti-Report System": false,
        "Log Cleaner":        false
    ]
    @Published var fpsTarget: Float = 120

    // ── AGX ───────────────────────────────────────────────────────
    @Published var agxToggles: [String: Bool] = [
        "Auto Clean":            false,
        "Disable Startup Bloat": false,
        "Kill BG Processes":     false,
        "Registry Optimizer":    false
    ]

    // ── Native Bridge (stubs) ─────────────────────────────────────
    func setToggle(dict: String, key: String, value: Bool) {
        switch dict {
        case "combat":  combatToggles[key]  = value
        case "visuals": visualsToggles[key] = value
        case "hitbox":  hitboxToggles[key]  = value
        case "system":  systemToggles[key]  = value
        case "agx":     agxToggles[key]     = value
        default: break
        }
        // Native calls via dlsym would go here
        switch key {
        case "No Recoil 100%": break // applyNoRecoil(value)
        case "Speedhack x5":   break // applySpeedHack(value)
        case "Antena View":    break // applyAntena(value)
        default: break
        }
    }
}

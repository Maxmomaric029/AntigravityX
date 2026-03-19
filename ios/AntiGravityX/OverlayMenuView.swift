import SwiftUI

// MARK: - Main Overlay Menu

struct OverlayMenuView: View {
    @EnvironmentObject var state: MenuState
    var onClose: () -> Void = {}

    private let sections  = ["COMBAT", "VISUALS", "HITBOX", "SYSTEM", "AGX"]
    private let icons     = [">>", "[]", "<>", "**", "AX"]

    // Colors — mirror of Android palette
    private let accent    = Color(hex: "8B2BE2")
    private let bgPanel   = Color(hex: "0D0D18")
    private let bgSidebar = Color(hex: "0A0A14")
    private let textPri   = Color(hex: "EAEAF5")
    private let textSec   = Color(hex: "7070A0")

    var body: some View {
        HStack(spacing: 0) {
            // ── Sidebar ───────────────────────────────────────────
            VStack(spacing: 0) {
                Text("AGX")
                    .font(.system(size: 10, weight: .black))
                    .foregroundColor(accent)
                    .padding(.top, 16)
                    .padding(.bottom, 8)

                Rectangle()
                    .fill(accent.opacity(0.3))
                    .frame(height: 1)
                    .padding(.horizontal, 8)

                Spacer().frame(height: 12)

                ForEach(0..<sections.count, id: \.self) { i in
                    SidebarTab(
                        icon:       icons[i],
                        label:      sections[i],
                        isSelected: state.activeSection == i,
                        accent:     accent
                    )
                    .onTapGesture {
                        UIImpactFeedbackGenerator(style: .light).impactOccurred()
                        withAnimation(.spring(response: 0.3, dampingFraction: 0.75)) {
                            state.activeSection = i
                        }
                    }
                }
                Spacer()
            }
            .frame(width: 68)
            .background(bgSidebar)

            // ── Content Area ──────────────────────────────────────
            VStack(spacing: 0) {
                // Top bar
                HStack(spacing: 8) {
                    Text(sections[state.activeSection])
                        .font(.system(size: 13, weight: .bold))
                        .foregroundColor(accent)
                    Spacer()
                    Button(action: onClose) {
                        Image(systemName: "xmark")
                            .font(.system(size: 12, weight: .semibold))
                            .foregroundColor(textSec)
                    }
                }
                .padding(.horizontal, 14)
                .padding(.vertical, 12)
                .background(Color(hex: "0B0B16"))

                Rectangle().fill(accent.opacity(0.2)).frame(height: 1)

                // Section content
                ScrollView(showsIndicators: false) {
                    SectionContent(
                        section: sections[state.activeSection],
                        accent: accent,
                        textPri: textPri,
                        textSec: textSec
                    )
                    .environmentObject(state)
                    .padding(.horizontal, 14)
                    .padding(.vertical, 12)
                }
                .background(bgPanel.opacity(0.97))
            }
        }
        .frame(width: 340, height: 460)
        .background(bgPanel)
        .cornerRadius(14)
        .overlay(
            RoundedRectangle(cornerRadius: 14)
                .stroke(accent.opacity(0.35), lineWidth: 1)
        )
        .shadow(color: accent.opacity(0.15), radius: 24)
        .shadow(color: .black.opacity(0.6),  radius: 12)
    }
}

// MARK: - Sidebar Tab
struct SidebarTab: View {
    let icon:       String
    let label:      String
    let isSelected: Bool
    let accent:     Color

    var body: some View {
        HStack(spacing: 0) {
            Rectangle()
                .fill(isSelected ? accent : Color.clear)
                .frame(width: 3)
                .padding(.vertical, 10)

            VStack(spacing: 3) {
                Text(icon)
                    .font(.system(size: 12, weight: .bold))
                Text(label)
                    .font(.system(size: 7, weight: .semibold))
            }
            .foregroundColor(isSelected ? .white : Color(hex: "7070A0"))
            .frame(maxWidth: .infinity)
            .padding(.vertical, 10)
            .background(isSelected ? accent.opacity(0.15) : Color.clear)
        }
    }
}

// MARK: - Section Content
struct SectionContent: View {
    @EnvironmentObject var state: MenuState
    let section: String
    let accent:  Color
    let textPri: Color
    let textSec: Color

    var body: some View {
        switch section {
        case "COMBAT":  combatSection
        case "VISUALS": visualsSection
        case "HITBOX":  hitboxSection
        case "SYSTEM":  systemSection
        default:        agxSection
        }
    }

    // ── COMBAT ────────────────────────────────────────────────────
    private var combatSection: some View {
        VStack(alignment: .leading, spacing: 0) {
            SectionHeader(title: "AIMBOT", accent: accent)
            ForEach(["AimLock Head", "AimLock Smooth", "Silent Aim", "FOV Circle"], id: \.self) { key in
                AGXToggle(label: key, isOn: binding("combat", key), accent: accent, textPri: textPri)
            }

            SectionHeader(title: "MOVEMENT", accent: accent)
            AGXToggle(label: "Speedhack x5", isOn: binding("combat", "Speedhack x5"), accent: accent, textPri: textPri)

            SectionHeader(title: "RECOIL", accent: accent)
            AGXToggle(label: "No Recoil 100%", isOn: binding("combat", "No Recoil 100%"), accent: accent, textPri: textPri)

            SectionHeader(title: "SLIDERS", accent: accent)
            AGXSlider(label: "FOV", value: $state.combatFOV, range: 20...180, accent: accent, textSec: textSec)
            AGXSlider(label: "Smooth", value: $state.combatSmooth, range: 1...10, accent: accent, textSec: textSec)
        }
    }

    // ── VISUALS ───────────────────────────────────────────────────
    private var visualsSection: some View {
        VStack(alignment: .leading, spacing: 0) {
            SectionHeader(title: "ESP", accent: accent)
            ForEach(["ESP Box 2D", "ESP Box 3D", "ESP Distance", "ESP Name & Clan", "ESP Skeleton", "ESP Line"], id: \.self) { key in
                AGXToggle(label: key, isOn: binding("visuals", key), accent: accent, textPri: textPri)
            }

            SectionHeader(title: "RADAR", accent: accent)
            AGXToggle(label: "Antena View", isOn: binding("visuals", "Antena View"), accent: accent, textPri: textPri)
        }
    }

    // ── HITBOX ────────────────────────────────────────────────────
    private var hitboxSection: some View {
        VStack(alignment: .leading, spacing: 0) {
            SectionHeader(title: "HITBOX CONFIG", accent: accent)
            ForEach(["Hitbox Expand", "Hitbox Pescoco", "NPC Big Head", "Triggerbot"], id: \.self) { key in
                AGXToggle(label: key, isOn: binding("hitbox", key), accent: accent, textPri: textPri)
            }

            SectionHeader(title: "HITBOX SCALE", accent: accent)
            Picker("", selection: $state.hitboxScale) {
                Text("x2").tag(0)
                Text("x5").tag(1)
                Text("x10").tag(2)
            }
            .pickerStyle(.segmented)
            .colorScheme(.dark)
            .tint(accent)
            .padding(.top, 4)
            .padding(.bottom, 8)

            AGXSlider(label: "Trig Delay (ms)", value: $state.trigDelay, range: 0...200, accent: accent, textSec: textSec)
        }
    }

    // ── SYSTEM ────────────────────────────────────────────────────
    private var systemSection: some View {
        VStack(alignment: .leading, spacing: 0) {
            SectionHeader(title: "PERFORMANCE", accent: accent)
            ForEach(["120 FPS Unlocker", "RAM Purge", "GPU Overclock (Sim)"], id: \.self) { key in
                AGXToggle(label: key, isOn: binding("system", key), accent: accent, textPri: textPri)
            }

            SectionHeader(title: "SECURITY", accent: accent)
            ForEach(["Anti-Report System", "Log Cleaner"], id: \.self) { key in
                AGXToggle(label: key, isOn: binding("system", key), accent: accent, textPri: textPri)
            }

            SectionHeader(title: "FPS TARGET", accent: accent)
            AGXSlider(label: "FPS", value: $state.fpsTarget, range: 30...120, accent: accent, textSec: textSec)
        }
    }

    // ── AGX ───────────────────────────────────────────────────────
    private var agxSection: some View {
        VStack(alignment: .leading, spacing: 0) {
            SectionHeader(title: "SYSTEM DASHBOARD", accent: accent)
            TelemetryBar(label: "CPU LOAD",  value: 0.23, color: accent)
            TelemetryBar(label: "RAM USAGE", value: 0.55, color: .cyan)

            Rectangle().fill(Color.white.opacity(0.06)).frame(height: 1).padding(.vertical, 8)

            SectionHeader(title: "OPTIMIZER", accent: accent)
            ForEach(["Auto Clean", "Disable Startup Bloat", "Kill BG Processes", "Registry Optimizer"], id: \.self) { key in
                AGXToggle(label: key, isOn: binding("agx", key), accent: accent, textPri: textPri)
            }
        }
    }

    // ── Helper ────────────────────────────────────────────────────
    private func binding(_ dict: String, _ key: String) -> Binding<Bool> {
        Binding(
            get: {
                switch dict {
                case "combat":  return state.combatToggles[key]  ?? false
                case "visuals": return state.visualsToggles[key] ?? false
                case "hitbox":  return state.hitboxToggles[key]  ?? false
                case "system":  return state.systemToggles[key]  ?? false
                default:        return state.agxToggles[key]     ?? false
                }
            },
            set: { state.setToggle(dict: dict, key: key, value: $0) }
        )
    }
}

// MARK: - Reusable Components

struct SectionHeader: View {
    let title:  String
    let accent: Color

    var body: some View {
        HStack(spacing: 6) {
            Text(title)
                .font(.system(size: 9, weight: .bold))
                .foregroundColor(accent)
            Rectangle().fill(accent.opacity(0.25)).frame(height: 1)
        }
        .padding(.top, 12)
        .padding(.bottom, 6)
    }
}

struct AGXToggle: View {
    let label:   String
    @Binding var isOn: Bool
    let accent:  Color
    let textPri: Color

    var body: some View {
        HStack {
            Text(label)
                .font(.system(size: 12))
                .foregroundColor(isOn ? textPri : Color(hex: "7070A0"))
            Spacer()
            ZStack {
                RoundedRectangle(cornerRadius: 12)
                    .fill(isOn ? accent : Color(hex: "23233A"))
                    .frame(width: 38, height: 22)
                Circle()
                    .fill(.white)
                    .frame(width: 16, height: 16)
                    .offset(x: isOn ? 8 : -8)
                    .shadow(color: isOn ? accent.opacity(0.5) : .clear, radius: 4)
            }
            .animation(.spring(response: 0.25, dampingFraction: 0.7), value: isOn)
            .onTapGesture {
                UIImpactFeedbackGenerator(style: .light).impactOccurred()
                isOn.toggle()
            }
        }
        .padding(.vertical, 7)
        .contentShape(Rectangle())
        .onTapGesture {
            UIImpactFeedbackGenerator(style: .light).impactOccurred()
            isOn.toggle()
        }
    }
}

struct AGXSlider: View {
    let label:   String
    @Binding var value: Float
    let range:   ClosedRange<Float>
    let accent:  Color
    let textSec: Color

    var body: some View {
        VStack(alignment: .leading, spacing: 4) {
            HStack {
                Text(label)
                    .font(.system(size: 11))
                    .foregroundColor(textSec)
                Spacer()
                Text(String(format: "%.0f", value))
                    .font(.system(size: 11, weight: .semibold))
                    .foregroundColor(accent)
            }
            Slider(value: $value, in: range)
                .accentColor(accent)
        }
        .padding(.bottom, 8)
    }
}

struct TelemetryBar: View {
    let label: String
    let value: CGFloat
    let color: Color

    var body: some View {
        VStack(alignment: .leading, spacing: 5) {
            HStack {
                Text(label)
                    .font(.system(size: 9))
                    .foregroundColor(Color(hex: "7070A0"))
                Spacer()
                Text("\(Int(value * 100))%")
                    .font(.system(size: 9, weight: .bold))
                    .foregroundColor(color)
            }
            GeometryReader { g in
                ZStack(alignment: .leading) {
                    RoundedRectangle(cornerRadius: 3)
                        .fill(Color(hex: "1A1A2E"))
                    RoundedRectangle(cornerRadius: 3)
                        .fill(color.opacity(0.85))
                        .frame(width: g.size.width * value)
                        .shadow(color: color.opacity(0.5), radius: 4)
                }
            }
            .frame(height: 6)
        }
        .padding(.bottom, 10)
    }
}

// MARK: - Color Helper
extension Color {
    init(hex: String) {
        let h = hex.trimmingCharacters(in: CharacterSet.alphanumerics.inverted)
        var val: UInt64 = 0
        Scanner(string: h).scanHexInt64(&val)
        let r = Double((val & 0xFF0000) >> 16) / 255
        let g = Double((val & 0x00FF00) >>  8) / 255
        let b = Double( val & 0x0000FF       ) / 255
        self.init(red: r, green: g, blue: b)
    }
}

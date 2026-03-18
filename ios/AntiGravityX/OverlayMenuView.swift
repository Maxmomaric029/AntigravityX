import SwiftUI

struct OverlayMenuView: View {
    @State private var activeSection = 0
    private let sections = ["COMBAT", "VISUALS", "HITBOX", "SYSTEM", "AGX"]
    private let icons = [">>", "[]", "<>", "**", "AX"]
    
    var body: some View {
        HStack(spacing: 0) {
            // Sidebar
            VStack(spacing: 20) {
                Text("AGX")
                    .font(.caption)
                    .fontWeight(.bold)
                    .foregroundColor(Color(hex: "7F52FF"))
                    .padding(.top, 40)
                
                Divider().background(Color(hex: "7F52FF").opacity(0.3))
                
                ForEach(0..<sections.count, id: \.self) { i in
                    SidebarTab(icon: icons[i], label: sections[i], isSelected: activeSection == i)
                        .onTapGesture {
                            withAnimation(.spring()) {
                                activeSection = i
                            }
                            UIImpactFeedbackGenerator(style: .medium).impactOccurred()
                        }
                }
                
                Spacer()
            }
            .frame(width: 70)
            .background(Color(hex: "0A0A14"))
            
            // Content
            VStack(spacing: 0) {
                // Top Bar
                HStack {
                    Text(sections[activeSection])
                        .font(.headline)
                        .foregroundColor(Color(hex: "7F52FF"))
                    Spacer()
                    Image(systemName: "xmark")
                        .foregroundColor(.gray)
                }
                .padding()
                .background(Color(hex: "0B0B16"))
                
                // Content area
                ScrollView {
                    VStack(alignment: .leading, spacing: 15) {
                        SectionContent(section: sections[activeSection])
                    }
                    .padding()
                }
                .background(Color(hex: "0D0D18").opacity(0.95))
            }
        }
        .frame(width: 320, height: 450)
        .cornerRadius(15)
        .overlay(
            RoundedRectangle(cornerRadius: 15)
                .stroke(Color(hex: "7F52FF").opacity(0.4), lineWidth: 1)
        )
        .shadow(color: .black.opacity(0.5), radius: 20)
    }
}

struct SidebarTab: View {
    let icon: String
    let label: String
    let isSelected: Bool
    
    var body: some View {
        VStack(spacing: 4) {
            Text(icon)
                .font(.system(size: 14, weight: .bold))
            Text(label)
                .font(.system(size: 8))
        }
        .foregroundColor(isSelected ? .white : .gray)
        .frame(width: 60, height: 50)
        .background(isSelected ? Color(hex: "7F52FF").opacity(0.2) : Color.clear)
        .cornerRadius(8)
        .overlay(
            isSelected ? Rectangle().fill(Color(hex: "7F52FF")).frame(width: 3).padding(.vertical, 8) : nil
            , alignment: .leading
        )
    }
}

struct SectionContent: View {
    let section: String
    
    var body: some View {
        VStack(alignment: .leading, spacing: 20) {
            if section == "AGX" {
                Text("System Dashboard")
                    .foregroundColor(Color(hex: "7F52FF"))
                    .font(.subheadline)
                
                TelemetryGraphView(label: "CPU LOAD", value: 0.65, color: Color(hex: "7F52FF"))
                TelemetryGraphView(label: "MEMORY LOAD", value: 0.42, color: .cyan)
                
                Divider().background(Color.gray.opacity(0.3))
                
                ToggleView(label: "Auto-Clean Cache", isOn: .constant(true))
                ToggleView(label: "Deep Registry Fix", isOn: .constant(false))
            } else {
                Text("\(section) features coming soon...")
                    .foregroundColor(.gray)
            }
        }
    }
}

struct TelemetryGraphView: View {
    let label: String
    let value: CGFloat
    let color: Color
    
    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            Text(label)
                .font(.system(size: 10))
                .foregroundColor(.gray)
            
            ZStack(alignment: .leading) {
                RoundedRectangle(cornerRadius: 4)
                    .fill(Color(hex: "1A1A2E"))
                    .frame(height: 6)
                
                RoundedRectangle(cornerRadius: 4)
                    .fill(color)
                    .frame(width: 250 * value, height: 6)
                    .glow(color: color, radius: 5)
            }
        }
    }
}

struct ToggleView: View {
    let label: String
    @Binding var isOn: Bool
    
    var body: some View {
        HStack {
            Text(label)
                .foregroundColor(.white)
            Spacer()
            Toggle("", isOn: $isOn)
                .toggleStyle(SwitchToggleStyle(tint: Color(hex: "7F52FF")))
        }
    }
}

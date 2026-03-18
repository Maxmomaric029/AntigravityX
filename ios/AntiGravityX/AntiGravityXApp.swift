import SwiftUI

@main
struct AntiGravityXApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}

struct ContentView: View {
    @State private var isOverlayShowing = false
    
    var body: some View {
        VStack(spacing: 20) {
            Image(systemName: "shield.fill")
                .resizable()
                .frame(width: 80, height: 80)
                .foregroundColor(Color(hex: "7F52FF"))
                .glow(color: Color(hex: "7F52FF"), radius: 15)
            
            Text("AntiGravity X - Premium")
                .font(.title)
                .fontWeight(.bold)
            
            Button(action: {
                isOverlayShowing.toggle()
                // In a jailbroken environment, this would call into the OverlayWindowManager
            }) {
                Text(isOverlayShowing ? "Hide Overlay" : "Show Overlay")
                    .padding()
                    .frame(maxWidth: .infinity)
                    .background(Color(hex: "7F52FF"))
                    .foregroundColor(.white)
                    .cornerRadius(12)
            }
            .padding(.horizontal, 40)
        }
        .preferredColorScheme(.dark)
    }
}

extension Color {
    init(hex: String) {
        let hex = hex.trimmingCharacters(in: CharacterSet.alphanumerics.inverted)
        var int: UInt64 = 0
        Scanner(string: hex).scanHexInt64(&int)
        let a, r, g, b: UInt64
        switch hex.count {
        case 3: // RGB (12-bit)
            (a, r, g, b) = (255, (int >> 8) * 17, (int >> 4 & 0xF) * 17, (int & 0xF) * 17)
        case 6: // RGB (24-bit)
            (a, r, g, b) = (255, int >> 16, int >> 8 & 0xFF, int & 0xFF)
        case 8: // ARGB (32-bit)
            (a, r, g, b) = (int >> 24, int >> 16 & 0xFF, int >> 8 & 0xFF, int & 0xFF)
        default:
            (a, r, g, b) = (1, 1, 1, 0)
        }
        self.init(.sRGB, red: Double(r) / 255, green: Double(g) / 255, blue: Double(b) / 255, opacity: Double(a) / 255)
    }
}

extension View {
    func glow(color: Color, radius: CGFloat) -> some View {
        self.shadow(color: color, radius: radius / 3)
            .shadow(color: color, radius: radius / 3)
            .shadow(color: color, radius: radius / 3)
    }
}

import SwiftUI

/// Draggable floating button that toggles the overlay menu.
/// Mirrors FloatingButtonView.kt from Android.
struct FloatingButtonView: View {

    @Binding var isMenuOpen: Bool
    @State private var position = CGPoint(x: 40, y: 200)
    @State private var dragOffset = CGSize.zero
    @State private var isPulsing = false

    var body: some View {
        ZStack {
            // Glow ring
            Circle()
                .stroke(Color(hex: "8B2BE2").opacity(0.4), lineWidth: 2)
                .frame(width: 56, height: 56)
                .scaleEffect(isPulsing ? 1.25 : 1.0)
                .opacity(isPulsing ? 0 : 0.8)
                .animation(.easeOut(duration: 1.2).repeatForever(autoreverses: false), value: isPulsing)

            // Main button
            Circle()
                .fill(
                    LinearGradient(
                        colors: [Color(hex: "8B2BE2"), Color(hex: "5A1A9A")],
                        startPoint: .topLeading,
                        endPoint: .bottomTrailing
                    )
                )
                .frame(width: 48, height: 48)
                .shadow(color: Color(hex: "8B2BE2").opacity(0.6), radius: 10)
                .overlay(
                    Text("AGX")
                        .font(.system(size: 11, weight: .black))
                        .foregroundColor(.white)
                )
        }
        .position(
            x: position.x + dragOffset.width,
            y: position.y + dragOffset.height
        )
        .gesture(
            DragGesture()
                .onChanged { g in dragOffset = g.translation }
                .onEnded { g in
                    position.x += g.translation.width
                    position.y += g.translation.height
                    dragOffset = .zero
                }
        )
        .onTapGesture {
            UIImpactFeedbackGenerator(style: .medium).impactOccurred()
            withAnimation(.spring(response: 0.35, dampingFraction: 0.7)) {
                isMenuOpen.toggle()
            }
        }
        .onAppear { isPulsing = true }
    }
}

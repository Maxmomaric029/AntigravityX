import SwiftUI

@main
struct AntiGravityXApp: App {
    @StateObject private var menuState = MenuState.shared

    var body: some Scene {
        WindowGroup {
            OverlayRootView()
                .environmentObject(menuState)
        }
    }
}

// MARK: - Root overlay host
struct OverlayRootView: View {
    @EnvironmentObject var state: MenuState
    @State private var isMenuOpen = false
    @State private var menuOffset = CGSize(width: -400, height: 0)
    @State private var menuOpacity = 0.0

    var body: some View {
        ZStack {
            Color.black.ignoresSafeArea()

            // Floating button — always visible
            FloatingButtonView(isMenuOpen: $isMenuOpen)

            // Overlay panel with slide-in
            if isMenuOpen {
                Color.black.opacity(0.55)
                    .ignoresSafeArea()
                    .onTapGesture { closeMenu() }
                    .transition(.opacity)

                OverlayMenuView(onClose: closeMenu)
                    .offset(menuOffset)
                    .opacity(menuOpacity)
                    .environmentObject(state)
                    .transition(.asymmetric(
                        insertion: .move(edge: .leading).combined(with: .opacity),
                        removal:   .move(edge: .leading).combined(with: .opacity)
                    ))
                    .onAppear {
                        withAnimation(.spring(response: 0.35, dampingFraction: 0.75)) {
                            menuOffset  = .zero
                            menuOpacity = 1
                        }
                    }
            }
        }
        .onChange(of: isMenuOpen) { open in
            if !open { menuOffset = CGSize(width: -400, height: 0); menuOpacity = 0 }
        }
    }

    private func closeMenu() {
        withAnimation(.easeIn(duration: 0.25)) {
            menuOffset  = CGSize(width: -400, height: 0)
            menuOpacity = 0
        }
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.25) {
            isMenuOpen = false
        }
    }
}

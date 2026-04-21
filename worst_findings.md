# Worst Findings

This list is ordered by engineering risk, with emphasis on user-data safety, correctness, test signal, and maintainability.

## 1. Drag-and-drop accepts everything on entry and validates too late

- References: `src/CullendulaMainWindow.cpp:79`, `src/CullendulaMainWindow.cpp:88`
- Problem: `dragEnterEvent()` unconditionally accepts the drag, even though `dropEvent()` only handles URL payloads. The app advertises acceptance before it knows whether the payload is usable.
- Impact: this produces misleading UX and increases the chance of odd states or confusing no-op drops. Input validation should happen at the boundary, not after the UI already signaled acceptance.

## 2. Image loading and scaling is fully synchronous on every resize event

- References: `src/CullendulaMainWindow.cpp:124`, `src/CullendulaMainWindow.cpp:204`
- Problem: every window resize triggers `refreshLabel()`, which reloads the image from disk and rescales it immediately on the UI thread.
- Impact: with large JPEGs, resize interactions can become visibly laggy. The cost compounds because the code decodes from disk repeatedly instead of caching the original image or deferring expensive work.

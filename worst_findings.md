# Worst Findings

This list is ordered by engineering risk, with emphasis on user-data safety, correctness, test signal, and maintainability.

## 1. Image loading and scaling is fully synchronous on every resize event

- References: `src/CullendulaMainWindow.cpp:124`, `src/CullendulaMainWindow.cpp:204`
- Problem: every window resize triggers `refreshLabel()`, which reloads the image from disk and rescales it immediately on the UI thread.
- Impact: with large JPEGs, resize interactions can become visibly laggy. The cost compounds because the code decodes from disk repeatedly instead of caching the original image or deferring expensive work.

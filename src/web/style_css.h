#pragma once

const char STYLE_CSS[] PROGMEM = R"CSS(
:root {
  color-scheme: light dark;
  --bg: #0c0c0f;
  --card: #171821;
  --accent: #ffb703;
  --text: #f1f5f9;
  font-family: "Segoe UI", system-ui, -apple-system, sans-serif;
}

body {
  background: radial-gradient(circle at top left, #1d1f2f, var(--bg));
  color: var(--text);
  min-height: 100vh;
  margin: 0;
  display: grid;
  place-items: center;
}

main {
  background: var(--card);
  padding: 1.5rem;
  border-radius: 18px;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.45);
  width: min(420px, 92vw);
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 1rem;
}

h1 {
  font-size: 1.4rem;
  margin: 0;
}

section {
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
}

.grid {
  display: grid;
  grid-template-columns: 1fr;
  gap: 0.5rem;
}

label {
  font-weight: 600;
}

button, select, input[type="range"], input[type="color"] {
  font: inherit;
}

button {
  background: var(--accent);
  color: #0b0b0f;
  border: none;
  padding: 0.75rem 1rem;
  border-radius: 12px;
  font-weight: 700;
  cursor: pointer;
  transition: transform 120ms ease, box-shadow 120ms ease;
  box-shadow: 0 10px 20px rgba(0, 0, 0, 0.35);
}

button.secondary {
  background: transparent;
  color: var(--text);
  border: 1px solid rgba(255, 255, 255, 0.14);
}

button:hover {
  transform: translateY(-2px);
}

select, input[type="color"], input[type="range"] {
  padding: 0.65rem;
  border-radius: 10px;
  border: 1px solid rgba(255, 255, 255, 0.08);
  background: rgba(255, 255, 255, 0.04);
  color: var(--text);
}

input[type="color"] {
  padding: 0.2rem;
  height: 48px;
}

@media (min-width: 520px) {
  .grid {
    grid-template-columns: repeat(2, 1fr);
  }
}
)CSS";


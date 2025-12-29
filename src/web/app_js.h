#pragma once

const char APP_JS[] PROGMEM = R"JS(
const state = {
  isOn: true,
  brightness: 180,
  speed: 128,
  effect: 'solid',
  color: { r: 255, g: 200, b: 180, w: 0 },
  effects: []
};

async function loadState() {
  const res = await fetch('/api/state');
  const data = await res.json();
  Object.assign(state, data);
  render();
}

async function sendState() {
  await fetch('/api/state', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(state)
  });
}

function render() {
  document.getElementById('brightness').value = state.brightness;
  document.getElementById('speed').value = state.speed;
  document.getElementById('effect').value = state.effect;
  document.getElementById('white').value = state.color.w;

  const colorHex = `#${state.color.r.toString(16).padStart(2, '0')}${state.color.g
    .toString(16)
    .padStart(2, '0')}${state.color.b.toString(16).padStart(2, '0')}`;
  document.getElementById('color').value = colorHex;
  document.getElementById('power').textContent = state.isOn ? 'AN' : 'AUS';
}

function attachHandlers() {
  document.getElementById('brightness').addEventListener('input', (e) => {
    state.brightness = Number(e.target.value);
    sendState();
  });

  document.getElementById('speed').addEventListener('input', (e) => {
    state.speed = Number(e.target.value);
    sendState();
  });

  document.getElementById('effect').addEventListener('change', (e) => {
    state.effect = e.target.value;
    sendState();
  });

  document.getElementById('white').addEventListener('input', (e) => {
    state.color.w = Number(e.target.value);
    sendState();
  });

  document.getElementById('color').addEventListener('input', (e) => {
    const hex = e.target.value.replace('#', '');
    state.color.r = parseInt(hex.slice(0, 2), 16);
    state.color.g = parseInt(hex.slice(2, 4), 16);
    state.color.b = parseInt(hex.slice(4, 6), 16);
    sendState();
  });

  document.getElementById('power').addEventListener('click', () => {
    state.isOn = !state.isOn;
    sendState();
    render();
  });
}

function populateEffects() {
  const select = document.getElementById('effect');
  select.innerHTML = '';
  state.effects.forEach((effect) => {
    const option = document.createElement('option');
    option.value = effect;
    option.textContent = effect;
    select.appendChild(option);
  });
}

window.addEventListener('DOMContentLoaded', async () => {
  attachHandlers();
  await loadState();
  populateEffects();
  render();
});
)JS";


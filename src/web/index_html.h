#pragma once

const char INDEX_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="de">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Weihnachtsstern</title>
  <link rel="stylesheet" href="/style.css" />
</head>
<body>
  <main>
    <header>
      <h1>Arduino Stern</h1>
      <button id="power" class="secondary">AUS</button>
    </header>

    <section>
      <label for="effect">Effekt</label>
      <select id="effect"></select>
    </section>

    <section class="grid">
      <label for="brightness">Helligkeit</label>
      <input id="brightness" type="range" min="0" max="255" />

      <label for="speed">Geschwindigkeit</label>
      <input id="speed" type="range" min="0" max="255" />
    </section>

    <section class="grid">
      <label for="color">Farbe (RGB)</label>
      <input id="color" type="color" />

      <label for="white">Weiss-Kanal</label>
      <input id="white" type="range" min="0" max="255" />
    </section>
  </main>
  <script src="/app.js"></script>
</body>
</html>
)HTML";



# 🧠 Progetto L03 – Background Subtraction 

## 📘 Descrizione
Tre strategie di **sottrazione dello sfondo** su sequenze di immagini in scala di grigi:
- **Prev**: differenza con il frame precedente.
- **Mean**: media mobile su una finestra di `N` frame.
- **EMA**: media esponenziale con fattore `alpha`.

Il programma legge sequenze con pattern `printf` (es. `seq_%03d.png`) e salva automaticamente i risultati nelle cartelle `prev/`, `mean/`, `ema/`.

---

## 📁 Struttura
```
L03-background/
├── CMakeLists.txt
├── include/
│   ├── args.hpp
│   ├── background.hpp
│   └── io.hpp
├── src/
│   ├── main.cpp
│   ├── background.cpp
│   └── io.cpp
├── data/
├── build/
├── put/
├── prev/
├── mean/
├── ema/
└── out/
```

---

## 🔧 Compilazione
Richiede **OpenCV** e **CMake ≥ 3.10**.
```bash
mkdir build && cd build
cmake ..
make
```

---

## ▶️ Esecuzione
```bash
# PrevFrame
./l03_bg_app --mode prev --image ../data/seq_%03d.png --threshold 50 --wait 1

# Running Mean (finestra N)
./l03_bg_app --mode mean --image ../data/seq_%03d.png --window 10 --threshold 50

# Exponential Moving Average (alpha in [0,1])
./l03_bg_app --mode ema  --image ../data/seq_%03d.png --alpha 0.6 --threshold 50
```

Salvataggi automatici:
- `prev/bg_XXXXXX.png`, `prev/fg_XXXXXX.png`
- `mean/bg_XXXXXX.png`, `mean/fg_XXXXXX.png`
- `ema/bg_XXXXXX.png`, `ema/fg_XXXXXX.png`

Disabilita con `--no-save`.

---

## ⚙️ Parametri
| Opzione | Descrizione |
|---|---|
| `--image <pattern>` | Pattern della sequenza (es. `../data/seq_%03d.png`) |
| `--mode <prev|mean|ema>` | Metodo di background subtraction |
| `--threshold <k>` | Soglia FG (default 50) |
| `--window <N>` | Finestra mean (default 10) |
| `--alpha <a>` | Fattore EMA in [0,1] (default 0.5) |
| `--wait <ms>` | Attesa tra frame (default 0) |
| `--no-save` | Non salvare su disco |

---

## 🧩 Note tecniche
- Foreground calcolato come sogliatura di `|frame - bg|` su tipo intero per evitare overflow.
- Background **Mean** ottenuto con `cv::accumulate` e normalizzazione.
- Background **EMA** aggiornato con `bg = alpha*bg + (1-alpha)*frame`.

---

## 👨‍🎓 Informazioni accademiche
**Studente:** Dimonte Giuseppe — **MAT:** 367431  
**Docente:** Prof. Massimo Bertozzi  
**Corso di laurea:** Laurea Magistrale in Ingegneria Informatica — **Percorso:** Intelligenza Artificiale

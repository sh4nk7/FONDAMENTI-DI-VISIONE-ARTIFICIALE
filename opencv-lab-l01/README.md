
# 🧠 Progetto L01 (rewrite) – Operazioni base su immagini con OpenCV

Questa è una **riscrittura originale** dell’assegnamento L01, con la stessa funzionalità ma
**codice e struttura progettuale inediti**, stile C++17 e CMake moderni.

## ✅ Obiettivi
- Accesso diretto ai pixel (`cv::Mat`) senza helper ad alto livello.
- Subsampling, flip, crop, padding, shuffle blocchi/canali.
- Debayerizzazione semplice su pattern **RGGB/GBRG/BGGR** (downsample, luminance, simple).

## 📦 Struttura
```
L01_rewrite/
├── CMakeLists.txt
├── include/
│   ├── args.hpp
│   ├── io.hpp
│   └── ops.hpp
├── src/
│   ├── io.cpp
│   ├── main.cpp
│   └── ops.cpp
└── data/
    └── <qui le immagini di test>
```

## ⚙️ Build
Richiede **CMake ≥ 3.10** e **OpenCV** installato.

```bash
mkdir build && cd build
cmake ..
make
```

## ▶️ Esecuzione

Build

Richiede CMake ≥ 3.10 e OpenCV installato.
```bash
mkdir build && cd build
cmake ..
make

```
Esecuzione (esempi)

```bash
# Esegui tutto
./l01_app --image ../data/lenna.png --all

# Solo alcuni esercizi
./l01_app --image ../data/lenna.png --exercises down2x,flipH,pad --pad 8

# Crop definito e poi casuale
./l01_app --image ../data/lenna.png --exercises crop --crop 120,80,200,150
./l01_app --image ../data/lenna.png --exercises crop --random-crop

# Sequenza di frame (pattern stile %06d)
./l01_app --image "../data/frame_%06d.png" --all --wait 30

```

### Opzioni principali
- `--image <path>`: immagine di input (supporta pattern tipo `%06d`).
- `--wait <ms>`: attesa tra frame.
- `--pad <n>`: padding zero.
- `--crop u,v,w,h`: specifica ritaglio.
- `--random-crop`: abilita crop casuale.
- `--exercises <lista>`: elenco separato da virgole.  
  Disponibili: `down2x, downRows, downCols, flipH, flipV, crop, pad, shuffleBlocks, shuffleChannels, debayerDownG, debayerLum, debayerSimple, debayerSimpleDown`
- `--all`: esegue tutto.

## 👨‍🎓 Intestazione accademica
**Studente:** Dimonte Giuseppe — **MAT:** 367431  
**Docente:** Prof. Massimo Bertozzi  
**Corso di laurea:** LM Ingegneria Informatica — **Percorso:** Intelligenza Artificiale

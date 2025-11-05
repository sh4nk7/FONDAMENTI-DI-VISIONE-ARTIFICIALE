
# 🧠 Laboratorio L05 — Edge Detection

## 📘 Descrizione
Pipeline completa per il **rilevamento dei bordi**:
1. **Gaussian blur** separabile
2. **Sobel 3×3**: magnitudo e orientazione
3. **Non-Maximum Suppression (NMS)**
4. **Double Threshold** con promozione dei deboli connessi ai forti

Implementazione in C++ con **OpenCV**, struttura modulare e salvataggio degli output intermedi.

---

## 📁 Struttura
```
L05-edges/
├── CMakeLists.txt
├── include/
│   ├── args.hpp
│   ├── utils.hpp
│   ├── convolution.hpp
│   └── filters.hpp
├── src/
│   ├── args.cpp
│   ├── utils.cpp
│   ├── convolution.cpp
│   ├── filters.cpp
│   └── main.cpp
├── data/
└── out/
    ├── gaussian/
    ├── sobel/
    ├── nms/
    └── canny/
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
./l05_edges_app -i ../data/lenna.png -s 1.0 -k 5 -L 50 -H 150 -t 1
```
Opzioni:
- `-i <file>` immagine di input (grayscale)
- `-s <sigma>` sigma gaussiana (default 1.0)
- `-k <kernel>` dimensione kernel (dispari, default 3)
- `-L <low>` soglia bassa
- `-H <high>` soglia alta
- `-t <ms>` attesa finestre

---

## 📤 Output
- `out/gaussian/gaussian.png`
- `out/sobel/magnitude.png`, `out/sobel/orientation.png`
- `out/nms/edges_nms.png`
- `out/canny/edges_final.png`

---

## 👨‍🎓 Informazioni accademiche
**Studente:** Dimonte Giuseppe — **MAT:** 367431  
**Docente:** Prof. Massimo Bertozzi  
**Corso di laurea:** Laurea Magistrale in Ingegneria Informatica — Percorso Intelligenza Artificiale

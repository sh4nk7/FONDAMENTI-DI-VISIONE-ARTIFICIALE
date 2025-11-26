# 🧭 Laboratorio L07 — Stereo Matching

## 📘 Descrizione  
Scopo del laboratorio: calcolare una **mappa di disparità** utilizzando immagini stereo rettificate e la metrica **SAD (Sum of Absolute Differences)** su una finestra 7×7.

L'obiettivo è:

- stimare la disparità per ogni pixel dell’immagine sinistra,
- limitare la ricerca entro 128 colonne,
- generare la matrice **V-Disparity** per analizzare l’andamento delle disparità lungo le righe.

---

## 🧮 Teoria di riferimento

### 🔹 Disparità stereo
Per immagini rettificate, i punti corrispondenti si trovano sulla **stessa riga**:

d = x_left − x_right


La disparità è inversamente proporzionale alla profondità:

Z ≈ (f * B) / d


dove:
- `f` → lunghezza focale,
- `B` → baseline tra le camere.

---

### 🔹 SAD 7×7  
Per ogni pixel `(r, c)` sull’immagine sinistra:

1. si confronta la finestra 7×7 con quella destra,
2. si calcola la somma delle differenze assolute,
3. si seleziona la disparità con il valore minimo.

Ricerca limitata a:

d ∈ [0, 127]


---

### 🔹 V-Disparity  
Matrice di dimensione:

H × 128


dove:
- ogni riga rappresenta la distribuzione delle disparità della stessa riga dell’immagine,
- utile per individuare superfici come **strade** o **piani inclinati**.

---

## 📁 Struttura del progetto
```bash

L07-stereo/
├── CMakeLists.txt
├── src/
│ └── simple.cpp
├── data/
│ ├── L.pgm
│ └── R.pgm
└── README.md

```


---

## ⚙️ Compilazione

Richiede **OpenCV** installato.

```bash
mkdir build && cd build
cmake ..
make
```

Genera:

```bash

./stereo

```
--- 

## ▶️ Esecuzione


```bash

./stereo ../data/L.pgm ../data/R.pgm

```

---

### Argomenti

| Flag     | Significato                     |
|----------|---------------------------------|
| `<left>` | immagine sinistra rettificata   |
| `<right>`| immagine destra rettificata     |


---

### 🧰 Funzionalità implementate

| Funzione                   | Descrizione                                 |
|----------------------------|---------------------------------------------|
| `computeDisparitySAD7x7()` | calcolo disparità con SAD 7×7               |
| `buildVDisparity()`        | genera matrice V-Disparity                  |
| visualizzazione OpenCV     | normalizzazione e display delle finestre    |
| salvataggio automatico     | esporta risultati nella cartella out/       |

---

## 📤 Output atteso

Finestre mostrate:

    - Left / Right → immagini originali

    - Disparity SAD → mappa normalizzata in scala di grigi

    - V-Disparity → istogramma per riga

---

## 💾 Output salvato automaticamente

Alla fine dell'esecuzione vengono creati i file:

```bash

out/disparity.png
out/vdisparity.png

```

Caratteristiche:

✅ la cartella out/ viene generata se assente
✅ nessuna modifica necessaria da parte dell’utente
✅ percorso sempre relativo alla directory di esecuzione

Esempio dopo la build:

```bash

build/out/disparity.png
build/out/vdisparity.png

```

---



## 🎯 Risultato finale

Il laboratorio mostra come ricavare informazioni di profondità da immagini stereo tramite:

    1. corrispondenza locale SAD,

    2. disparità per pixel,

    3. analisi strutturata tramite V-Disparity.
    
    4. esportazione automatica dei risultati.

---

## 👨‍🎓 Informazioni accademiche

Studente: Dimonte Giuseppe — MAT: 367431
Docente: Prof. Massimo Bertozzi
Corso di laurea: Laurea Magistrale in Ingegneria Informatica — Percorso Intelligenza Artificiale

---




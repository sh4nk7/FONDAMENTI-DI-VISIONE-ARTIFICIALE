# 🧭 Laboratorio L04 — Morphological Image Processing
## 📘 Descrizione

Scopo del laboratorio: applicare operazioni di elaborazione morfologica su immagini binarie ottenute tramite background subtraction, al fine di migliorare la segmentazione e identificare gli oggetti presenti nella scena.

L’obiettivo è comprendere come, partendo da un’immagine in scala di grigi, sia possibile:

- ottenere una versione binarizzata,

- applicare operazioni morfologiche di erosione e dilatazione,

- realizzare un’apertura per rimuovere rumore,

- e infine etichettare i cluster connessi presenti nell’immagine.

---

## 🧮 Teoria di riferimento
🔹 Binarizzazione (Thresholding)

Dato un pixel p, viene convertito in immagine binaria secondo:

```bash

p = 0     se  p < T
p = 255   altrimenti

```
dove T è la soglia impostata dall’utente.

--- 
🔹 Morfologia matematica
Le operazioni utilizzano uno structuring element (SE) 3×3 con origine centrale.

✅ Dilatazione
Espande le regioni bianche:

il pixel diventa 255 se almeno un elemento del SE sovrapposto è 255.

✅ Erosione
Riduce le regioni bianche:

il pixel resta 255 solo se tutti i pixel coperti dal SE sono 255.

✅ Apertura
Composizione:

```bash

apertura = erosione → dilatazione

```

Effetto:

rimozione di piccoli disturbi,

preservazione delle forme principali.

---

🔹 Labelling (Componenti connesse)

L’immagine binaria viene analizzata riga per riga, assegnando etichette progressive e unificando etichette equivalenti in un secondo passaggio.

Risultato:

- ogni cluster ottiene un’identità numerica distinta.

---

## 📁 Struttura del progetto

```bash

L04-morphology/
├── CMakeLists.txt
├── exprunning.cpp
├── data/
│   └── (sequenza di immagini)
└── README.md

```

---

⚙️ Compilazione

Richiede OpenCV installato.

```bash

mkdir build && cd build
cmake ..
make

```
Genera l’eseguibile:

```bash

./exprunning

```

---

## ▶️ Esecuzione

La sequenza deve essere specificata in formato numerico (es. %04d):

```bash

./exprunning -i ../data/frame%04d.png -k 50 -t 0 -a 0.5

```
---

### Argomenti

| Flag | Significato |
|------|-------------|
| `-i` | file/sequence di input (obbligatorio) |
| `-k` | soglia binarizzazione (default 50) |
| `-t` | attesa tra i frame in ms |
| `-a` | alpha per aggiornamento background |

---

### Funzionalità implementate

| Funzione     | Descrizione |
|--------------|-------------|
| `binarize()`   | converte immagine CV_8UC1 in binaria |
| `dilation()`   | implementazione manuale della dilatazione |
| `erosion()`    | implementazione manuale dell’erosione |
| `opening()`    | erosione + dilatazione |
| `labelImage()` | etichettatura row-by-row dei cluster |
| `otsu()`       | calcolo della soglia tramite istogramma |

---

## 📤 Output atteso

Finestre visualizzate:

- background → background aggiornato

- binary → foreground binarizzato

- opened → risultato morfologico

- labels → componenti etichettate con applyColorMap

Il risultato finale evidenzia gli oggetti in movimento filtrati dal rumore.

---

🎯 Risultato finale

La pipeline implementata esegue:

    1. background subtraction

    2. binarizzazione

    3. apertura per rimozione rumore

    4. labelling delle componenti connesse

Consentendo l’isolamento degli oggetti significativi nella scena.

---

## 👨‍🎓 Informazioni accademiche

Studente: Dimonte Giuseppe — MAT: 367431
Docente: Prof. Massimo Bertozzi
Corso di laurea: Laurea Magistrale in Ingegneria Informatica — Percorso Intelligenza Artificiale

---

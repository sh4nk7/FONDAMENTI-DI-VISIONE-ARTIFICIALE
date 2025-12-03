# 🧪 Laboratorio L08 — Model Fitting con RANSAC (Stima di una retta)

## 📘 Descrizione

In questo laboratorio imparerai a stimare una **retta** a partire da un insieme di punti 2D affetti da **rumore** e **outlier**, utilizzando l’algoritmo **RANSAC**.

Il progetto carica:

- un’immagine,
- un file di punti 2D (inlier + outlier),
- ed esegue un fitting robusto della retta.

Alla fine otterrai:

- la retta stimata (colore giallo),
- inlier marcati in **verde**,
- outlier marcati in **rosso**,
- salvataggio dell’immagine annotata in `out/`.

---

## 🧮 Teoria

### 🔹 Model Fitting

Un modello geometrico (qui: una retta) deve “spiegare” al meglio un insieme di punti.  
Per una retta definita da due punti \( A, B \), la distanza di un punto \( P \) è:

\[
d = \frac{|(x - x_A)(y_B - y_A) - (y - y_A)(x_B - x_A)|}{\sqrt{(x_B - x_A)^2 + (y_B - y_A)^2}}
\]

Questa distanza viene confrontata con una **soglia**: se minore → il punto è **inlier**, altrimenti **outlier**.

---

### 🔹 RANSAC

RANSAC (**RAN**dom **SA**mple **C**onsensus) permette di stimare un modello anche in presenza di molti outlier.

Procedura:

1. Scegli **casualmente** un campione minimo di punti → qui, **2 punti**.
2. Costruisci un’**ipotesi di retta**.
3. Conta quanti punti reali sono abbastanza vicini (distanza < soglia).
4. Salva la retta che ottiene **più inlier**.
5. Ripeti per un certo numero di iterazioni (es. 500).

L’output finale è il modello che spiega meglio i dati.

---

## 🗂 Struttura del progetto
```bash
L08/
├── CMakeLists.txt
├── README.md
├── data/
│ ├── image.png
│ ├── points_30.txt
│ └── ...
├── src/
│ └── fitting.cpp
└── out/
└── (vuota all’inizio)
```
> ⚠️ La cartella `out/` **Viene creata**, quella che vedete è già una simulazione.

---

## ⚙️ Compilazione

Da terminale:

```bash
mkdir build
cd build
cmake ..
make
```

Se la compilazione va a buon fine, otterrai un eseguibile come:

```bash
./fitting
```
---

## ▶️ Esecuzione

Esempio principale:
```bash
./fitting \
  -i ../data/barn.png \
  -p ../data/00-033.dat \
  -d 30 \
  -t 0

```

---

## 📌 Parametri disponibili

| Opzione    | Significato                                                      |
|------------|------------------------------------------------------------------|
| `-i <img>` | immagine di input (supporta sequenze tipo `frame_%03d.png`)      |
| `-p <pts>` | file di punti (uno per riga: `x y`)                              |
| `-d <dist>`| soglia distanza RANSAC (default **30**)                          |
| `-t <ms>`  | tempo di attesa di `cv::waitKey` in millisecondi (default **0**) |
| `-h`       | mostra il messaggio di help                                      |

---

## 🔍 Passi eseguiti dal programma

### 1️⃣ Parsing degli argomenti
Il programma legge i parametri da riga di comando (`-i`, `-p`, `-d`, `-t`, `-h`) e inizializza le variabili necessarie.

### 2️⃣ Lettura dell’immagine
Carica l’immagine specificata tramite `-i`, supportando anche sequenze (`frame_%03d.png`).

### 3️⃣ Lettura dei punti
Legge da `points_name` una lista di punti nel formato:

xy 

### 4️⃣ Algoritmo RANSAC

- **Scelta casuale di 2 punti** → definiscono una retta candidata.  
- **Costruzione dell’ipotesi di retta** basata su quei due punti.  
- **Conteggio degli inlier/outlier** confrontando la distanza con la soglia `-d`.  
- **Aggiornamento del best model** se l’ipotesi corrente è migliore della precedente.  
- Ripetizione dell’intero processo per un numero fisso di iterazioni.

### 5️⃣ Visualizzazione dei risultati

- **Inlier** → evidenziati in **verde**  
- **Outlier** → evidenziati in **rosso**  
- **Retta stimata** → tracciata in **giallo**  

Il risultato è mostrato a schermo con `imshow`.

### 6️⃣ Salvataggio automatico

Il programma salva l’immagine prodotta nella cartella:

out/result_<num>.png


dove `<num>` è il numero del frame corrente.

### 7️⃣ Controllo da tastiera

- `q` → chiude il programma  
- altri tasti → pronti per future estensioni (pausa, step-by-step, replay)

---

## 🖼 Risultato finale

Il programma produce un'immagine annotata contenente:

- i punti letti dal file,
- la classificazione inlier/outlier,
- la retta stimata robustamente tramite RANSAC,
- colori coerenti con la leggenda didattica.

E salva il tutto in `out/`.

---

## 📤 Output atteso

Al termine dell’esecuzione troverai:

```bash
    out/
    ├── result_0.png
    ├── result_1.png (se usi sequenze)
    └── ...
```


Ogni immagine sarà una copia dell'input con retta + punti annotati.

Output a terminale previsto:

```bash
    Opening image.png
    Letti 250 punti.
    Best model has 171 inliers.
    Saved: out/result_0.png
    key -1
```

---

## 🎯 Obiettivi del laboratorio

- Comprendere il concetto di **modellazione robusta**.  
- Imparare il funzionamento dell’algoritmo **RANSAC**.  
- Implementare correttamente la **distanza punto–retta**.  
- Visualizzare ed interpretare graficamente il fitting.  
- Saper lavorare con **OpenCV + CMake** in un progetto strutturato.

---

## 💡 Suggerimenti per l’esplorazione

- Prova file con percentuali diverse di outlier (`points_10.txt`, `points_50.txt`, ecc.).  
- Modifica la soglia `-d` e osserva come varia il numero di inlier.  
- Cambia il numero di iterazioni RANSAC nel codice per studiare stabilità e accuratezza.  
- Aggiungi uno **stop adattivo** basato sul numero di inlier trovati.

---

## 🏁 Conclusione

Il laboratorio L08 insegna un concetto chiave della visione artificiale:  
**estrarre un modello valido anche in presenza di dati sporchi, incompleti o contaminati da outlier**.

Questa tecnica è utilizzata in:  
computer vision, robotica mobile, SLAM, estrazione di feature geometriche, ricostruzione 3D e tanto altro.

---

## 👨‍🎓 Informazioni accademiche

**Studente:** Dimonte Giuseppe — **MAT:** 367431  
**Docente:** Prof. Massimo Bertozzi  
**Corso di laurea:** Laurea Magistrale in Ingegneria Informatica — Percorso Intelligenza Artificiale  

---


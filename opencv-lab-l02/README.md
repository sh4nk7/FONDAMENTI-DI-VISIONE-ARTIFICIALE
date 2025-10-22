# 🧠 Progetto L02: Implementazione di `myfilter2D` in C++ con OpenCV

## 📘 Descrizione

Questo progetto ha l’obiettivo di **implementare una funzione di convoluzione 2D personalizzata** (`myfilter2D`) che replica il comportamento della funzione `cv::filter2D` di OpenCV, **senza utilizzare** il metodo `at<T>()`.

La funzione viene testata confrontando i risultati con i filtri integrati di OpenCV, come:
- `cv::filter2D`
- `cv::Sobel`

Il progetto è stato riorganizzato a partire dal codice fornito dal docente, in modo modulare e facilmente compilabile tramite **CMake**.

---

## 📁 Struttura del progetto

```text
L02-convolution/
│
├── include/
│ ├── myfilter2D.hpp # Dichiarazione della funzione myfilter2D
│ └── utils.hpp # Dichiarazioni funzioni di supporto
│
├── src/
│ ├── main.cpp # Programma principale
│ ├── myfilter2D.cpp # Implementazione del filtro personalizzato
│ └── utils.cpp # Funzioni di supporto (argomenti, padding)
│
├── data/
│ └── lena.png # Immagine di test (puoi sostituirla con qualsiasi immagine)
│
├── CMakeLists.txt # Configurazione per la build
└── README.md # Questo file
```

---

## ⚙️ Funzione `myfilter2D`

### Dichiarazione
```cpp
void myfilter2D(
    const cv::Mat& src,
    const cv::Mat& krn,
    cv::Mat& out,
    int stride = 1
);
```
-----

### 📋 Specifiche della funzione `myfilter2D`

| Parametro | Tipo | Descrizione |
|------------|------|-------------|
| `src` | `CV_8UC1` | Immagine di input in scala di grigi (grayscale) |
| `krn` | `CV_32FC1` | Kernel di convoluzione (matrice di pesi) con dimensioni dispari |
| `out` | `CV_32SC1` | Immagine di output con valori interi a 32 bit con segno |
| `stride` | `int` | Passo del filtro durante lo scorrimento (default = 1) |

---

### ⚠️ Vincoli

- ❌ **Non è consentito usare** `at<T>()` per accedere ai pixel  
  ✅ Usare invece `ptr<T>()`, `data`, o accesso diretto in memoria.
- 🔢 Il kernel deve avere **dimensioni dispari** (es. 3×3, 5×5, 7×7).
- 🧮 L’output deve essere di tipo **intero a 32 bit con segno** → `CV_32S`.


## 🧩 Funzionalità principali del programma

Il programma implementa una pipeline di elaborazione di immagini che include:

1. 🖼️ **Caricamento immagine**
   - Supporta sia immagini singole che sequenze (tramite formato `%0xd` nel nome file).

2. ⚫ **Conversione in scala di grigi**
   - Conversione da RGB/BGR a grayscale per semplificare le operazioni sui pixel.

3. 🔲 **Applicazione filtro Box (media)**
   - Implementato manualmente tramite `myfilter2D`.
   - Confronto diretto con il risultato di `cv::boxFilter` di OpenCV.

4. 🧭 **Applicazione filtro Sobel**
   - Calcolo del gradiente orizzontale e verticale con `myfilter2D`.
   - Confronto visivo con i risultati ottenuti tramite `cv::Sobel`.

5. 📈 **Calcolo di magnitudine e orientamento**
   - Magnitudine calcolata come `sqrt(gx² + gy²)`.
   - Orientamento dei gradienti visualizzato con `cv::applyColorMap`.

---

## 🧮 Logica della convoluzione

La funzione `myfilter2D` segue la seguente logica operativa:

1. ✅ **Verifica** che il kernel (`krn`) abbia dimensioni dispari.  
2. 🧱 **Aggiunge padding** ai bordi dell’immagine per evitare perdita di informazioni (tramite la funzione `addPadding`).  
3. 🔁 **Scorre** l’immagine di input con passo `stride` su righe e colonne.  
4. ➕ **Calcola** per ogni posizione la somma pesata dei valori di pixel e kernel:  
    ```
    out(i, j) = Σ Σ [ src(i + k, j + l) * kernel(k, l) ]
    ```
5. 💾 **Scrive** il risultato finale nel buffer di output (`CV_32S`), producendo un’immagine filtrata.

---

## Immagine (grayscale) con padding:

+-------------------------------------------------------+
| 0  0  0  0  0  0  0  0  0  0                         |
| 0 10 12 15 18 22 25 28 30  0                         |
| 0 11 14 17 20 24 27 29 31  0                         |
| 0 13 16 19 22 26 29 32 35  0                         |
| 0 15 18 21 25 28 31 34 37  0                         |
| 0 17 20 23 27 30 33 36 39  0                         |
| 0 19 22 25 29 32 35 38 41  0                         |
| 0 21 24 27 31 34 37 40 43  0                         |
| 0 23 26 29 33 36 39 42 45  0                         |
| 0  0  0  0  0  0  0  0  0  0                         |
+-------------------------------------------------------+

Finestra 3x3 del kernel (centrata su un pixel dell’immagine):

Kernel (esempio Sobel X):
[-1   0   1
 -2   0   2
 -1   0   1]

Per ogni posizione (i, j):

Output(i, j) = Σ Σ [ immagine(i + k, j + l) * kernel(k, l) ]

dove:
- (k, l) scorre sugli indici del kernel
- il risultato viene salvato in `out` come valore intero a 32 bit

---

Scorrimento del kernel (stride = 1):

→ Il kernel si muove pixel per pixel da sinistra a destra e poi verso il basso:

riga 1: (0,0) → (0,1) → (0,2) → …  
riga 2: (1,0) → (1,1) → (1,2) → …

---

Se stride > 1, il kernel salta alcuni pixel:

stride = 2  → si sposta ogni 2 colonne e 2 righe

---

## 🔧 Compilazione con CMake

Assicurati di avere installato **OpenCV** e **CMake (≥ 3.10)**.  
Poi, dalla cartella principale del progetto, esegui i seguenti comandi:

```bash
mkdir build
cd build
cmake ..
make
```
---

## ▶️ Esecuzione

Dopo la compilazione, esegui il programma:
```
./main -i ../data/lena.png -t 0
```
---

### ⚙️ Argomenti disponibili

| Opzione | Descrizione |
|----------|-------------|
| `-i <nome_file>` | Specifica il percorso dell’immagine di input |
| `-t <ms>` | Imposta il tempo di attesa tra i frame (in millisecondi) |
| `-h` | Mostra il messaggio di aiuto con la descrizione dei parametri |

```
./main -i ../data/lena.png -t 100

```


---


### 🎮 Controlli da tastiera

Durante l’esecuzione del programma è possibile interagire tramite i seguenti comandi:

| Tasto | Azione |
|-------|---------|
| `q` | Esci dal programma |
| `s` | Diminuisci lo stride (movimento del kernel) |
| `S` | Aumenta lo stride |
| `k` | Cambia la dimensione del kernel (valori ciclici: 3, 5, 7, 11, 13) |
| `c` | Chiudi tutte le finestre aperte |
| `p` | Stampa sul terminale la matrice dell’immagine corrente |

---

## 🧠 Suggerimenti per il testing

Puoi confrontare i risultati del tuo filtro con le funzioni equivalenti di OpenCV:

```cpp
cv::filter2D(img, out_ref, CV_32S, krn);
cv::Sobel(img, out_sobel, CV_32S, 1, 0);
```

Per visualizzare correttamente le immagini con tipo CV_32S, utilizza:

```cpp
cv::Mat vis;
cv::convertScaleAbs(out, vis);
cv::imshow("visualized", vis);
```
---

## 📚 Obiettivi didattici

Questo progetto permette di:

- 🧮 Comprendere il funzionamento della convoluzione 2D.

- 🔍 Imparare ad accedere ai pixel in memoria senza usare at<T>().

- 🧰 Riprodurre e confrontare filtri lineari di base (Box, Sobel).

- 🧱 Organizzare un progetto C++ modulare con header, sorgenti e CMake.

- 🧩 Applicare concetti di elaborazione digitale delle immagini con OpenCV.



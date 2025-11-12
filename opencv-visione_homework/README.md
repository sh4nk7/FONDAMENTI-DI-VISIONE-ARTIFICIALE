# 🧠 Prova Pratica di Visione – Novembre 2025  
**Università degli Studi di Parma**  
Dipartimento di Ingegneria e Architettura  
Corso di Laurea Magistrale in Ingegneria Informatica  

---

## 📘 Testo della prova
> A partire dallo scheletro di codice fornito si sviluppi un programma che:
> 1. Prenda in ingresso la seguente immagine che contiene degli “spicchi” a toni di grigio sempre “crescenti” e della stessa dimensione angolare.  
> 2. Si supponga di numerare gli *edge* da 1 a x in senso orario, partendo dall’alto ed escludendo l’edge verticale.  
> 3. Scrivere una funzione che, ricevuta in ingresso una lista di indici nel range [1, x], crei una nuova immagine con solo i corrispondenti edge in evidenza.  
> 4. Ad esempio, per l’immagine in esame e input = {2,5,6}, l’output sarà un’immagine binaria con linee bianche sugli edge selezionati.  
> 5. Si modifichi quindi il codice per considerare anche immagini a 7 spicchi.  
> 
> **Consegna:**  
> Si carichi, quando richiesto, il solo codice C++ tramite il portale:  
> [http://www.ce.unipr.it/didattica/visione/homework.php](http://www.ce.unipr.it/didattica/visione/homework.php)

---

## 🧩 Descrizione del progetto

Questo repository contiene il codice C++ sviluppato per la prova pratica di Visione.  
Il progetto utilizza **OpenCV 4** per l’elaborazione di immagini e la gestione grafica.  

### Obiettivo
Data un’immagine divisa in “spicchi” di grigio, il programma individua e disegna in bianco solo i bordi corrispondenti a un insieme di indici scelti dall’utente.

L’immagine risultante è **binaria**:  
- 0 (nero) = sfondo  
- 255 (bianco) = linee richieste  

---

## 📁 Struttura della cartella

```bash
visione_homework/
│
├── vision.cpp # File principale (consegna)
├── simple.cpp # File del professore da modificare
├── 9spicchi.pgm # Esempio di immagine con 9 spicchi
├── 7spicchi.pgm # Esempio di immagine con 7 spicchi
└── README.md # 
```


---

## ⚙️ Requisiti

- Sistema operativo: Linux / macOS / WSL  
- Libreria: [OpenCV 4](https://opencv.org/releases/)  
  Installazione rapida su Ubuntu:
```bash
  sudo apt update
  sudo apt install libopencv-dev
```

## 🧠 Spiegazione del codice
vision.cpp

- Riceve i parametri da linea di comando (-i, -n, -s, -o).

- Carica l’immagine in scala di grigi.

- Esegue la funzione drawEdge(), che:

    1. Calcola l’angolo tra gli spicchi (360 / numSpicchi);

    2. Disegna linee bianche solo per gli indici passati;

    3. Genera un’immagine binaria (edges).

- Mostra a schermo le finestre:

    - image → immagine originale

    - edges → immagine con i bordi selezionati evidenziati

- Se specificato, salva il risultato su file (-o risultato.png).

## 🏗️ Compilazione

Da terminale, nella cartella del progetto:

Per il programma principale:

```bash
    g++ vision.cpp -o vision `pkg-config --cflags --libs opencv4`
```
  
## ▶️ Esecuzione

```bash
    g++ vision.cpp -o vision `pkg-config --cflags --libs opencv4`
```
    
## Eseguire il programma principale

```bash
    ./vision -i 9spicchi.pgm -n 9 -s 2 -s 5 -s 6
```

```bash
    ./vision -i 7spicchi.pgm -n 7 -s 1 -s 3 -s 5
```

## Salvare il risultato

```bash
    ./vision -i 9spicchi.pgm -n 9 -s 2 -s 5 -s 6 -o risultato.png

```
```bash
    ./vision -i 7spicchi.pgm -n 7 -s 1 -s 3 -s 5 -o risultato.png

```

## 🧪 Output atteso

- Immagine binaria con sfondo nero e linee bianche corrispondenti agli edge richiesti.

- Le linee partono dal centro e sono disposte in senso orario.

- Nessun altro contenuto deve essere disegnato.

```bash
Esempio visuale:

Immagine originale:     Immagine output:
   (spicchi grigi)      (solo linee bianche)
```

## 🧾 Autore

Studente: Giuseppe Dimonte
Matricola: 367431
Corso: Ingegneria Informatica – Università di Parma
Anno Accademico: 2025/2026

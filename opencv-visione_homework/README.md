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


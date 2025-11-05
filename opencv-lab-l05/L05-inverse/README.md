# 🧭 Laboratorio L05 — Inverse Perspective Mapping

## 📘 Descrizione  
Scopo del laboratorio: realizzare una **trasformazione prospettica inversa (IPM)** per ricavare le **coordinate del mondo** dei punti di un’immagine, assumendo che essi appartengano a un **piano noto** (ad esempio il suolo).  

L’obiettivo è comprendere come, a partire da un’immagine e dai parametri intrinseci ed estrinseci di una camera, sia possibile **proiettare i pixel dell’immagine** su un piano nel mondo 3D e ottenere così una **rappresentazione metrica**.

---

## 🧮 Teoria di riferimento

### 🔹 Proiezione prospettica diretta  
Un punto 3D `P = [X, Y, Z, 1]^T` nel mondo è proiettato sul piano immagine come:

s * [u v 1]^T = M * [X Y Z 1]^T


dove `M = K [R|t]` combina:
- `K` → matrice **intrinseca** (parametri della camera),
- `[R|t]` → **rotazione e traslazione** della camera rispetto al mondo.

---

### 🔹 Vincolo di planarità  
Poiché la ricostruzione 3D da una singola immagine è in generale impossibile, imponiamo che i punti appartengano a un piano `Π`:

Π : aX + bY + cZ + d = 0



Questo vincolo riduce la trasformazione prospettica a un caso invertibile **sul piano Π**.

---

### 🔹 Proiezione inversa  

Combinando la proiezione con il vincolo del piano otteniamo:

[ u v w 0 ]^T = [ M ; a b c d ] * [ X Y Z 1 ]^T


e invertendo:

[ X Y Z W ]^T = [ M ; a b c d ]^(-1) * [ u v 1 0 ]^T


Da cui si ricavano le coordinate euclidee del punto nel mondo:


X' = X / W
Y' = Y / W
Z' = Z / W



---

### 🔹 Caso particolare: piano Y = 0  

Nel caso di un piano orizzontale (ad esempio il terreno), imponiamo:

(a, b, c, d) = (0, 1, 0, 0)


In questo caso la matrice `M` si riduce a una **omografia 3×3** che lega il piano del mondo alle coordinate immagine:

[ u v w ]^T = M' * [ X Z 1 ]^T


e la trasformazione inversa diventa:

[ X Z 1 ]^T = (M')^(-1) * [ u v 1 ]^T


Questo permette di ricavare le coordinate reali (X, Z) dei punti sul piano Y=0 a partire dai pixel dell’immagine.

---

### 🔹 Schema del processo
```
 +---------------------------+
 |  Immagine (u,v)           |
 +-------------+-------------+
               |
               v
    [M = K [R|t]]  +  Piano Π : Y=0
               |
               v
 +---------------------------+
 |  Proiezione inversa (IPM) |
 +-------------+-------------+
               |
               v
      Coordinate mondo (X,Z)
```


---

## 📁 Struttura del progetto  

```
L05-inverse/
├── CMakeLists.txt
├── include/
│ └── utils.h
├── src/
│ └── main.cpp
├── data/
│ ├── input.png
│ └── camera_params.txt
└── out/
└── ipm_result.png
```

---

## ⚙️ Compilazione  

Richiede **OpenCV** e **Eigen3** installati.

```bash
mkdir build && cd build
cmake ..
make
```
---
## ▶️ Esecuzione
```
./lab5_2 ../data/input.png ../data/camera_params.txt
```

Argomenti:

- <image_filename> → immagine di input
- <camera_params_filename> → file dei parametri di camera

---

## 🧰 File principali

| File | Descrizione |
|------|--------------|
| `main.cpp` | Caricamento immagine e parametri, calcolo della matrice di proiezione e applicazione IPM |
| `utils.h` | Gestione parametri intrinseci/estrinseci, conversioni con Rodrigues e Affine3f |
| `CMakeLists.txt` | Configurazione di build con OpenCV e Eigen |
| `data/` | File di input (immagine e parametri della camera) |
| `out/` | Output dell’immagine trasformata (vista dall’alto, “bird’s eye view”) |

---
📤 Output atteso

- Finestra Input: mostra l’immagine originale.
- Finestra IPM: visualizza la trasformazione inversa prospettica, tipicamente una vista “bird’s eye” del piano selezionato (ad esempio la strada vista dall’alto).

L’immagine risultante rappresenta la proiezione metrica del piano definito.
---

👨‍🎓 Informazioni accademiche

Studente: Dimonte Giuseppe — MAT: 367431
Docente: Prof. Massimo Bertozzi
Corso di laurea: Laurea Magistrale in Ingegneria Informatica — Percorso Intelligenza Artificiale

---









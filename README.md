# 🌐 HTTP Server en C

Un **servidor HTTP minimalista** implementado en **C**, creado para comprender en detalle cómo funcionan las conexiones TCP/IP y el protocolo HTTP a bajo nivel.  
Este proyecto no utiliza librerías externas para manejar HTTP: todo se parsea y procesa manualmente.

---

## ✨ Características

- 📡 Servidor multicliente mediante **threads** (`pthread`).
- 📄 Parseo de la **línea de inicio** (`Método`, `Ruta`, `Versión`).
- 📝 Extracción y almacenamiento dinámico de **headers HTTP**.
- 📬 Respuestas configurables con `HttpResponse`.
- ⚡ Endpoints de ejemplo:
  - `/` → retorna *Hello World*.
  - `/time` → retorna la fecha y hora actual.
  - Otros → `404 Not Found`.
- 🧩 Código modular dividido en:
  - `server.c/h` → lógica del servidor HTTP.
  - `sockets.c/h` → manejo de sockets TCP.
  - `client.c/h` → cliente de prueba.

---

## 📂 Estructura del proyecto

HTTP-SERVER-IN-C
├── bin/ # Binarios compilados
├── obj/ # Archivos objeto intermedios
├── src/ # Código fuente
│ ├── client.c
│ ├── client.h
│ ├── server.c
│ ├── server.h
│ ├── sockets.c
│ └── sockets.h
├── .gitignore
├── Makefile
└── README.md


---

## ⚙️ Compilación y ejecución

Compilar el proyecto:

```bash
make

Esto generará los binarios en bin/.

Ejecutar el servidor (por defecto en puerto 8000):

./bin/server [PORT]
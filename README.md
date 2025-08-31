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

- HTTP-SERVER-IN-C
- ├── bin/ # Binarios compilados
- ├── obj/ # Archivos objeto intermedios
- ├── src/ # Código fuente
- │ ├── client.c
- │ ├── client.h
- │ ├── server.c
- │ ├── server.h
- │ ├── sockets.c
- │ └── sockets.h
- ├── .gitignore
- ├── Makefile
- └── README.md


---

## ⚙️ Compilación y ejecución

1. Compilar el proyecto:

```bash
make
```
Esto generará los binarios en bin/.

2. Ejecutar el servidor (por defecto en puerto 8000):

```bash
./bin/server [PORT]
```

3. Una vez levantado el servidor, enviar la request:
```bash
curl http://localhost:[PORT]/
curl http://localhost:[PORT]/time
curl http://localhost:[PORT]/lalala
```

4. Esperar la respuesta, ej:
```bash
HTTP/1.1 200 OK
Content-Length: 16
Content-Type: text/plain
Connection: close

Hello, World!
```

## 🛠️ Tecnologías utilizadas

- Lenguaje: C (C99)

- Sockets: POSIX TCP/IP

- Concurrencia: pthreads

- Entorno: Linux (probado en Ubuntu/Pop!_OS, debería funcionar en WSL2/macOS).

## 📜 Licencia

Podés usarlo, modificarlo y aprender de él sin restricciones.

## 👨‍💻 Autor

Desarrollado por Lucas 🧑‍💻
Estudiante de Ingeniería en Sistemas, interesado en programación de bajo nivel, redes y sistemas distribuidos.
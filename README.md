# High-Performance Secure Data Transfer Engine (Linux)

A **high-throughput, low-latency, multi-threaded secure data transfer engine** implemented in **C/C++ on Linux** using **TCP/IP sockets**, **AES-128 encryption (OpenSSL EVP)**, and **performance benchmarking**. The system is optimized with **low-copy / zero-copy–inspired techniques** and validated via **stress testing with 100+ concurrent clients**.

---

## ✨ Key Features
- **Multi-threaded TCP server** using pthreads (concurrent client handling)
- **AES-128 encrypted payloads** via OpenSSL EVP
- **Latency (RTT) & throughput (MB/s) benchmarking**
- **Low-copy optimizations** (direct buffers, reduced allocations, fewer syscalls)
- **TCP tuning** (`TCP_NODELAY`, `MSG_NOSIGNAL`)
- **Stress testing** with 100+ concurrent clients
- Clean, modular project structure with Makefile

---

## 🧠 Architecture (High-Level)
```
Client(s)
  └─ Payload (buffer)
      └─ AES Encrypt
          └─ TCP Send  ───────────────▶  Server (listen)
                                           ├─ accept()
                                           ├─ thread per client
                                           ├─ TCP recv (direct buffer)
                                           ├─ AES Decrypt
                                           └─ ACK → Client
```

---

## 📁 Project Structure
```
secure_transfer_engine/
├── Makefile
├── README.md
├── src/
│   ├── server.cpp
│   ├── client.cpp
│   ├── stress_client.cpp
│   ├── crypto.cpp
│   └── metrics.cpp
├── include/
│   ├── crypto.h
│   └── metrics.h
├── bin/
├── build/
└── tests/
```

---

## ⚙️ Build & Run

### Prerequisites
- Ubuntu / Linux
- `g++`, `make`
- OpenSSL dev libs: `sudo apt install libssl-dev`

### Build
```bash
make clean
make
make stress
```

### Run
```bash
# Terminal 1
./bin/server

# Terminal 2
./bin/client

# Stress test (Terminal 2)
./bin/stress_client
```

---

## 📊 Benchmarking
- **Latency (RTT):** measured client-side using high-resolution timers
- **Throughput:** computed from payload size and RTT
- **Aggregate throughput:** measured during stress tests with concurrent clients

> Note: Localhost results are intentionally high; tune payload size and client count to simulate real workloads.

---

## 🔐 Security
- **AES-128-CBC** encryption using OpenSSL EVP
- Clear crypto abstraction layer
- Encrypted payloads end-to-end

---

## 🛠️ Tech Stack
- **Languages:** C, C++
- **OS:** Linux (POSIX)
- **Networking:** TCP/IP sockets
- **Security:** OpenSSL (EVP, AES)
- **Concurrency:** pthreads / std::thread
- **Build:** Make, GCC/G++
- **Tools:** Git, GDB

---

## 🎯 Why This Project
This project mirrors **data-path engineering** and **secure networking** concerns found in data-center and silicon-adjacent systems:
- Networking protocols
- Cryptography integration
- Linux system programming
- Performance & scalability

---

## 👤 Author

**Yash Raj**  
Software Engineer with strong interest in Linux system programming, secure networking, and performance-critical C/C++ systems. Focused on building scalable, low-latency, and secure data-path software.

- GitHub: https://github.com/algorithmist-yash  
- LinkedIn: https://www.linkedin.com/in/yash-raj-476290369/


---

## 📄 License

This project is licensed under the [MIT License](LICENSE).


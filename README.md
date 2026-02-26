# PointToPoint_DataLink_Communication

## Project Overview

**PointToPoint_DataLink_Communication** is a C-based implementation that simulates **Data Link Layer communication** in a point-to-point network environment.

This project demonstrates how frames are created, transmitted, and received between nodes using two different simulation models:

* File-Based Communication
* Socket-Based Communication

---

## Objective

The main objectives of this project are:

* To simulate frame-based communication between nodes
* To understand Data Link Layer functionality
* To implement controlled communication between nodes
* To compare file-based and socket-based communication models
* To gain hands-on experience with C programming and networking

---

## Project Structure

```
PointToPoint_DataLink_Communication/
│
├── file_simulation/
│   ├── frame.h
│   ├── frame.c
│   ├── node_a.c
│   ├── node_b.c
│   └── node_c.c
│
├── socket_simulation/
│   ├── frame.h
│   ├── frame.c
│   ├── node_a.c
│   ├── node_b.c
│   └── node_c.c
│
└── README.md
```

---

# File-Based Simulation

## Description

In this simulation model, communication between nodes is performed using files.

Instead of sending frames over a network, the sender writes data into a file, and the receiver reads from that file. This simulates frame transmission without using actual networking.

This approach helps understand the logical working of:

* Framing
* Data transfer
* Sequential communication
* Controlled message passing

---

## Compilation (File Simulation)

Step 1: Navigate to the folder

```bash
cd file_simulation
```

Step 2: Compile the programs

```bash
gcc frame.c node_a.c -o node_a
gcc frame.c node_b.c -o node_b
gcc frame.c node_c.c -o node_c
```

---

## Execution (File Simulation)

Open **three separate terminals** and run:

Terminal 1:

```bash
./node_a
```

Terminal 2:

```bash
./node_b
```

Terminal 3:

```bash
./node_c
```

This simulates point-to-point communication using file-based frame exchange.

---

# Socket-Based Simulation

## Description

In this simulation model, communication between nodes is performed using TCP sockets.

Each node runs as a separate process and communicates over localhost. This model closely resembles real network communication and demonstrates practical socket programming in C.

This simulation helps understand:

* Socket creation
* Connection establishment
* Frame transmission over TCP
* Real-time communication

---

## Compilation (Socket Simulation)

Step 1: Navigate to the folder

```bash
cd socket_simulation
```

Step 2: Compile the programs

```bash
gcc frame.c node_a.c -o node_a
gcc frame.c node_b.c -o node_b
gcc frame.c node_c.c -o node_c
```

---

## Execution (Socket Simulation)

Open **three separate terminals**:

Terminal 1:

```bash
./node_a
```

Terminal 2:

```bash
./node_b
```

Terminal 3:

```bash
./node_c
```

The nodes will establish socket connections and simulate frame transmission over TCP.

---

## Key Concepts

* Data Link Layer (OSI Layer 2)
* Frame Structure
* Point-to-Point Communication
* Inter-process Communication
* File Handling in C
* Socket Programming in C
* Practical Networking Concepts

---

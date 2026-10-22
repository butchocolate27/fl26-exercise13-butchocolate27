# 3574 Exercise 13: QSharedMemory

Use `QSharedMemory` for inter-process communication: two separate programs share one queue in memory.

## What's here

| File | Role |
|---|---|
| `shared_ring_buffer.h` / `.tpp` | `SharedRingBuffer<T>`: a queue (ring buffer) that lives in a `QSharedMemory` segment. Every access is between `lock()` and `unlock()`. |
| `qt_shared_deque_1.cpp` | **Consumer.** Waits until the queue has data, dequeues one integer, prints it, exits. |
| `qt_shared_deque_2.cpp` | **Producer.** Repeatedly reads integers from the keyboard and enqueues them while the queue has space. |
| `CMakeLists.txt` | Builds both programs. |
| `dockerfile` | Ubuntu 24.04 with Qt 6 and CMake. |

## Changes and observations

The producer now keeps prompting while the queue has space (up to 100 integers).
It checks the input and the result of `enqueue()` before printing confirmation.
EOF or input that is not an integer ends the producer.

For step 9, I started the producer first and entered `12`, `-7`, and `0`.
Starting the consumer printed `Dequeued: 12`, then the consumer exited.
Running the consumer two more times printed `Dequeued: -7` and `Dequeued: 0`.
The numbers were stored in the shared queue and retrieved in FIFO order.
The producer stayed running during this test so the shared memory remained attached.

I also checked that the producer stops accepting input when 100 numbers are queued
and continues after the consumer frees a slot.

## Build

From this folder on your computer (PowerShell, macOS or Linux):

```bash
docker build -t ece3574 -f dockerfile .
docker run -it --name ex13 -v "${PWD}:/work" -w /work ece3574 bash
```

Inside the container:

```bash
cmake -S . -B build
cmake --build build
```

If you ever get a CMake error about `CMakeCache.txt` being in a different directory, delete the `build` folder and run both commands again.

## Run: both programs in the SAME container

Shared memory is private to each container. If you start the two programs with two separate `docker run` commands, they will never see each other and the consumer will wait forever.

**Terminal 1** (the one from `docker run` above):

```bash
./build/qt_shared_deque_1
```

**Terminal 2**: open a new terminal on your computer and attach to the same container:

```bash
docker exec -it ex13 bash
cd /work
./build/qt_shared_deque_2
```

Type a number in terminal 2 and watch terminal 1. The consumer prints one number
and exits. Run it again to dequeue another number.

To test step 9, start the producer first, enter several integers, and keep it
running. Then start the consumer in the other terminal.

## Troubleshooting

**The producer doesn't prompt, or old numbers show up.**
Stopping a program with Ctrl+C skips its cleanup, so the shared segment from the last run is still there with old data. Clear it:

```bash
ipcs -m              # list shared-memory segments
ipcrm -m <shmid>     # remove one by its shmid
```

Or start fresh: `exit` both terminals, then `docker rm -f ex13` and run the `docker run` command again.

**`docker run` says the name `ex13` is already in use.**
A container from last time still exists. Remove it with `docker rm -f ex13`, or attach to it with `docker start -ai ex13`.

# Exercise 13: QSharedMemory

I changed `qt_shared_deque_2.cpp` so it keeps reading integers from standard input while the buffer has space. The buffer holds up to 100 integers. When it is full, the producer waits until a slot is available. Invalid input or EOF ends the program.

Build the Docker image and open the first shell:

```bash
docker build -t ece3574-ex13 -f dockerfile .
docker run -it --name ex13 -v "${PWD}:/work" -w /work ece3574-ex13 bash
```

Inside the container, build both programs and start the producer:

```bash
cmake -S . -B build
cmake --build build
./build/qt_shared_deque_2
```

Enter a few integers and leave the producer running. Open another terminal and start the consumer in the same container:

```bash
docker exec -it -w /work ex13 bash
./build/qt_shared_deque_1
```

## Step 9

I entered `12`, `-7`, and `0` before starting the consumer. The consumer printed `Dequeued: 12` and exited. Running it two more times printed `Dequeued: -7` and `Dequeued: 0`.

The numbers stayed in the shared buffer and came out in the same order they were entered. The consumer only reads one number each time it runs because it still has the original `break` statement.

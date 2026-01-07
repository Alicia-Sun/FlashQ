# Description
This is a low latency distributed MPMC Queue
# Usage
./main [-i node_id] [-n queue_size] [-s max_msg_payload_size] <config_file>  
ex. "./main -i 1 -n 5 -s 3 configs/config1.json"
  

# Notes  

Ideas:  
-batching+timer either internally in queue or in the producer/consumer nodes  
    -worses latency per message but better throughput  
-uses raw TCP; no serialization overheads like gRPC
-slab allocation with array  
-lock-free ring buffer with ptrs  
-fixed size messages that are reused  
-avoids strings  
-mmaps log file to memory  
  
-what is epoll / io_uring?


# Structure
1. Data layout
- Queue state
    - std::atomic<size_t> head; // consumers claim positions
    - std::atomic<size_t> tail; // producers claim positions
    - mask = N - 1
    - Initialized head = tail = 0
- Each Slot
    - carries some atomic "ticket" (std::atomic<size_t> seq;) telling you if ready to be written to. It essentially stores the next global position this slot is expecting
    - T data;
    - Initialized as seq.store(i, relaxed);

```
Think of tail and head as ever-increasing “global positions” (0,1,2,…), not indices. The ring index = pos & mask. pos is the global only increasing index that maps to ring index.
```
- ex. ring of size N has indices 0,1,2,3 so pos=4 would map to 0. seq in this example is initialized as 0,1,2,3 meaning ready for producer pos 0,1,2,3

2. Protocol walk through
- First increment the pos (either head/tail), then complete the operation and only after you commit by publishing the mapped slot's new seq.

- ex. producer claims pos = 0
- index is 0 & 3 = 0 so maps to ring index 0 as expected. Finishes the operation by setting slot[0].seq to pos + 1.
- Think of 
```
pos     → empty / writable only
pos + 1 → full / readable only
```

- consumer claims pos=0
- maps to index 0 and they see seq is set to 1, which matches what they expect (pos + 1) so they can read it
- Free the slot by setting slot[0].seq = pos + N = 4 since must wraparound to next producer
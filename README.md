# Description
This is a low latency distributed MPMC Queue
# Usage
./main [-i node_id] [-n queue_size] [-s max_msg_payload_size] <config_file>  
ex. "./main -i 1 -n 5 -s 3 configs/config1.json"
  
# Notes
Ignore producer and consumer nodes for now. May want to implement  
the extra network hop as an option in the future to add safety,  
although this would increase latency of queue.
  

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

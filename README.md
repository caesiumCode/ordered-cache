# ordered-cache
Implementation of several cache data structures that maintain ordering on the key. In particular, Move-to-root tree (introduced by [Allen and Munro](https://dl.acm.org/doi/10.1145/322092.322094)) and Attach-and-splay tree are the main components of a paper I co-authored ([not available yet]()). The purpose of this program is to evaluate the performance of different cache data structures. Here are the implemented data structures:

| Name | Codename | Restructuring Heuristic | Leaf Queue
| - | - | - | - |
| Splay Tree | ```st``` | promote to the root with double rotations | yes
| Light Splay Tree | ```lst``` | promote to the root with double rotations | no
| Attach-and-splay Tree | ```ast``` | single rotation then promote to the root with double rotations | yes
| Move-to-root Tree | ```mtr``` | promote to the root with single rotations | yes
| Simple Exchange Tree | ```se``` | one single rotation | yes

An LRU cache is also implemented (Codename ```lru```) and a fixed cache (Codename ```fc```) where the container is untouched once it is full.

A side program is also available to compute some statistics on the input streams.

# Compilation

Evaluate a data structure:
```g++ orderd-cache/*.cpp -o ocds -O3```

Compute statistics of a stream:
```g++ dataset-analysis/main.cpp -o dsa -O3```

# Dataset
The input stream must be of the following format (one key per line):
```
<string>
<string>
<string>
...
<string>
```
# Execution

| Parameter | Type | Description |
| - | - | - |
| ```<path>``` | string | path the dataset folder (must end with "/") |
| ```<dataset>```| string | filename of the dataset (must be inside ```<path>```) |
| ```<n>``` | int | maximum number of key stored |
| ```<model>``` | string |  codename of the model |
| ```<ranking>``` | ```0``` or ```1``` | ```1``` to activate tracking mode |
| ```<prefix>``` | ```0``` or ```1``` | ```1``` to activate the prefix queries |
| ```<prefix_offset>``` | int | optional: offset of the end of the prefix |

The purpose of the tracking mode is to track specifically the distinct age of the deleted keys and the size of the leaf queue. Tracking the distinct age is particularly time consuming and therefore not computed by default. When the prefix queries are activated, the default prefix is defined as the smallest prefix that contains the last occurence of ```/``` in the key. If there is no ```/```, the entire key is taken as prefix. The prefix offset indicates how much it is exdented on the right. For example with the key ```fold1/fold2/file.ext```, the default prefix would be ```fold1/fold2/```, and with a prefix offset of 2 the prefix would be ```fold1/fold2/fi```. If omitted, the prefix offset is set to ```0```.

### Evaluate a data structure
```./ocds <path> <dataset> <n> <model> <tracking> <prefix> <prefix_offset> ```

### Compute statistics of a stream
```./dsa <path> <dataset>```

# Output
### Evaluate a data structure
#### Without tracking mode activated
There is one line

```<dataset> <model> <n> <size (bytes)> <hit rate> <latency (us)> <cache hit latency (us)> <avg. prefix hits> <prefix latency (us)>```

#### With tracking mode activated
There are two lines

```<dataset> <model> <n> rank <array of 101 integers>```

```<dataset> <model> <n> leafset <array of 501 integers>```

The first line display an array R such that R[i] is the number of time the distinct age of the deleted node is (i $\pm$ 0.5)% of n.
The second line display an array L such that L[i] is the number of time the size of the leaf queue is (i $\pm$ 0.5)‰ of n.


### Compute statistics of a stream
There is one line

```<dataset> <number of distinct keys> <number of keys> <avg. key size>```


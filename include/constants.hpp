#ifndef CONSTANTS
#define CONSTANTS

namespace constants {
    /*
     * General constants
     */

    const int NUM_THREADS = 4;
    /*
     * mstd::logger constants
     */
    const int STDOUT = 0x0;
    const int LOGFILE = 0x1;
    const int BOTH_STREAMS = 0x2;

    const int DEFAULT_STREAM = BOTH_STREAMS;

    /*
     * bloom_filter constants
     */
    const size_t BLOOM_SIZE = 34816;
    const size_t BLOOM_K = 23;

    /*
     * trie constants
     */
    const size_t TRIE_CHILDREN_INIT_SIZE = 3;

    /*
     * linear hash table
     */
    const size_t LH_INIT_SIZE = 4;
    const size_t LH_MAX_BUCKET_SIZE = 4;
    const int LH_LOAD_FACTOR = 90;
}
#endif // CONSTANTS
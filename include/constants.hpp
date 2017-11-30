#ifndef CONSTANTS
#define CONSTANTS

/*
 * mstd::logger constants
 */
#define STDOUT 0x0
#define LOGFILE 0x1
#define BOTH_STREAMS 0x2

#define DEFAULT_STREAM BOTH_STREAMS

/*
 * bloom_filter constants
 */
#define BLOOM_SIZE 34816
#define BLOOM_K 23

/*
 * trie constants
 */
#define CHILDREN_INITIAL_SIZE 3

/*
 * linear hash table
 */
#define LINEAR_HASH_INITIAL_SIZE 4
#define LINEAR_HASH_MAX_BUCKET_SIZE 4
#define LINEAR_HASH_LOAD_FACTOR 90
#endif // CONSTANTS
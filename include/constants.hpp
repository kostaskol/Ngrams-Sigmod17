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
#define MURMUR3 0x0
#define JENKINS_PEARSONS 0x1
#define DEFAULT_HASH MURMUR3

#define BLOOM_SIZE 24576
#define BLOOM_K 3

/*
 * trie constants
 */
#define CHILDREN_INITIAL_SIZE 3

/*
 * linear hash table
 */
#define LINEAR_HASH_INITIAL_SIZE 4
#define LINEAR_HASH_MAX_BUCKET_SIZE 4
#define LINEAR_HASH_LOAD_FACTOR 75
#endif // CONSTANTS

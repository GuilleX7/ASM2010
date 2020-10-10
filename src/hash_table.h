/** @file hash_table.h */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

#define CHT_MIN_LOAD_FACTOR 0.15
#define CHT_MAX_LOAD_FACTOR 0.75

/** @brief Struct representing the hash table */
struct hash_table {
	/** @brief Size index of the array of pointers to entries */
	size_t size_index;
	/** @brief Number of entries in the hash table */
	size_t entry_count;
	/** @brief Array of pointers to entries */
	struct hash_table_entry **entries;
};
typedef struct hash_table hash_table;

/** @brief Struct representing an entry in the hash table */
struct hash_table_entry {
	/** @brief Pointer to string containing the key of the pair */
	char *key;
	/** @brief Pointer to the object stored in the pair*/
	void *val;
	/** @brief Pointer to the next entry in the hash table */
	struct hash_table_entry *next;
};

/** @brief Struct representing a hash table iterator */
struct hash_table_iterator {
	/** @brief Pointer to the iterated hash table */
	struct hash_table *hash_table;
	/** @brief Pointer to the actual entry */
	struct hash_table_entry *entry;
	/** @brief Next index in the array of entries */
	size_t next_index;
};

/**
	@brief Initializes an existing hash table structure
	@returns Non-zero value if initialization succeded,
			0 otherwise
*/
int hash_table_init(struct hash_table *hash_table);

/**
	@brief Frees a hash table
	@param hash_table Pointer to the hash_table to be freed
*/
void hash_table_free(struct hash_table *hash_table);

/**
	@brief Put key-value pair into a hash table
	@param ht Pointer to hash_table where pair will be inserted
	@param key Pointer to string used as key. Will be copied in memory.
	@param val Pointer to the object to be stored. Will be copied in memory
	@param size Size of value in bytes
	@return Non-zero value if pair was stored successfully,
			0 otherwise
*/
int hash_table_put_sized(struct hash_table *hash_table, char *key, void *val, size_t val_size);

/**
	@brief Put key-value pair into a hash table
	@param hash_table Pointer to hash_table where pair will be inserted
	@param key Pointer to string used as key. Will be copied in memory.
	@param val Pointer to the object to be stored. Will be copied in memory.
			Size will be obtained using sizeof operator. If storing
			a opaque type, see hash_table_put_sized()
	@return Non-zero value if the pair was stored successfully,
			0 otherwise
*/
#define hash_table_put(hash_table, key, val) hash_table_put_sized(hash_table, key, val, sizeof *val)

/**
	@brief Get the value associated to a key
	@param hash_table Pointer to hash_table to search in
	@param key Pointer to string that will be searched for
	@return Pointer to value if found,
			null pointer otherwise
*/
void *hash_table_get(struct hash_table *hash_table, char *key);

/**
	@brief Removes an entry from the hash table if it exists
	@param hash_table Pointer to hash_table to search in
	@param key Pointer to string that will be deleted
	@return Non-zero value if the pair was removed successfully,
			0 otherwise
*/
int hash_table_remove(struct hash_table *hash_table, char *key);

/**
	@brief Check if a key already exists
	@param hash_table Pointer to hash_table to search in
	@param key Pointer to string that will be searched for
	@return Non-zero value if key was found,
			0 otherwise
*/
int hash_table_exists(struct hash_table *hash_table, char *key);

/**
	@brief Initializes a hash table iterator to iterate a given
	hash table
	@param hash_table Pointer to the hash table to be iterated over
	@return Iterator for the given hash table
*/
struct hash_table_iterator hash_table_iterator_create(struct hash_table *hash_table);

/**
	@brief Returns the next pair in the hash table pointed by
	the iterator
	@param iterator Pointer to an initialized iterator
	@return Pointer to the next pair in the iteration,
			or NULL if no pair left
*/
struct hash_table_entry *hash_table_iterator_next(struct hash_table_iterator *iterator);

#endif /*HASH_TABLE_H */

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
    /** @brief Pointer to the string containing the key of the entry */
    char *key;
    /** @brief Pointer to the value stored in the entry */
    void *value;
    /** @brief Pointer to the next linked entry */
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
    @param hash_table Pointer to the hash table to be freed
*/
void hash_table_free(struct hash_table *hash_table);

/**
    @brief Put an entry into a hash table
    @param ht Pointer to the hash table where the entry will be inserted
    @param key Pointer to the string used as the key of the entry. Will be
copied in memory.
    @param value Pointer to the value to be stored. Will be copied in memory
    @param value_size Size of the value in bytes
    @return Non-zero value if the entry was inserted successfully,
                    0 otherwise
*/
int hash_table_put_sized(struct hash_table *hash_table, char *key, void *value, size_t value_size);

/**
    @brief Put an entry into a hash table
    @param ht Pointer to the hash table where the entry will be inserted
    @param key Pointer to the string used as the key of the entry. Will be
copied in memory.
    @param value Pointer to the value to be stored. Will be copied in memory
                    The value's size will be obtained using sizeof operator.
If the value's type is opaque (i.e. a pointer to a complex struct) and can't
be obtained using sizeof, see hash_table_put_sized()
    @return Non-zero value if the entry was stored successfully,
                    0 otherwise
*/
#define hash_table_put(hash_table, key, value) hash_table_put_sized(hash_table, key, value, sizeof *value)

/**
    @brief Get the value associated to a key
    @param hash_table Pointer to the hash table to search in
    @param key Pointer to the string to look for
    @return Pointer to the associated value if found,
                    null pointer otherwise
*/
void *hash_table_get(struct hash_table *hash_table, char *key);

/**
    @brief Removes an entry from the hash table if it exists
    @param hash_table Pointer to the hash table to search in
    @param key Pointer to the string that will be removed
    @return Non-zero value if the entry was removed successfully,
                    0 otherwise
*/
int hash_table_remove(struct hash_table *hash_table, char *key);

/**
    @brief Check if a key already exists in a hash table
    @param hash_table Pointer to the hash table to search in
    @param key Pointer to the string to look for
    @return Non-zero value if key was found,
                    0 otherwise
*/
int hash_table_exists(struct hash_table *hash_table, char *key);

/**
    @brief Initializes a hash table iterator for a given hash table
    @param hash_table Pointer to the hash table to be iterated over
    @return Iterator for the given hash table
*/
struct hash_table_iterator hash_table_iterator_create(struct hash_table *hash_table);

/**
    @brief Returns the next entry in the hash table pointed by
    the iterator
    @param iterator Pointer to an initialized iterator
    @return Pointer to the next entry in the iteration,
                    or NULL if no entries left
*/
struct hash_table_entry *hash_table_iterator_next(struct hash_table_iterator *iterator);

#endif /*HASH_TABLE_H */

# set - Custom Hash Set in C++

`Algorithms and DataStructures - Set` is a custom implementation of a hash-based set container in C++, similar to `std::unordered_set`. It provides fast insertions, lookups, and deletions using a dynamically managed hash table with chaining for collision resolution.  

This project was developed as a **study project** to deepen the understanding of **data structures**, particularly **hash tables, memory management, and iterator design** in C++. By implementing this data structure from scratch, we explore the core concepts behind efficient storage and retrieval mechanisms.

## Features

- **Hash-based storage** using **separate chaining** for collision resolution.
- **Dynamic resizing** with load factor control to maintain performance.
- **Custom iterators** for efficient element traversal.
- **Exception safety** with RAII (Resource Acquisition Is Initialization).
- **Operator overloads** for ease of use (`==`, `!=`, `swap`).

## Key Methods & Functionality
- **Insertion & Lookup**
  - `void insert(const Key &key)`: Adds an element if it doesn't already exist.
  - `size_type count(const Key &key) const`: Checks if an element is present.
  - `iterator find(const Key &key) const`: Returns an iterator to the element if found.
- **Deletion**
  - `size_type erase(const Key &key)`: Removes an element if it exists.
  - `void clear()`: Empties the entire set.
- **Performance Optimization**
  - `void reserve(size_type n)`: Increases the table size if necessary.
  - `void rehash(size_type n)`: Rebuilds the table to a new size.
- **Iterators**
  - Supports forward iteration over stored elements using `begin()` and `end()`.
  - Implements a custom `Iterator` class to traverse elements efficiently.

## Implementation Details
- **Hash Function**: Uses `std::hash<Key>` to determine bucket placement.
- **Collision Handling**: Uses linked lists (separate chaining) within each bucket.
- **Memory Management**:
  - Each bucket points to a linked list of nodes.
  - Nodes are dynamically allocated and properly cleaned up.
  - `delete[]` is used for table management, and `delete` for node cleanup.

## Study Objective

This project was part of a **self-study** to gain hands-on experience in **data structures and algorithms**. The primary learning objectives included:

- Understanding **hash table implementations** and collision resolution techniques.
- Managing **dynamic memory allocation** and avoiding memory leaks.
- Designing **iterators** for custom containers.
- Exploring **performance trade-offs** between different data structures.

By implementing this from scratch, the project bridges theoretical concepts with practical programming.

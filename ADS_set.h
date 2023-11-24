#ifndef ADS_SET_H
#define ADS_SET_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>

template <typename Key, size_t N = 12>
class ADS_set {
public:
	class Iterator;
	using value_type = Key; 
	using key_type = Key;   
	using reference = value_type &;
	using const_reference = const value_type &;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;
	using const_iterator = Iterator;
	using iterator = const_iterator; 
	using key_equal = std::equal_to<key_type>;                       // Hashing
	using hasher = std::hash<key_type>;                              // Hashing
private:
	struct Node {
		key_type key;
		Node *next {nullptr};
		~Node() { 
			delete next;
			next = nullptr;
		} 
	};

	Node **table {nullptr};
	size_type table_size {N};
	size_type current_size {0};
	float max_lf {0.7};
  
	Node *add(const key_type &key);
	Node *locate(const key_type &key) const;
	size_type h(const key_type &key) const;

	void reserve(size_type n);
	void rehash(size_type n);

public:
	ADS_set() : table(new Node*[N]()), table_size(N), current_size(0) {}
	ADS_set(std::initializer_list<key_type> ilist): ADS_set{} { insert(ilist); }
	template<typename InputIt> ADS_set(InputIt first, InputIt last): ADS_set{} { insert(first,last); }
	ADS_set(const ADS_set &other); 
	~ADS_set() {
		for (size_type i {0}; i < table_size; i++) {
			delete table[i];
		}
		delete[] table;
	}
	
	ADS_set &operator=(const ADS_set &other);
	ADS_set &operator=(std::initializer_list<key_type> ilist);

	size_type size() const { return current_size; }
	bool empty() const { return current_size == 0; }
	void insert(std::initializer_list<key_type> ilist) { insert(ilist.begin(), ilist.end()); }

	std::pair<Iterator,bool> insert(const key_type &key);

	template<typename InputIt> void insert(InputIt first, InputIt last); 

	void clear(); 

	size_type erase(const key_type &key);
	size_type count(const key_type &key) const {  return locate(key) != nullptr; }
	iterator find(const key_type &key) const;

	void swap(ADS_set &other) {
  		using std::swap;	
  		swap(table, other.table);
  		swap(table_size, other.table_size);
  		swap(current_size, other.current_size);
  		swap(max_lf, other.max_lf);
	}
	const_iterator begin() const {	return const_iterator{this, 0, nullptr}; }
	const_iterator end() const { return const_iterator{this, table_size, nullptr }; }
  	void dump(std::ostream &o = std::cerr) const;

	friend bool operator==(const ADS_set &lhs, const ADS_set &rhs) {
		if(lhs.current_size != rhs.current_size) return false;
		for (const auto &l : lhs) {
			if(!rhs.count(l)) return false;
		}
		return true;
	}
	friend bool operator!=(const ADS_set<Key,N> &lhs, const ADS_set<Key,N> &rhs)  { return !(lhs == rhs); }
};

template <typename Key, size_t N>
ADS_set<Key,N>::ADS_set(const ADS_set &other) {

	table_size = other.table_size; 
	table = new Node*[table_size]();

  	for (const auto &k : other) { 
		add(k);
 	}
}

template <typename Key, size_t N>
ADS_set<Key,N> &ADS_set<Key,N>::operator=(const ADS_set &other) {
	ADS_set tmp{other}; 
	swap(tmp);
	return *this;
}

template <typename Key, size_t N>
ADS_set<Key,N> &ADS_set<Key,N>::operator=(std::initializer_list<key_type> ilist) {
	ADS_set tmp{ilist};
	swap(tmp);
	return *this;
}

template <typename Key, size_t N>
template<typename InputIt> void ADS_set<Key,N>::insert(InputIt first, InputIt last) {
	for(auto it {first}; it != last; ++it) {
		if (!count(*it)) {
			reserve(current_size+1);
			add(*it);
		}
	}
}

template <typename Key, size_t N>
std::pair<typename ADS_set<Key,N>::Iterator,bool> ADS_set<Key,N>::insert(const key_type &key) {
	if (auto e {locate(key)}) return { Iterator{this,h(key),e}, false };
	reserve(current_size+1);
	return { Iterator{this, h(key), add(key)}, true };
}

template <typename Key, size_t N>
void ADS_set<Key,N>::clear() { 
	ADS_set tmp;
	swap(tmp);
}

template <typename Key, size_t N>
typename ADS_set<Key,N>::size_type ADS_set<Key,N>::erase(const key_type &key) {
	
	size_type index {h(key)};
	Node * previous_node {nullptr};
	Node* current_node {table[index]};

	while (current_node != nullptr) { 
		if(key_equal{}(key, current_node->key)) { 
			if(previous_node == nullptr) {
				table[index] = current_node->next;
			} 
			else { previous_node->next = current_node->next; }
			
			current_node->next = nullptr;
			delete current_node;
			--current_size;
			return 1; 
		}
		else { 
			previous_node = current_node;
			current_node = current_node->next; 
		}
	} 
	return 0; 
}

template <typename Key, size_t N>
typename ADS_set<Key,N>::Node *ADS_set<Key,N>::add(const key_type &key) {
	size_type hashedIndex {h(key)}; 
	++current_size;
	if(current_size / table_size >= max_lf) { reserve(table_size*2); }
	Node *new_node {new Node{key, table[hashedIndex]}};
	table[hashedIndex] = new_node;
	return table[hashedIndex];
}

template<typename Key, size_t N>
typename ADS_set<Key,N>::Node *ADS_set<Key,N>::locate(const key_type &key) const {
	size_type index {h(key)};
	Node* current_node {table[index]};
	while (current_node != nullptr) { 
		if(key_equal{}(key, current_node->key)) { 
			return current_node; 
		}
		else { current_node = current_node->next; }
	} 
	return nullptr; 
}

template <typename Key, size_t N>
typename ADS_set<Key,N>::size_type ADS_set<Key,N>::h(const key_type &key) const { 
	if(table_size == 0) {throw std::runtime_error("error: table_size cant be 0, modulo 0 isnt allowed");};
	return hasher{}(key) % table_size; 
}

template <typename Key, size_t N>
void ADS_set<Key,N>::dump(std::ostream &o) const {
  o << "table_size = " << table_size << ", current_size = " << current_size << "\n";

  for (size_type idx {0}; idx < table_size; idx++) {
  	Node *current_node {table[idx]};

	o << "[ " << idx << " ] : ";
	while(current_node != nullptr){
		o << current_node->key << ", ";
		current_node = current_node->next;
	}
	o << '\n';
    }
}

template <typename Key, size_t N>
void ADS_set<Key,N>::reserve(size_type n) {
	if (table_size * max_lf >= n) return;
	size_type new_table_size {table_size};
	while (new_table_size * max_lf < n) ++(new_table_size *=2);
	rehash(new_table_size); 
}

template <typename Key, size_t N>
void ADS_set<Key,N>::rehash(size_type n) {
	size_type new_table_size {std::max(N,std::max(n,size_type(current_size/max_lf)))};

	Node **old_table {table};
	Node **new_table {new Node*[new_table_size]()}; 

	size_type old_table_size {table_size};
	table_size = new_table_size; 
	
 	for (size_type index {0}; index < old_table_size; index++) {
			Node *current_node {old_table[index]};
			
			while (current_node != nullptr){
	
				Node *new_node {new Node}; 
				new_node->key = current_node->key;

				size_type new_hash_index {h(new_node->key)};
				new_node->next = new_table[new_hash_index];
				new_table[new_hash_index] = new_node;

				current_node = current_node->next;

			}
			delete old_table[index];
		}
		delete[] old_table;
		table = new_table; 
}

template <typename Key, size_t N>
typename ADS_set<Key,N>::iterator ADS_set<Key,N>::find(const key_type &key) const {
  if (auto e {locate(key)}) return iterator{this, h(key),e};
  return end();
}

template <typename Key, size_t N> 
class ADS_set<Key,N>::Iterator {
	
	using Node = typename ADS_set<Key,N>::Node;
	
	const ADS_set *ads_set;
	size_type position;
	Node *ptr;

	void findNextNode() {
		for (; position < ads_set->table_size; ++position) {
			if(ads_set->table[position] != nullptr) {
				ptr = ads_set->table[position];
				return;
			}
		}	
	}


public:
	using value_type = Key;
	using difference_type = std::ptrdiff_t;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::forward_iterator_tag;

	explicit Iterator() {}
	explicit Iterator(const ADS_set *ads_set, size_type position, Node* ptr = nullptr) : ads_set{ads_set}, position{position}, ptr{ptr}  { 
		if(position == 0 && ptr == nullptr) {  findNextNode(); }
		if(position == ads_set->table_size) {  ptr = ads_set->table[position]; }
	}
	reference operator*() const { return ptr->key; } 
	pointer operator->() const { return &ptr->key; } 

	Iterator &operator++() {
		if(ptr != nullptr) {
			ptr = ptr->next;
			if(ptr == nullptr) { 
				position++;
				findNextNode(); 
			} 
		}
		return *this;
	} 

	Iterator operator++(int) {auto old {*this}; ++*this; return old;}
	friend bool operator==(const Iterator &lhs, const Iterator &rhs) { 
		return (lhs.ptr == rhs.ptr); 
	}
	friend bool operator!=(const Iterator &lhs, const Iterator &rhs) { return !(lhs == rhs); }
};

template <typename Key, size_t N>
void swap(ADS_set<Key,N> &lhs, ADS_set<Key,N> &rhs) { lhs.swap(rhs); }


#endif // ADS_SET_H
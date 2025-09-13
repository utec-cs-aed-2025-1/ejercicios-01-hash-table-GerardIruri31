#include <vector>

using namespace std;

const int maxColision = 3;
const float maxFillFactor = 0.8;

template<typename TK, typename TV>
struct ChainHashNode {
	TK key;
	TV value;
	ChainHashNode* next;
	ChainHashNode(const TK& k, const TV& v, ChainHashNode* n=nullptr)
		: key(k), value(v), next(n) {}
};

template<typename TK, typename TV>
class ChainHashListIterator {
private:
	ChainHashNode<TK, TV>* current;
public:
	using Node = ChainHashNode<TK, TV>;

	ChainHashListIterator(Node* ptr=nullptr) : current(ptr) {}

	Node& operator*() const { return *current; }
	Node* operator->() const { return current; }

	ChainHashListIterator& operator++() {
		if (current) current = current->next;
		return *this;
	}

	bool operator==(const ChainHashListIterator& other) const {
		return current == other.current;
	}
	bool operator!=(const ChainHashListIterator& other) const {
		return !(*this == other);
	}
};

template<typename TK, typename TV>
class ChainHash
{
private:    
    typedef ChainHashNode<TK, TV> Node;
    typedef ChainHashListIterator<TK, TV> Iterator;

	Node** array;  // array de punteros a Node
    int nsize; // total de elementos <key:value> insertados
	int capacity; // tamanio del array
	int *bucket_sizes; // guarda la cantidad de elementos en cada bucket
	int usedBuckets; // cantidad de buckets ocupados (con al menos un elemento)

public:
    ChainHash(int initialCapacity = 10){
		this->capacity = initialCapacity; 
		this->array = new Node*[capacity]();  
		this->bucket_sizes = new int[capacity]();
		this->nsize = 0;
		this->usedBuckets = 0;
	}

	TV get(TK key){
		size_t hashcode = getHashCode(key);
		size_t index = hashcode % capacity;
		
		Node* current = this->array[index];
		while(current != nullptr){
			if(current->key == key) return current->value;
			current = current->next;
		}
		throw std::out_of_range("Key no encontrado");
	}
	
	int size(){ return this->nsize; }	

	int bucket_count(){ return this->capacity; }

	int bucket_size(int index) { 
		if(index < 0 || index >= this->capacity) throw std::out_of_range("Indice de bucket invalido");
		return this->bucket_sizes[index]; 
	}	
	
	// TODO: implementar los siguientes métodos
	void set(TK key, TV value){
    	size_t index = getHashCode(key) % capacity;
    	Node* head = array[index];
    	for(Node* cur = head; cur != nullptr; cur = cur->next){
    		if(cur->key == key){
    			cur->value = value;
    			return;
    		}
    	}
    	Node* newNode = new Node(key, value, head);
    	array[index] = newNode;
    	nsize++;
    	if(bucket_sizes[index] == 0) usedBuckets++;
    	bucket_sizes[index]++;
    	if(bucket_sizes[index] > maxColision || fillFactor() > maxFillFactor){
    		rehashing();
    	}
    }

	bool remove(TK key){
		size_t index = getHashCode(key) % capacity;
		Node* cur = array[index];
		Node* prev = nullptr;
		while(cur){
			if(cur->key == key){
				if(prev) prev->next = cur->next;
				else array[index] = cur->next;
				delete cur;
				nsize--;
				bucket_sizes[index]--;
				if(bucket_sizes[index] == 0) usedBuckets--;
				return true;
			}
			prev = cur;
			cur = cur->next;
		}
		return false;
	}

	bool contains(TK key){
		size_t index = getHashCode(key) % capacity;
		for(Node* cur = array[index]; cur; cur = cur->next){
			if(cur->key == key) return true;
		}
		return false;
	}
	Iterator begin(int index){
		if(index < 0 || index >= capacity) throw std::out_of_range("Indice de bucket invalido");
		return Iterator(array[index]);
	}
	Iterator end(int index){
		(void)index;
		return Iterator(nullptr);
	}

private:
	double fillFactor(){
		return (double)this->usedBuckets / (double)this->capacity;
	}	

	size_t getHashCode(TK key){
		std::hash<TK> ptr_hash;
		return ptr_hash(key);
	}

	//TODO: implementar rehashing
	void rehashing(){
		int oldCapacity = capacity;
		capacity = capacity * 2 + 1;
		Node** oldArray = array;
		int* oldBucketSizes = bucket_sizes;

		array = new Node*[capacity]();
		bucket_sizes = new int[capacity]();
		usedBuckets = 0;
		int oldSize = nsize;
		nsize = 0;

		for(int i=0;i<oldCapacity;i++){
			Node* cur = oldArray[i];
			while(cur){
				Node* next = cur->next;
				size_t idx = getHashCode(cur->key) % capacity;
				cur->next = array[idx];
				array[idx] = cur;
				if(bucket_sizes[idx] == 0) usedBuckets++;
				bucket_sizes[idx]++;
				nsize++;
				cur = next;
			}
		}
		delete[] oldArray;
		delete[] oldBucketSizes;
		(void)oldSize;
	}

public:
	// TODO: implementar destructor
	~ChainHash() {
		if(!array) {
			return;
		}
		for(int i=0;i<capacity;i++){
			Node* cur = array[i];
			while(cur){
				Node* nxt = cur->next;
				delete cur;
				cur = nxt;
			}
		}
		delete[] array;
		delete[] bucket_sizes;
		array = nullptr;
		bucket_sizes = nullptr;
		capacity = 0;
		nsize = 0;
		usedBuckets = 0;
	}
};

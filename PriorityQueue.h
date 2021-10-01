#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

class PriorityQueueNode
{
public:
	void* data;
	float priority = 0;
	PriorityQueueNode* next;
	PriorityQueueNode* previous;

	PriorityQueueNode() 
	{
		data = nullptr;
		priority = 0;
		next = nullptr;
		previous = nullptr;
	}

	PriorityQueueNode(void* data, float priority, PriorityQueueNode* next, PriorityQueueNode* prev)
	{
		this->data = data;
		this->priority = priority;
		this->next = next;
		this->previous = prev;
	}
};

class PriorityQueue
{
private:
	PriorityQueueNode* top;
	long _size = 0;

public:
	PriorityQueue() 
	{
		top = nullptr;
		_size = 0;
	}

	~PriorityQueue()
	{
		PriorityQueueNode* c = top; // Current node
		PriorityQueueNode* p = nullptr; // Previous node

		while (c != nullptr)
		{
			p = c;
			c = c->next;

			if (p != nullptr) 
			{
				delete p;
			}
		}
	}

	bool isEmpty()
	{
		return top == nullptr;
	}

	long size()
	{
		return this->_size;
	}

	void enqueue(void* data, float priority)
	{
		PriorityQueueNode* node = new PriorityQueueNode(data, priority, this->top, nullptr);

		if (this->top != nullptr)
		{
			this->top->previous = node;
		}
		
		this->top = node;
		this->_size++;
	}

	void remove(PriorityQueueNode* node)
	{
		// Remove n from list.
		if (node != nullptr)
		{
			if (node->previous != nullptr)
			{
				node->previous->next = node->next;
			}
			if (node->next != nullptr)
			{
				node->next->previous = node->previous;
			}

			node->previous = nullptr;
			node->next = nullptr;

			delete node;

			this->_size--;
		}
	}

	void* dequeue()
	{
		//NAIVE implementation
		PriorityQueueNode* c = this->top; // Current node
		PriorityQueueNode* p = this->top; // Previous node
		PriorityQueueNode* n = this->top; // Node with highest priority (shortest path)

		// Get n with the shortest distance (highest priority).
		// TODO: Worst case this is O(N) TODO: Optimise
		while (c != nullptr)
		{
			if (c->priority < p->priority)
			{
				n = c;
			}

			p = c;
			c = c->next;
		}

		// Remove n from list.
		if (n != nullptr)
		{
			if (n->previous != nullptr)
			{
				n->previous->next = n->next;
			}
			if (n->next != nullptr)
			{
				n->next->previous = n->previous;
			}

			n->previous = nullptr;
			n->next = nullptr;

			this->_size--;

			return n->data;
		}

		this->_size = 0;

		return nullptr;
	}
};

#endif
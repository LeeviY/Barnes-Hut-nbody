#pragma once


class LinkedList{
    template <typename T>
    struct Node
    {
        T data;
        Node* next;
    };

}
/*struct linkedList
{
	float data_;
	linkedList* next;
    void add(float data);
};

linkedList* first;

void linkedList::add(float data)
{
	linkedList* new_data = new linkedList{data, nullptr};
    if (first == nullptr)
    {
        first = new_data;
    }
    else
    {
        new_data->next = first;
        first = new_data;
    }
}

void remove*/
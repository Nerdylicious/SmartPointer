//A SmartPointer implementation. A SmartPointer is a data type, implemented
//with templates, that simulates a pointer while also providing automatic
//garbage collection. It automatically counts the number of references to a
//SmartPointer<T *> object and frees the object of type T when the reference
//count hits zero.
//A SmartPointer can point to an object of any type by using generics/templates
//A SmartPointer provides safety via automatic memory management
//Avoids issues like dangling pointers, memory leaks and allocation failures
//SmartPointer must maintain a single reference count for all references
//to a given object
#include <iostream>
#include <limits>
using namespace std;

template <class T>
class SmartPointer{
    T *obj;
    //need a reference count variable that is incremented when we add a new
    //reference to the object and decremented when we remove a reference
    //we use a pointer to the actual reference count because the reference
    //count needs to be kept between multiple SmartPointers (that point
    //to the same object)
    unsigned *ref_count;
    void remove();
    void decrement_ref_count();
    void increment_ref_count();
    public:
        SmartPointer();
        SmartPointer(T *, unsigned *);
        SmartPointer(SmartPointer<T> &);
        ~SmartPointer();
        SmartPointer<T> &operator=(SmartPointer<T> &);
        unsigned *get_ref_count_ptr();
        T **get_pointer_address();
        T *get_pointer();
        void set_pointer(T *);
        T get_value();
        void set_value(T);
};

template <class T>
SmartPointer<T>::SmartPointer(){
    this->obj = NULL;
    this->ref_count = NULL;
}

template <class T>
SmartPointer<T>::SmartPointer(T *obj, unsigned *ref_count){
    this->obj = obj;
    this->ref_count = ref_count;
    if(this->obj != NULL){
        increment_ref_count();
    }
}

//this constructor creates a new SmartPointer that points to an existing object
template <class T>
SmartPointer<T>::SmartPointer(SmartPointer<T> &sptr){
    this->obj = sptr.get_pointer();
    this->ref_count = sptr.get_ref_count_ptr();
    if(this->obj != NULL){
        increment_ref_count();
    }
}

template <class T>
SmartPointer<T>::~SmartPointer(){
    //We are destroying a reference to the object. Decrement ref_count. If
    //ref_count is 0, then free the memory created by the integer and and
    //destroy the object
    cout << "Call destructor" << endl;
    if(this->ref_count != NULL){
        decrement_ref_count();
        remove();
    }
}

template <class T>
void SmartPointer<T>::remove(){
    if(*(this->ref_count) == 0){
        cout << "Deallocate memory" << endl;
        //only call delete this->obj if this->obj points to an object that
        //has been dynamically allocated
        //delete this->obj;
        delete this->ref_count;
    }
    this->obj = NULL;
    this->ref_count = NULL;
}

//Override the equal operator, so that when you set a SmartPointer equal to
//another SmartPointer, the previous SmartPointer's ref_count is decremented
//and the new SmartPointer's ref_count is incremented
template <class T>
SmartPointer<T> &SmartPointer<T>::operator=(SmartPointer<T> &sptr){
    if(this == &sptr){
        return *this;
    }
    if(this->ref_count != NULL){
        decrement_ref_count();
        remove();
    }
    this->obj = sptr.get_pointer();
    this->ref_count = sptr.get_ref_count_ptr();
    increment_ref_count();
    return *this;
}

template <class T>
unsigned *SmartPointer<T>::get_ref_count_ptr(){
    return this->ref_count;
}

template <class T>
void SmartPointer<T>::decrement_ref_count(){
    *(this->ref_count) = *(this->ref_count) - 1;
}

template <class T>
void SmartPointer<T>::increment_ref_count(){
    *(this->ref_count) = *(this->ref_count) + 1;
}

template <class T>
T **SmartPointer<T>::get_pointer_address(){
    return &(this->obj);
}

template <class T>
T *SmartPointer<T>::get_pointer(){
    return this->obj;
}

template <class T>
void SmartPointer<T>::set_pointer(T *obj){
    this->obj = obj;
    if(this->obj != NULL){
        increment_ref_count();
    }
}

template <class T>
T SmartPointer<T>::get_value(){
    if(this->obj != NULL){
        return *(this->obj);
    }
    else{
        //change this
        return std::numeric_limits<int>::min();
    }
}

template <class T>
void SmartPointer<T>::set_value(T val){
    if(this->obj != NULL){
        *(this->obj) = val;
    }
}

int main(){
    unsigned *ref_count = new unsigned;
    *ref_count = 0;
    int a = 5;
    SmartPointer<int> *sptr1 = new SmartPointer<int>(&a, ref_count);
    cout << "ref_count: " << *(sptr1->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptr1->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptr1->get_value() << endl;
    SmartPointer<int> *sptr2 = new SmartPointer<int>(&a, ref_count);
    cout << "ref_count: " << *(sptr1->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptr2->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptr2->get_value() << endl;
    cout << "---------------------------------------" << endl;
    a = 8;
    SmartPointer<int> *sptr3 = new SmartPointer<int>(*sptr1);
    cout << "ref_count: " << *(sptr1->get_ref_count_ptr()) << endl;
    cout << "ref_count: " << *(sptr2->get_ref_count_ptr()) << endl;
    cout << "ref_count: " << *(sptr3->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptr1->get_pointer() << endl;
    cout << "Value of ptr (address it is storing): " << sptr2->get_pointer() << endl;
    cout << "Value of ptr (address it is storing): " << sptr3->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptr1->get_value() << endl;
    cout << "ptr dereferenced: " << sptr2->get_value() << endl;
    cout << "ptr dereferenced: " << sptr3->get_value() << endl;
    cout << "---------------------------------------" << endl;
    delete sptr1;
    cout << "ref_count: " << *(sptr2->get_ref_count_ptr()) << endl;
    cout << "ref_count: " << *(sptr3->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptr2->get_pointer() << endl;
    cout << "Value of ptr (address it is storing): " << sptr3->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptr2->get_value() << endl;
    cout << "ptr dereferenced: " << sptr3->get_value() << endl;
    cout << "---------------------------------------" << endl;
    delete sptr3;
    cout << "ref_count: " << *(sptr2->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptr2->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptr2->get_value() << endl;
    cout << "---------------------------------------" << endl;
    delete sptr2;
    cout << "---------------------------------------" << endl;
    unsigned *ref_count_v1 = new unsigned;
    *ref_count_v1 = 0;
    int v1 = 5;
    SmartPointer<int> *sptra = new SmartPointer<int>(&v1, ref_count_v1);
    SmartPointer<int> *sptrb = new SmartPointer<int>(&v1, ref_count_v1);
    cout << "ref_count: " << *(sptra->get_ref_count_ptr()) << endl;
    cout << "ref_count: " << *(sptrb->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptra->get_pointer() << endl;
    cout << "Value of ptr (address it is storing): " << sptrb->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptra->get_value() << endl;
    cout << "ptr dereferenced: " << sptrb->get_value() << endl;
    cout << "---------------------------------------" << endl;
    unsigned *ref_count_v2 = new unsigned;
    *ref_count_v2 = 0;
    int v2 = 10;
    SmartPointer<int> *sptrc = new SmartPointer<int>(&v2, ref_count_v2);
    cout << "ref_count: " << *(sptrc->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptrc->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptrc->get_value() << endl;
    cout << "---------------------------------------" << endl;
    //overridden equals will not be called if you do sptra = sptrc because sptra and
    //sptrc are pointers to SmartPointer objects. You need to use = on SmartPointer
    //objects and not pointers to it
    //sptra = sptrc;
    *sptra = *sptrc;
    cout << "ref_count: " << *(sptra->get_ref_count_ptr()) << endl;
    cout << "ref_count: " << *(sptrb->get_ref_count_ptr()) << endl;
    cout << "ref_count: " << *(sptrc->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptra->get_pointer() << endl;
    cout << "Value of ptr (address it is storing): " << sptrb->get_pointer() << endl;
    cout << "Value of ptr (address it is storing): " << sptrc->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptra->get_value() << endl;
    cout << "ptr dereferenced: " << sptrb->get_value() << endl;
    cout << "ptr dereferenced: " << sptrc->get_value() << endl;
    cout << "---------------------------------------" << endl;
    delete sptrb;
    cout << "ref_count: " << *(sptra->get_ref_count_ptr()) << endl;
    cout << "ref_count: " << *(sptrc->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptra->get_pointer() << endl;
    cout << "Value of ptr (address it is storing): " << sptrc->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptra->get_value() << endl;
    cout << "ptr dereferenced: " << sptrc->get_value() << endl;
    cout << "---------------------------------------" << endl;
    delete sptra;
    cout << "ref_count: " << *(sptrc->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptrc->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptrc->get_value() << endl;
    cout << "---------------------------------------" << endl;
    SmartPointer<int> *sptrd = new SmartPointer<int>();
    *sptrd = *sptrc;
    cout << "ref_count: " << *(sptrc->get_ref_count_ptr()) << endl;
    cout << "ref_count: " << *(sptrd->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptrc->get_pointer() << endl;
    cout << "Value of ptr (address it is storing): " << sptrd->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptrc->get_value() << endl;
    cout << "ptr dereferenced: " << sptrd->get_value() << endl;
    cout << "---------------------------------------" << endl;
    *sptrc = *sptrc;
    cout << "ref_count: " << *(sptrc->get_ref_count_ptr()) << endl;
    cout << "ref_count: " << *(sptrd->get_ref_count_ptr()) << endl;
    cout << "Value of ptr (address it is storing): " << sptrc->get_pointer() << endl;
    cout << "Value of ptr (address it is storing): " << sptrd->get_pointer() << endl;
    cout << "ptr dereferenced: " << sptrc->get_value() << endl;
    cout << "ptr dereferenced: " << sptrd->get_value() << endl;
    return 0;
}

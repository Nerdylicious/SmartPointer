# Smart Pointer

A SmartPointer implementation in C++. A SmartPointer is a data type, implemented with templates, that simulates a pointer while also providing automatic garbage collection. It automatically counts the number of references to a `SmartPointer<T *>` object and frees the object of type T when the reference count hits zero.

* A SmartPointer can point to an object of any type by using generics/templates
* A SmartPointer provides safety via automatic memory management
* Avoids issues like dangling pointers, memory leaks and allocation failures
* SmartPointer must maintain a single reference count for all references to a given object

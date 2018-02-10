#ifndef TWIST_REFERENCE_H_
#define TWIST_REFERENCE_H_

template<class T>
class Reference {
	T* ref;
public:
	Reference(T& ref_) : ref(&ref_) { }
	Reference(T* ref_) : ref(ref_) { }

	T* operator->() {
		return ref;
	}

	operator bool() {
		return ref;
	}
};

#endif
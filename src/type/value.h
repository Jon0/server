#pragma once

namespace type {


/**
 * A typed value holder
 */
class value_base {
public:
	virtual ~value_base() {}

	/**
	 * sets the type and value
	 */
    template<class T, class V> void set(const V &value);

	/**
	 * returns the stored value
	 * throws an exception if the template
	 * does not match the set type
	 */
	template<class T> const T &get() const;

};


template<class T>
class value : public value_base {
public:
	value(const T &initial_value)
		:
		stored_value(initial_value) {}


    const T &get() const {
    	return stored_value;
    }

    void set(const T &v) {
    	stored_value = v;
    }

    T &access() {
    	return stored_value;
    }

private:
	T stored_value;

};


template<class T>
const T &value_base::get() const {
	return dynamic_cast<const value<T> &>(*this).get();
}

template<class T, class V>
void value_base::set(const V& new_value) {
	return dynamic_cast<value<T> &>(*this).set(new_value);
}


}	// namespace type

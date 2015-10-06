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


/**
 * base templated type
 */
template<class T>
class value : public value_base {
public:
	virtual ~value() {}

	virtual const T &get() const = 0;

	virtual T &access() = 0;

	void set(const T &v) {
		access() = v;
	}
};


template<class T>
class value_owned : public value<T> {
public:
	value_owned(const T &initial_value)
		:
		stored_value(initial_value) {}

	const T &get() const override {
		return stored_value;
	}

	T &access() override {
		return stored_value;
	}

private:
	T stored_value;

};


template<class T>
class value_shared : public value<T> {
public:
	value_shared(const T &initial_value)
		:
		value<T>(initial_value) {}

	const T &get() const override {
		return *stored_ptr;
	}

	T &access() override {
		return *stored_ptr;
	}

private:
	std::shared_ptr<T> stored_ptr;

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

#include <iostream>
#include <vector>

#include <io/template.h>
#include <type/atomic.h>
#include <type/var.h>


void test_list() {
	type::atomic<std::vector<int>> intlist;
	type::value<std::vector<int>> ints({1, 2, 3});

	std::cout << intlist.name << "\n";

	type::typed_value tv = type::typed_value::create(ints);


	type::var<std::vector<int>> thing("thing", {1, 2, 3});
	std::cout << thing.status() << "\n";


	type::value<int> number(234);
	type::typed_value val = type::typed_value::create(number);

	type::var<type::object> thinga("thing0", type::object{{"wut", val}});
	type::var<type::object> things("thing1", type::object{{"list", type::typed_value::create(ints)}});
	things.access().emplace(std::make_pair("inner_object", type::typed_value::create(thinga)));
	std::cout << things.status() << "\n";
}


void test_type() {
	// test types
	type::atomic<int> int_type;
	std::cout << "type = " << int_type.name << "\n";


	type::value<int> int_value = int_type.create(50);
	std::cout << "value = " << int_value.get() << "\n";

	int_type.execute("add", int_value, {&int_value});

	type::typed_value tv = type::typed_value::create(int_value);


	//type::typed_value tv(int_type, *int_value);
	std::cout << "type = " << tv.type.name << "\n";
	std::cout << "value = " << tv.str_value() << "\n";


	int_value.set(34);

	std::cout << "type = " << tv.type.name << "\n";
	std::cout << "value = " << tv.str_value() << "\n";

	// hmm
	type::value<int> something(12);
	type::typed_value st = type::typed_value::create(something);


	type::typed_value st2 = st;
	std::cout << "type = " << st2.type.name << "\n";
	std::cout << "value = " << st2.str_value() << "\n";

	st2 = tv;
	std::cout << "type = " << st2.type.name << "\n";
	std::cout << "value = " << st2.str_value() << "\n";


	// variables
	type::var<int> thing("thing", 7);
	std::cout << thing.status() << "\n";


	thing.set(45);
	std::cout << thing.status() << "\n";
}

void test_tmpl() {

	type::value<int> number(64);
	io::tmpl t("abc{{var}}ghi");


	type::node n("test");
	n.add("var", type::typed_value::create(number));


	std::cout << t.render(n) << "\n";

	number.set(1234);
	std::cout << t.render(n) << "\n";
}

int main() {
	std::cout << "running tests\n";

	test_list();
	//test_type();
	//test_tmpl();

	return 0;
}
#include <iostream>
#include <vector>

#include <io/template.h>
#include <linux/status.h>
#include <type/atomic.h>
#include <type/var.h>


void status_test() {

}


void function_test() {

	//type::atomic<int> inttype;
	//std::cout << inttype.name << "\n";

	//type::value<int> number = inttype.create(3);
	//number.set(4);
	//type::typed_value tv = type::typed_value::create(status_test);
}

void test_list() {
	//type::atomic<std::vector<int>> intlist;
	//type::value_owned<std::vector<int>> ints({1, 2, 3});
	//std::cout << intlist.name << "\n";

	//type::typed_value tv = type::typed_value::create(ints.get());
	//std::cout << tv.str_value() << "\n";

	//type::var<std::vector<int>> thing("thing", {1, 2, 3});
	//std::cout << thing.status() << "\n";


	//type::value_owned<int> number(234);
	//type::typed_value val = type::typed_value::create(number.get());

	//type::var<type::object> thinga("thing0", type::object{{"wut", val}});
	//type::var<type::object> things("thing1", type::object{{"list", type::typed_value::create(ints.get())}});
	//things.access().emplace(std::make_pair("inner_object", type::typed_value::create(thinga.get())));
	//std::cout << things.status() << "\n";
}


void test_type() {
	// test types
	type::atomic<int> int_type;
	std::cout << "type = " << int_type.name << "\n";


	type::value_owned<int> int_value = int_type.create(50);
	std::cout << "value = " << int_value.get() << "\n";

	//int_type.execute("add", int_value, {&int_value.get()});

	type::typed_value tv = type::typed_value::create(int_value.get());


	//type::typed_value tv(int_type, *int_value);
	std::cout << "type = " << tv.type.name << "\n";
	std::cout << "value = " << tv.str_value() << "\n";


	int_value.set(34);

	std::cout << "type = " << tv.type.name << "\n";
	std::cout << "value = " << tv.str_value() << "\n";
}

void test_tmpl() {

	type::value_owned<int> number(64);
	io::tmpl t("abc{{var}}ghi");


	type::node n("test");
	n.add("var", type::typed_value::create(number.get()));


	std::cout << t.render(n) << "\n";

	number.set(1234);
	std::cout << t.render(n) << "\n";
}

int main() {
	std::cout << "running tests\n";

	status_test();
	function_test();
	test_list();
	test_type();
	test_tmpl();

	return 0;
}

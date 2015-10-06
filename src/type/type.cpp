#include "type.h"

namespace type {

func::func(const std::string &name, const type_func f)
	:
	name{name},
	to_exec{f} {
}


bool func::execute(value_base &modify, const val_list &args) {
	return to_exec(modify, args);
}


type_base::type_base(const std::string &name)
	:
	name(name) {}


bool type_base::operator ==(const type_base &other) const {
	return name == other.name;
}

bool type_base::operator !=(const type_base &other) const {
	return name != other.name;
}

void type_base::func(const std::string &name, const type_func &f) {
	functions.emplace(std::make_pair(name, f));
}

bool type_base::execute(const std::string &name, value_base &modify, const val_list &args) {
	auto f = functions.find(name);
	if (f != functions.end()) {
		return f->second(modify, args);
	}
	return false;
}


typed_value::typed_value(const typed_value &tv)
	:
	type(tv.type),
	var(tv.var) {}


typed_value::typed_value(const type_base &ty, std::shared_ptr<value_base> val)
	:
	type(ty),
	var(val) {}


bool typed_value::operator ==(const typed_value &other) const {
	if (type != other.type) {
		return false;
	}
	return false;
}


const typed_value &typed_value::operator =(const typed_value &other) {
	if (type == other.type) {
		type.set(*var, *other.var);
	}
	return *this;
}

void typed_value::set(const typed_value &other) {

}


std::string typed_value::str_value() const {
	return type.str_value(*var);
}


node::node(const std::string &name)
	:
	name(name),
	parent(nullptr) {
}


node::~node() {
	if (parent) {
		parent->remove_node(this);
	}
}


std::vector<std::string> node::list_options(bool recurse, std::string indent) {
	std::vector<std::string> result;
	result.push_back(indent + "node " + this->name + " {");
	std::string inner_indent = indent + "\t";
	for (auto &v : varmap) {
		result.push_back(inner_indent + "var " + v.first + " = " + v.second.str_value());
	}
	for (auto &p : nodes) {
		for (auto &line : p.second->list_options(recurse, inner_indent)) {
			result.push_back(line);
		}
	}
	result.push_back(indent + "}");
	return result;
}

std::vector<std::string> node::list_variables() const {
	std::vector<std::string> result;
	for (auto &v : this->varmap) {
		result.push_back(v.first);
	}
	return result;
}

typed_value *node::get_variable_rw(const std::string &name) {
	auto tv = varmap.find(name);
	if (tv == varmap.end()) {
		return nullptr;
	}
	return &tv->second;
}

const typed_value *node::get_variable(const std::string &name) const {
	auto tv = varmap.find(name);
	if (tv == varmap.end()) {
		return nullptr;
	}
	return &tv->second;
}

node *node::get_child(const std::string &name) const {
	auto child = nodes.find(name);
	if (child == nodes.end()) {
		return nullptr;
	}
	return child->second;
}


void node::set_parent(node *new_parent) {
	if (parent) {
		parent->remove_node(this);
	}
	parent = new_parent;
	new_parent->add_node(this);
}

void node::add(const std::string &vname, const typed_value &value) {
	varmap.emplace(std::make_pair(vname, value));
}


void node::add_node(node *child) {
	nodes.emplace(std::make_pair(child->name, child));
}


void node::remove_node(node *child) {
	nodes.erase(child->name);
}


} // namespace type

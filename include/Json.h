#ifndef UNTITLED_JSON_H
#define UNTITLED_JSON_H

#include <any>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>
#include <fstream>


using std::cout;
using std::cin;
using std::string;
using std::any;
using std::vector;
using std::map;
using std::any_cast;

class Json {
private:

    map <string, any> _map;
    vector <any> _arr;

    bool _is_array = false;
    bool _is_object = false;

    void create_vector(const string& s);
    void create_map(const string& s);

public:

    explicit Json(const string& s);

    [[nodiscard]] bool is_array() const;

    [[nodiscard]] bool is_object() const;

    std::any& operator[](const string& key);

    std::any& operator[](int index);

    static Json parse(const string& s);

    static Json parseFile(const string& path_to_file);

    static unsigned int miss_spaces(unsigned int i, const string& s);

    static string read_key(unsigned int &i, const string& s);

    static unsigned int find_end(unsigned int i, const string& s);

    static string cut_num(unsigned int i, const string& s);

    void print_map();

    void print_vector();

    static void print(any _data);

};

#endif //UNTITLED_JSON_H

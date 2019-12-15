
#include "Json.h"


Json::Json(const string& s) {
    unsigned int i = 0;
    i = miss_spaces(i, s);
    if (s[i] == '{') {
        _is_object = true;
        create_map(s);
    }
    else if (s[i] == '[') {
        _is_array = true;
        create_vector(s);
    } else {
        throw std::logic_error("invalid data type");
    }

}

bool Json::is_array() const{
    return _is_array;
}

bool Json::is_object() const{
    return _is_object;
}

std::any& Json::operator[](const string& key) {
    if (Json::is_object()) {
        return this->_map[key];
    } else {
        throw std::logic_error("is not an object");
    }
}

std::any& Json::operator[](int index) {
    if (is_array()) {
        return this->_arr[index];
    } else {
        throw std::logic_error("is not an array");
    }
}
Json Json::parse(const std::string& s) {
    Json obj(s);
    return obj;
}

Json Json::parseFile(const std::string& path_to_file){
    std::ifstream json;
    json.open(path_to_file);
    string s = "";
    string line;
    while(!json.eof()) {
        std::getline(json, line);
        s += line;
    }
    Json obj(s);
    return obj;
}

void Json::create_vector(const string& s) {

    unsigned int i = 1;
    while (i < s.find_last_of("]")) {
        miss_spaces(i, s);
        if (s[i] == '{') {
            string s1;
            unsigned int n = find_end(i, s);
            if(n == s.length())
                throw std::logic_error("string is not valid");
            s1 = s.substr(i, n - i + 1);
            Json obj(s1);
            this->_arr.emplace_back(obj._map);
            i += s1.length();
        }
        else if (s[i] == '\"') {
            string word;
            word = read_key(i, s);
            i = miss_spaces(i, s);
            this->_arr.emplace_back(word);
        }
        else if ((s[i] == 't' && s[i + 1] == 'r' && s[i + 2] == 'u' && s[i + 3] == 'e') || (s[i] == 'f' && s[i + 1] == 'a' &&s[i + 2] == 'l' && s[i + 3] == 's' && s[i + 4] == 'e')) {
            bool x;
            if(s[i] == 't') {
                i += 4;
                x = true;
            }
            else {
                i += 5;
                x = false;
            }
            this->_arr.emplace_back(x);
        }
        else if(s[i] == '[') {
            string s1;
            unsigned int n = find_end(i, s);
            if(n == s.length())
                throw std::logic_error("string is not valid");
            s1 = s.substr(i, n - i + 1);
            Json obj(s1);
            this->_arr.emplace_back(obj._arr);
            i += s1.length();
        }
        else if(std::isdigit(static_cast<unsigned char>(s[i])) || (s[i] == '-'  && std::isdigit(static_cast<unsigned char>(s[i + 1])))) {
            // is alpha or is symbol from std namespace
            string num = cut_num(i, s);
            i += num.length();
            double d = stod(num);
            if (d - (int) d == 0) {
                int n = (int)d;
                this->_arr.emplace_back(n);
            } else {
                this->_arr.emplace_back(d);
            }
        }
        i = miss_spaces(i, s);
    }
}
unsigned int Json::miss_spaces(unsigned int i, const string& s) {
    while(s[i] == ' ' || s[i] == ',') {
        i++;
    }
    return i;
}

// string::find_end -> std::npos
unsigned int Json::find_end(unsigned int i, const string& s) {
    unsigned int cnt_open = 1 , cnt_close = 0;
    char key1, key2;
    key2 = '}';
    key1 = '{';
    if (s[i] == '[') {
        key1 = '[';
        key2 = ']';
    }

    while(cnt_close != cnt_open && i < s.length()) {
        i++;
        if (s[i] == key1) cnt_open++;
        else if (s[i] == key2) cnt_close++;
    }
    if(cnt_close != cnt_open) return s.length();
    return i;
}

void Json::create_map(const string& s) {
    unsigned int i = 1;
    while (i < s.find_last_of("}")) {
        string key;
        miss_spaces(i, s);
        key = read_key(i, s);
        i = miss_spaces(i, s);
        if (s[i] != ':')
            throw std::logic_error("String is not valid");
        i++;
        i = miss_spaces(i, s);
        if (s[i] == '{') {
            string s1;
            unsigned int n = find_end(i, s);
            if( n == s.length())
                throw std::logic_error("string is not valid");
            s1 = s.substr(i, n - i + 1);
            Json obj(s1);
            this->_map[key] = obj._map;
            i += s1.length();
        }
        else if (s[i] == '\"') {
            string word;
            word = read_key(i, s);
            i = miss_spaces(i, s);
            this->_map[key] = word;
        }
        else if ((s[i] == 't' && s[i + 1] == 'r' && s[i + 2] == 'u' && s[i + 3] == 'e') ||
                 (s[i] == 'f' && s[i + 1] == 'a' &&s[i + 2] == 'l' && s[i + 3] == 's' && s[i + 4] == 'e')) {
            bool x;
            if(s[i] == 't') {
                i += 4;
                x = true;
            }
            else {
                i += 5;
                x = false;
            }
            this->_map[key] = x;
        }
        else if(s[i] == '[') {
            string s1;
            unsigned int  n = find_end(i, s);
            if( n == s.length())
                throw std::logic_error("string is not valid");

            s1 = s.substr(i, n - i + 1);

            Json obj(s1);
            this->_map[key] = obj._arr;
            i += s1.length();
        }
        else if(std::isdigit(static_cast<unsigned char>(s[i])) || (s[i] == '-'  && std::isdigit(static_cast<unsigned char>(s[i + 1])))) {
            string num = cut_num(i, s);
            i += num.length();
            double d = stod(num);
            if (d - (int) d == 0) {
                int n = (int)d;
                this->_map[key] = n;
            } else
                this->_map[key] = d;
        }
        i = miss_spaces(i, s);
    }
}

string Json::cut_num(unsigned int i, const string& s) {
    unsigned int st = i;
    string num;
    while (isdigit(static_cast<unsigned char>(s[i]))) i++;
    num = s.substr(st, i - st);
    return num;
}
string Json::read_key(unsigned int &i, const string& s) {
    unsigned int st;
    string key;
    i = miss_spaces(i, s);
    if (s[i] == '\"') {
        i++;
        st = i;
    }
    else throw std::logic_error("string isn't valid!");
    while (s[i] != '"') i++;
    key = s.substr(st, i - st);
    i++;
    
    return key;
}
void Json::print_map() {
    cout << "{\n";
    int i = 0;
    for(const auto& p  : this->_map) {
        if (i != 0) cout << ",\n";
        cout << "    " << p.first << " : ";
        print(p.second);
        i++;
    }
    cout << "}\n";
}
void Json::print(any _data) {
    string type = _data.type().name();
   
    try {
        if (type == "i") {
            cout << any_cast<int> (_data);
        }
        else if (type == "d") {
            cout << any_cast<double> (_data);
        }
        else if (type == "b") {
            if (any_cast<bool> (_data)) std::cout << "true";
            else cout << "false";
        }
        else if (type == "Ss" ||type == "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE") {
            cout << any_cast<string> (_data);
        }
        else if (type.find("St6vector") < type.length()) {
            std::vector <std::any> vec;
            vec = any_cast<vector<any>>(_data);
            unsigned int count = 0;
            cout << "[ ";
            for(const auto& c: vec) {
                count++;
                if (count > 1) std::cout << " , ";
                print(c);
            }
            cout << " ]";
        }
        else if (type.find("St3map") < type.length()) {
            map <string, any> _map;
            _map = std::any_cast<map <string, any>>(_data);
            cout << "{\n" ;
            unsigned int count = 0;
            for(const auto& c: _map) {
                count++;
                if (count > 1) cout << " ,\n";
                cout << "\t" << c.first << " : ";
                print(c.second);
            }
            cout << "\n\t}";
        }
    }
    catch(const std::bad_any_cast& e) {
        cout << e.what() << '\n';
    }
}
void Json::print_vector() {
    unsigned int count = 0;
    cout << "[ ";
    for(const auto& c: this->_arr) {
        count++;
        if (count > 1) cout << ", ";
        print(c);
    }
    cout << " ]";
}

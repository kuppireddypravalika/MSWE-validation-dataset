#include <list>
#include <string>
#include <unordered_map>
#include <vector>

using LayerValue = std::vector<std::string>;
using LayerOptions = std::unordered_map<int, LayerValue>;

class Layer {
public:
    void Set(int option, const std::string &data) {
        option_map[option].push_back(data);
    }

    void EraseSet(int option, const std::string &data) {
        option_map[option].clear();
        option_map[option].push_back(data);
    }

    void Erase(int option) {
        option_map.erase(option);
    }

    const LayerOptions &Options() const { return option_map; }
private:
    LayerOptions option_map;
};

template <typename T>
class Value {
public:
    explicit Value(const T &value) : value_data(value) {}
    const T& Get() const { return value_data; }
private:
    T value_data;
};

unsigned long run(int repeat, int str_size) {
    Layer layer;
    unsigned long sum = 0;
    std::string s(str_size, 'x');
    for (int i = 0; i < repeat; ++i) {
        layer.Set(0, s);
        layer.EraseSet(0, s);
        Value<std::string> val(s);
        sum += val.Get().size();
    }
    if (layer.Options().count(0)) {
        sum += layer.Options().at(0).back().size();
    }
    return sum;
}

// Explicit template instantiation statements
template class Value<std::string>;
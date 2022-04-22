#ifndef JETCONTEXT_H
#define JETCONTEXT_H

#include <unordered_map>
#include <stdexcept>
#include <variant>

class JetContext {
    public:
        /**
         * @brief Set the Value object
         * 
         * @tparam T the template type parameter mus be supported and of same type as 
         * value, if it is float be sure to cast the value into a float. 
         */
        template <typename T>
        bool setValue(const std::string& name, const T value, bool allowOverwrite = false);

        template <typename T>
        bool getValue(const std::string& name, T& value) const;

        template <typename T>
        T getValue(const std::string& name) const;

        template <typename T>
        bool isAvailable(const std::string& name) const;

        static constexpr int ERRORVALUE {-999};

    private:
        std::unordered_map<std::string, std::variant<int, float>> dict_;
};

template <typename T> bool JetContext::getValue(const std::string& name, T& value) const {
    // to do : add error checking.
    value = std::get<T>(dict_.at(name));
    return true;
}

template <typename T> T JetContext::getValue(const std::string& name) const {
    return std::get<T>(dict_.at(name));
}

template <typename T> bool JetContext::setValue(const std::string& name, const T value, bool allowOverwrite) {
    std::pair<std::string, T> val(name, value);
    dict_.insert(val);
    return true;
}

template <typename T> bool JetContext::isAvailable(const std::string& name) const {
    return dict_.find(name) != dict_.end();    
}

#endif


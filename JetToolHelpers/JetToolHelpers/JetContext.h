#ifndef JETCONTEXT_H
#define JETCONTEXT_H

#include <unordered_map>
#include <stdexcept>
#include <variant>
#include <type_traits>

class JetContext {
    public:
        /**
         * @brief add the association (name, value) to the context, functions very much like
         * a simple dictionarry.  
         * @tparam T the template type parameter must be supported, 
         * if you're passing a floating point literal be sure to cast the value into a float.
         * @param allowOverwrite specifies wether the name value can be
         * overwritten if already specified. Default is false. 
         * @returns bool indicating wether value was successfully set or not.
         * @exception std::invalid_argument if type of value being inserted is
         * not supported. 
         */
        template <typename T> bool setValue(const std::string& name, const T value, bool allowOverwrite = false);
        
        /**
         * @brief get value from context with specified name in value. 
         * @tparam T the type of the value to be returned.
         * @param name key at which the dictionary is read. SEE EXCEPTIONS. 
         * @exception std::invalid_argument will be thrown if name is not a key in
         * JetContext.
         * @exception std::bad_variant_access will be thrown if name is a key in
         * JetContext but is of different type than T. 
         * @return T the value. 
         */
        template <typename T> void getValue(const std::string& name, T& value) const;
        template <typename T> T getValue(const std::string& name) const;

        /**
         * @brief Return true if dictionary has an entry with key name.
         * @param name the key of the dictionary. 
         * @return true/false denoting wether the key is there or not. 
         */
        bool isAvailable(const std::string& name) const {
            return dict_.find(name) != dict_.end();
        };

        static constexpr int ERRORVALUE {-999}; // set at compile time.
    private:
        // it would probably make more sense to replace all templates by std::variant<int, float>...
        std::unordered_map<std::string, std::variant<int, float>> dict_;
};

template <typename T> void JetContext::getValue(const std::string& name, T& value) const {
    if(isAvailable(name))
        value = std::get<T>(dict_.at(name));
    else
        throw std::invalid_argument(std::string("Key Error : ") + name + std::string(" not found in JetContext."));
}

template <typename T> T JetContext::getValue(const std::string& name) const {
    T value;
    getValue(name, value);
    return value;
}

template <typename T> bool JetContext::setValue(const std::string& name, const T value, bool allowOverwrite) {
    if constexpr (!std::is_same<T, int>::value && !std::is_same<T, float>::value) {
        throw std::invalid_argument("Unsupported type");
    } else {
        if(name == "" || ( !allowOverwrite && isAvailable(name))) 
            return false;
        
        dict_.insert_or_assign(name, value);         // insert returns pair with iterator and return code
        return true;                                 // if true => insertion, if false => assignement.
    }
}

#endif

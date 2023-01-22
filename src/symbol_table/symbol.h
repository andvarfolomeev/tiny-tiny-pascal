#ifndef SYMBOL_H
#define SYMBOL_H

#include <map>
#include <memory>
#include <string>
#include <vector>

class Symbol {
   public:
    Symbol(std::string name) : name(std::move(name)) {}
    virtual std::string get_type_of_object_str();
    virtual std::string get_ret_type_str();
    virtual void draw_additional(std::ostream &os, int depth);
    std::string get_name() const;
    virtual ~Symbol() {}
    virtual bool is_type() { return false; }

    int offset = 4;

   protected:
    std::string name;
};

#endif  // SYMBOL_H

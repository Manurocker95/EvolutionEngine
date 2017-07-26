
// This is free code released into the public domain.
// Drafted by Ángel on April 2017.
// Use it at your own risk. It might have bugs.
// angel.rodriguez@esne.edu

#pragma once

#include <string>

using std::string;
using std::to_string;

namespace EvolutionEngine
{

    class Variant
    {
        enum Type
        {
            BOOL, INT, FLOAT, STRING
        }
        type;

        union
        {
            bool       bool_value;
            int         int_value;
            float     float_value;
            string * string_value;
        }
        value;

    public:

        Variant(bool initializer)
        {
            type = BOOL;
            value.bool_value = initializer;
        }

        Variant(int initializer)
        {
            type = INT;
            value.int_value = initializer;
        }

        Variant(float initializer)
        {
            type = FLOAT;
            value.float_value = initializer;
        }

        Variant(const string & initializer)
        {
            type = STRING;
            value.string_value = new string(initializer);
        }

       ~Variant()
        {
            release ();
        }

    private:

        void release ()
        {
            if (type == STRING)
            {
                delete value.string_value;
            }
        }

    public:

        Type get_type () const
        {
            return type;
        }

        bool is_bool () const
        {
            return type == BOOL;
        }

        bool is_int () const
        {
            return type == INT;
        }

        bool is_float () const
        {
            return type == FLOAT;
        }

        bool is_string () const
        {
            return type == STRING;
        }

        // ...

    public:

        // En algunos casos el compilador podrá determinar el valor de type en tiempo de compilación
        // y evitará la sentencia switch:

        bool as_bool () const
        {
            switch (type)
            {
                case BOOL:    return  value.  bool_value;
                case INT:     return  value.   int_value != 0;
                case FLOAT:   return  value. float_value != 0.f;
                case STRING:  return *value.string_value == "true" ? true : false;
            }

            return false;
        }

        int as_int () const
        {
            switch (type)
            {
                case BOOL:    return value.bool_value ? 1 : 0;
                case INT:     return value.int_value;
                case FLOAT:   return int(value.float_value);
                case STRING:  return strtol (value.string_value->c_str (), nullptr, 10);
            }

            return 0;
        }

        string as_string () const
        {
            switch (type)
            {
                case BOOL:    return  value.bool_value == true ? "true" : "false";
                case INT:     return  to_string (value.  int_value);
                case FLOAT:   return  to_string (value.float_value);
                case STRING:  return *value.string_value;
            }

            return string();
        }

    public:

        // ...

        Variant & operator = (const Variant & other)
        {
            if (this != &other)
            {
                release ();

                this->type  = other.type;
                this->value = other.value;
            }

            return *this;
        }

        Variant & operator = (int new_value)
        {
            release ();

            type = INT;
            value.int_value = new_value;

            return *this;
        }

        // ...

        operator int () const
        {
            return this->as_int ();
        }

        operator string () const
        {
            return this->as_string ();
        }

        // ...

    };

}

#ifndef TYPE_ERASED_H
#define TYPE_ERASED_H

// https://cppcon.digital-medium.co.uk/wp-content/uploads/2021/09/Type-Erasure-A-Design-Analysis.pdf


#include "jsoncpp/json/json.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <type_traits>

#include <stdexcept>
#include <type_traits>


// Serialization functions
// These are called by Model<T>::serialize()
// If other types T are to be type erased, then implementations of the serialize
// function must be provided for T

/*template<typename T>
std::string serialize(const T value)
{
    return std::to_string(value);
}*/



enum class GUIObjectType
{
    BOX,
    LABEL,
    TEXT_AREA,
    TEXT_ENTRY,
    TEXT_BOX
};



// TODO: implement the rule of 5 using swap

class AnyGUIObject
{

    friend std::ostream& operator<<(std::ostream& os, const AnyGUIObject& typeErased);

    friend void swap(AnyGUIObject& l, AnyGUIObject& r) noexcept;

public:

    class TypeErasedTypeCastException : public std::runtime_error
    {
    public:
        TypeErasedTypeCastException()
            : std::runtime_error("")
        {
        }

        const char* what() const noexcept override
        {
            return "AnyGUIObject::type_cast failure wrong type";
        }
    };

    AnyGUIObject()
        : p_data(nullptr)
    {
    }

    template<typename T>
    AnyGUIObject(T t)
        : p_data(std::make_unique<Model<T>>(std::move(t)))
    {
    }

    AnyGUIObject(const AnyGUIObject& other)
        : p_data(other.p_data->clone())
    {
    }

    AnyGUIObject(AnyGUIObject&& other) noexcept = default;

    AnyGUIObject& operator=(AnyGUIObject other) noexcept
    {
        p_data = std::move(other.p_data);
        return *this;
    }

    ~AnyGUIObject() = default;

    std::string serialize() const
    {
        return p_data->serialize();
    }

    // TODO: Make an operator
    // TODO: this is runtime logic, which is what we were trying to
    // avoid in the first place
    template<typename T>
    static T type_cast(const AnyGUIObject& te)
    {
        Concept *base = te.p_data.get();
        if(const auto *const derived = dynamic_cast<Model<T>*>(base))
        {
            const T* const derived_p_data(static_cast<const T*>(derived->address()));
            return *derived_p_data;
            //return *(T*)te.p_data->address();
        }
        throw TypeErasedTypeCastException();
    }

private:

    class Concept
    {

    public:

        Concept() = default;

        Concept(const Concept&) = delete;

        Concept(Concept&&) = delete;

        virtual ~Concept() = default;

        virtual std::unique_ptr<Concept> clone() const = 0;

        virtual const void* address() const = 0;

        virtual void draw() const = 0;

        virtual void processEvent() const = 0;

        virtual std::string name() const = 0;

        virtual int getPosX() const = 0;
        virtual int getPosY() const = 0;
        virtual int getSizeX() const = 0;
        virtual int getSizeY() const = 0;

        virtual void setPos(const int, const int) = 0;
        virtual void setSize(const int, const int) = 0;

        virtual bool resizable() const = 0;
        virtual bool visible() const = 0;
        virtual bool inactive() const = 0;

        virtual void setResizable(const bool) const = 0;
        virtual void setVisible(const bool) const = 0;
        virtual void setInactive(const bool) const = 0; 

        // TODO: return by reference if U is a large object
        //template<typename U>
        //U get() const = 0;
        
    };


    template<typename T>
    class Model : public Concept
    {

    public:

        explicit
        Model(T t)
            : data(std::move(t))
        {

        }

        Model(const Model<T>& other) = delete;

        Model(Model<T>&& other) = delete;

        Model<T> operator=(Model<T> other) = delete;

        std::unique_ptr<Concept> clone() const override
        {
            return std::make_unique<Model<T>>(data);
        }

        const void * address() const override
        {
            //return std::remove_const_t<decltype(data)*>(&data);
            return &data;
            //return reinterpreted_cast<void*>(&data);
        }

        void draw() const override
        {
            p_data->draw();
        }

        void processEvent(Json::Value &event) const override
        {
            p_data->processEvent(event);
        }

        std::string name() const override
        {
            return p_data->name();
        }

        /*
        template<typename U>
        U get() const override
        {
            constexpr if(std::is_same(U, T))
            {
                return data;
            }
            else
            {
                static_assert(false);
            }
        }
        */
        
        int getPosX() const override
        {
            return p_data->getPosX();
        }

        int getPosY() const
        {
            return p_data->getPosY();
        }
        
        int getSizeX() const
        {
            return p_data->getSizeX();
        }
        
        int getSizeY() const
        {
            return p_data->getSizeY();
        }
        

        void setPos(const int x, const int y)
        {
            p_data->setPos(x, y);
        }
        
        void setSize(const int x, const int y)
        {
            p_data->setPos(x, y);
        }
        

        bool resizable() const
        {
            return p_data->resizable();
        }
        
        bool visible() const
        {
            return p_data->visible();
        }
        
        bool inactive() const
        {
            return p_data->inactive();
        }
        

        void setResizable(const bool resizable) const
        {
            p_data->setResizable(resizable)
        }
        
        void setVisible(const bool visible) const
        {
            p_data->setVisible(visible);
        }
        
        void setInactive(const bool inactive) const
        {
            p_data->setInactive(inactive);
        }
        

    private:

        T data;

    };

    std::unique_ptr<Concept> p_data;

};


#endif
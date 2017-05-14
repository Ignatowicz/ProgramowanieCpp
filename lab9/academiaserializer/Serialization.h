//
// Created by matematyk60 on 09.05.17.
//

#ifndef JIMP_EXERCISES_SERIALIZATION_H
#define JIMP_EXERCISES_SERIALIZATION_H


#include <iosfwd>
#include <string>
#include <memory>
#include <vector>

using namespace std;
using namespace std::literals;

namespace academia{
    class Serializable;



    class Serializer{
    public:
        Serializer(std::ostream *output);
        virtual void IntegerField(const std::string &field_name, int value) = 0;
        virtual void DoubleField(const std::string &field_name, double value) = 0;
        virtual void StringField(const std::string &field_name, const std::string &value) = 0;
        virtual void BooleanField(const std::string &field_name, bool value) = 0;
        virtual void SerializableField(const std::string &field_name, const Serializable &value) = 0;
        virtual void ArrayField(const std::string &field_name,
                                const vector<reference_wrapper<const Serializable>> &value) = 0;
        virtual void Header(const std::string &object_name) = 0;
        virtual void Footer(const std::string &object_name) = 0;
    protected:
        std::ostream *output_;
    };

    class Serializable{
    public:
        virtual void Serialize(Serializer *serializer) const = 0;
    };




    class Room : public Serializable{
    public:

        enum Type{
            COMPUTER_LAB,
            LECTURE_HALL,
            CLASSROOM,
        };
        std::string TypeToString(Type type) const;
        Room(int id,const string &name, Type);

        void Serialize(Serializer *serializer) const override;

    private:
        int id_;
        string name_;
        Type type_;

    };

    class XmlSerializer : public Serializer{
    public:
        XmlSerializer(std::ostream *output);

        void IntegerField(const std::string &field_name, int value) override;

        void DoubleField(const std::string &field_name, double value) override;

        void StringField(const std::string &field_name, const std::string &value) override;

        void BooleanField(const std::string &field_name, bool value) override;

        void SerializableField(const std::string &field_name, const Serializable &value) override;

        void
        ArrayField(const std::string &field_name, const vector<reference_wrapper<const Serializable>> &value) override;

        void Header(const std::string &object_name) override;

        void Footer(const std::string &object_name) override;
    };

    class Building : public Serializable{
    public:
        Building(int id, string name, std::initializer_list<reference_wrapper<Serializable>> elements);

        void Serialize(Serializer *serializer) const override;

    private:
        int id_;
        string name_;
        std::vector<reference_wrapper<const Serializable>> elements_;
    };
}


#endif //JIMP_EXERCISES_SERIALIZATION_H

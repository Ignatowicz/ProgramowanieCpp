//
// Created by matematyk60 on 09.05.17.
//

#include "Serialization.h"

namespace academia {


    Serializer::Serializer(std::ostream *output) {
        output_ = output;
    }

    Room::Room(int id, const string& name, Room::Type type) {
        id_ = id;
        name_ = name;
        type_ = type;
    }

    std::string Room::TypeToString(Room::Type type) const {
        switch (type) {
            case Type::COMPUTER_LAB:
                return "COMPUTER_LAB";
            case Type::CLASSROOM:
                return "CLASSROOM";
            case Type::LECTURE_HALL:
                return "LECTURE_HALL";
        }
    }

    void Room::Serialize(Serializer *serializer) const {
        serializer->Header("room");
        serializer->IntegerField("id",id_);
        serializer->StringField("name",name_);
        serializer->StringField("type",TypeToString(type_));
        serializer->Footer("room");
    }

    XmlSerializer::XmlSerializer(std::ostream *output) : Serializer(output) {

    }

    void XmlSerializer::IntegerField(const std::string &field_name, int value) {
        *output_ << "<" + field_name + ">"+ std::to_string(value) + "<\\" + field_name + ">";
    }

    void XmlSerializer::DoubleField(const std::string &field_name, double value) {
        *output_ << "<" + field_name + ">" + std::to_string(value) + "<\\" + field_name + ">";
    }

    void XmlSerializer::StringField(const std::string &field_name, const std::string &value) {
        *output_ << "<" + field_name + ">" + value + "<\\" + field_name + ">";
    }

    void XmlSerializer::BooleanField(const std::string &field_name, bool value) {
        *output_ << "<" + field_name + ">" + std::to_string(value) + "<\\" + field_name + ">";
    }

    void XmlSerializer::SerializableField(const std::string &field_name, const Serializable &value) {
        *output_ << "<" + field_name + ">";
        value.Serialize(this);
        *output_ <<  "<\\" + field_name + ">";
    }

    void XmlSerializer::ArrayField(const std::string &field_name,
                                   const vector<reference_wrapper<const Serializable>> &value) {
        Header(field_name);

        for( const Serializable &n : value){
            n.Serialize(this);
        }

        Footer(field_name);
    }

    void XmlSerializer::Header(const std::string &object_name) {
        *output_ << "<" + object_name + ">";
    }

    void XmlSerializer::Footer(const std::string &object_name) {
        *output_ << "<\\" + object_name + ">";
    }

    Building::Building(int id, string name, std::initializer_list<reference_wrapper<Serializable>> elements) {
        id_ = id;
        name_ = name;
        for(Serializable &n : elements){
            elements_.emplace_back(n);
        }
    }

    void Building::Serialize(Serializer *serializer) const {
        serializer->Header("building");
        serializer->IntegerField("id", id_);
        serializer->StringField("name", name_);
        serializer->ArrayField("rooms", elements_);
        serializer->Footer("building");

    }
}
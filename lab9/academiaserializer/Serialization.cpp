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

    std::string Room::TypeToString() const {
        switch (type_) {
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
        serializer->StringField("type",TypeToString());
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

    Building::Building(int id, string name, std::vector<Room> elements) {
        id_ = id;
        name_ = name;
        for(auto n : elements){
            elements_.emplace_back(n);
        }
    }

    void Building::Serialize(Serializer *serializer) const {
        serializer->Header("building");
        serializer->IntegerField("id", id_);
        serializer->StringField("name", name_);
        std::vector<std::reference_wrapper<const Serializable>> tmp;
        for(const Room &n : elements_){
            tmp.emplace_back(n);
        }
        serializer->ArrayField("rooms", tmp);
        serializer->Footer("building");

    }

    JsonSerializer::JsonSerializer(std::ostream *output) : Serializer(output) {

    }

    void JsonSerializer::NextField(void) {
        string tmp = ", ";
        *output_<< tmp;
    }

    void JsonSerializer::GetName(const std::string &field_name) {
        *output_ << "\"" + field_name + "\": ";
    }

    void JsonSerializer::IntegerField(const std::string &field_name, int value) {
        GetName(field_name);
        *output_ << std::to_string(value);
        NextField();
    }

    void JsonSerializer::DoubleField(const std::string &field_name, double value) {
        GetName(field_name);
        *output_ << std::to_string(value);
        NextField();
    }

    void JsonSerializer::StringField(const std::string &field_name, const std::string &value) {
        GetName(field_name);
        *output_ << "\"" + value + "\"";
        if(field_name != "type"){
            NextField();
        }

    }

    void JsonSerializer::BooleanField(const std::string &field_name, bool value) {
        GetName(field_name);
        *output_ << std::to_string(value);
        NextField();
    }

    void JsonSerializer::SerializableField(const std::string &field_name, const Serializable &value) {
        GetName(field_name);
        value.Serialize(this);
        NextField();

    }

    void JsonSerializer::ArrayField(const std::string &field_name,
                                    const vector<reference_wrapper<const Serializable>> &value) {
        GetName(field_name);
        string open = "[", close = "]";
        *output_ << open;
        int i  = 1;
        for(const Serializable &n : value){
            n.Serialize(this);
            if(i != value.size()){
                NextField();
            }
            i++;
        }
        *output_ << close;
    }

    void JsonSerializer::Header(const std::string &object_name) {
        string open = "{";
        *output_ << open;
    }

    void JsonSerializer::Footer(const std::string &object_name) {
        string close = "}";
        *output_ << close;
    }

    BuildingRepository::BuildingRepository(const std::initializer_list<Building> &elements) {
        for( auto &n : elements){
            elements_.emplace_back(n);
        }
    }


    BuildingRepository::BuildingRepository() {
        elements_.clear();
    }

    void BuildingRepository::StoreAll(Serializer *serializer) const {
        serializer->Header("building_repository");
        std::vector<std::reference_wrapper<const Serializable>> tmp;
        for(const Building &n : elements_){
            tmp.emplace_back(n);
        }
        serializer->ArrayField("buildings",tmp);
        serializer->Footer("building_repository");
    }

    void BuildingRepository::Add(const Building &b) {
        elements_.emplace_back(b);
    }

    int Building::Id(void) const {
        return id_;
    }

    std::experimental::optional<Building> BuildingRepository::operator[](int id) const {
        for(auto &n : elements_){
            if(n.Id() == id){
                return std::experimental::make_optional(n);
            }
        }

        return experimental::optional<Building>();
    }


}
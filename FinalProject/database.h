#pragma once
#include "date.h"
#include "event_set.h"

#include <iostream>
#include <map>

struct Entry {
    Date date;
    string event;
};

ostream& operator<<(ostream& os, const Entry& e);

bool operator==(const Entry& lhs, const Entry& rhs);

bool operator!=(const Entry& lhs, const Entry& rhs);

class Database {
public:
    void Add(const Date& date, const string& event);


    void Print(ostream& os) const;

    template<typename Predicate>
    vector<Entry> FindIf(Predicate predicate) const {
        vector<Entry> result;
        for (auto& v : storage) {
            for (const auto& event : v.second.GetAll()) {
                if (predicate(v.first, event)) {
                    result.push_back(Entry{ v.first, event });
                }
            }
        }
        return result;
    }

    template <typename Predicate>
    int RemoveIf(Predicate predicate) {
        int result = 0;
        for (auto& v : storage) {
            const Date& date = v.first;
            result += v.second.RemoveIf([=](const string& event) {
                return predicate(date, event);
                });
        }
        for (auto it = storage.begin(); it != storage.end(); ) {
            if (it->second.GetAll().empty()) {
                storage.erase(it++);
            }
            else {
                ++it;
            }
        }
        return result;
    }

    Entry Last(const Date& date) const;

private:
    map<Date, EventSet> storage;
};

ostream& operator<<(ostream& stream, const pair<Date, string>& p);
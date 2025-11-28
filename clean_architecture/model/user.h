#pragma once
#include <string>
#include <chrono>
#include <optional>

using namespace std;
using namespace std::chrono;

// ユーザー情報を表す構造体
// 問題点：ただのデータ構造で、ドメインロジックが含まれていない
struct User {
    int id;
    string name;
    string email;
    string phone;
    system_clock::time_point created_at;
    system_clock::time_point updated_at;
    system_clock::time_point last_name_update;
    system_clock::time_point last_email_update;
    system_clock::time_point last_phone_update;
    
    User() : id(0), name(""), email(""), phone(""),
             created_at(system_clock::now()),
             updated_at(system_clock::now()),
             last_name_update(system_clock::now()),
             last_email_update(system_clock::now()),
             last_phone_update(system_clock::now()) {}
    
    User(int id, const string& name, const string& email, const string& phone)
        : id(id), name(name), email(email), phone(phone),
          created_at(system_clock::now()),
          updated_at(system_clock::now()),
          last_name_update(system_clock::now()),
          last_email_update(system_clock::now()),
          last_phone_update(system_clock::now()) {}
};

// ユーザー更新リクエスト
struct UpdateUserRequest {
    optional<string> name;
    optional<string> email;
    optional<string> phone;
};


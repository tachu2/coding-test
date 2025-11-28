#pragma once
#include "../model/user.h"
#include <map>
#include <memory>
#include <stdexcept>

using namespace std;

// オンメモリのデータベース
// 問題点：具象クラスで、インターフェースが定義されていない
class Database {
private:
    map<int, User> users;
    int next_id = 1;

public:
    Database() {}
    
    // ユーザーを作成
    User create_user(const string& name, const string& email, const string& phone) {
        User user(next_id, name, email, phone);
        users[next_id] = user;
        next_id++;
        return user;
    }
    
    // ユーザーを取得
    User* find_user_by_id(int id) {
        auto it = users.find(id);
        if (it != users.end()) {
            return &it->second;
        }
        return nullptr;
    }
    
    // ユーザー情報を更新
    void update_user(const User& user) {
        auto it = users.find(user.id);
        if (it != users.end()) {
            it->second = user;
            it->second.updated_at = system_clock::now();
        } else {
            throw runtime_error("User not found");
        }
    }
};


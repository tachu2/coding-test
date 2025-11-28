#pragma once
#include "../model/user.h"
#include <map>
#include <stdexcept>

using namespace std;

// データベース
// Warning: 本当のDBとして扱うこと
// Userの参照先は変えてもOKです
class Database {
private:
    map<int, User> users;
    int next_id = 1;

public:
    Database() {}
    
    // ユーザーを作成
    User create_user(const User& user) {
        users[user.id] = user;
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
        } else {
            throw runtime_error("User not found");
        }
    }
};

